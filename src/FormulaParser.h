/*
 * FormulaParser.h
 *
 *  Created on: 06.04.2013
 *      Author: il
 */

#ifndef FORMULAPARSER_H_
#define FORMULAPARSER_H_

#include <map>
#include <string>

#include <llvm/Value.h>
#include <llvm/IRBuilder.h>
#include <llvm/Constants.h>

#include "lexer/Lexer.h"
#include "ParserNode.h"
#include "ParserVariables.h"

using namespace std;
using namespace llvm;

enum ParserOperator
{
	foNegate,
	foAdd, foSubtract, foMultiply, foDivide,
	foEquate
};

class ParserOperatorPriorities
{
private:
	map<ParserOperator, int> priorities;
public:
	ParserOperatorPriorities()
	{
		priorities.insert(pair<ParserOperator, int>(foNegate, 4));
		priorities.insert(pair<ParserOperator, int>(foMultiply, 3));
		priorities.insert(pair<ParserOperator, int>(foDivide, 3));
		priorities.insert(pair<ParserOperator, int>(foAdd, 2));
		priorities.insert(pair<ParserOperator, int>(foSubtract, 2));
		priorities.insert(pair<ParserOperator, int>(foEquate, 1));
	}

	int priorityOf(ParserOperator oper) const
	{
		return priorities.at(oper);
	}
};

class ConstantParserNode : public ParserNode
{
private:
	double value;
public:
	bool canBeAssigned()
	{
		return false;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder)
	{
		Type* doubleType = builder.getDoubleTy();
		return ConstantFP::get(doubleType, value);
	}

	virtual Value* generateSetValueLLVMCode(Value* value, IRBuilder<>& builder)
	{
		return NULL;
	}

	ConstantParserNode(double value, ParserVariables& vars) : ParserNode(vars), value(value)
	{

	}

	static ConstantParserNode parse(string str, ParserVariables& vars)
	{
		double res = atof(str.c_str());
		return ConstantParserNode(res, vars);
	}
	
	static bool isParseable(string str)
	{
		bool dotFound = false;
		for (size_t i = 0; i < str.length(); i++)
		{
			if (str[i] == '.')
			{
				if (!dotFound)
				{
					dotFound = true;
				}
				else
				{
					// double dot
					return false;
				}
			}
			else
			{
				if (str[i] < '0' || str[i] > '9')
				{
					// Not a digit
					return false;
				}
			}
		}

		// All characters are passed correctly
		return true;
	}
};

class VariableParserNode : public ParserNode
{
private:
	string name;
public:
	bool canBeAssigned()
	{
		return true;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder)
	{
		return getVariables().generateLLVMVariableGetValueCode(name, builder);
	}

	virtual Value* generateSetValueLLVMCode(Value* value, IRBuilder<>& builder)
	{
		return getVariables().generateLLVMVariableSetValueCode(name, value, builder);
	}

	VariableParserNode(string name, ParserVariables& vars) : ParserNode(vars), name(name)
	{

	}
};

class UnaryOperationParserNode : public ParserNode
{
private:
	ParserNode *operand;
	ParserOperator formulaOperator;
public:
	bool canBeAssigned()
	{
		return false;
	}

	UnaryOperationParserNode(ParserNode *operand, ParserOperator formulaOperator, ParserVariables& vars) :
		ParserNode(vars),
		operand(operand), formulaOperator(formulaOperator)
	{

	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder)
	{
		Value* operandVal = operand->generateGetValueLLVMCode(builder);

		if (operandVal == NULL)
		{
			return NULL;
		}

		switch (formulaOperator)
		{
		case foNegate:
			return builder.CreateFNeg(operandVal, "neg");
		default:
			return NULL;
		}
	}

	virtual Value* generateSetValueLLVMCode(Value* value, IRBuilder<>& builder)
	{
		return NULL;
	}



};

class BinaryOperationParserNode : public ParserNode
{
private:
	ParserNode *first, *second;
	ParserOperator oper;
public:
	bool canBeAssigned()
	{
		return false;
	}

	virtual Value* generateSetValueLLVMCode(Value* value, IRBuilder<>& builder)
	{
		return NULL;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder)
	{
		Value* firstVal;
		if (oper != foEquate) // If firstVal isn't an l-value
		{
			firstVal = first->generateGetValueLLVMCode(builder);
		}
		Value* secondVal = second->generateGetValueLLVMCode(builder);

		if (firstVal == NULL || secondVal == NULL)
		{
			return NULL;
		}

		switch (oper)
		{
		case foAdd:
			return builder.CreateFAdd(firstVal, secondVal, "addop");
		case foSubtract:
			return builder.CreateFSub(firstVal, secondVal, "subop");
		case foMultiply:
			return builder.CreateFMul(firstVal, secondVal, "mulop");
		case foDivide:
			return builder.CreateFDiv(firstVal, secondVal, "divop");
		case foEquate:
		{
			if (first->canBeAssigned())
			{
				first->generateSetValueLLVMCode(secondVal, builder);
				return first->generateGetValueLLVMCode(builder);
			}
			else
			{
				// TODO throw a proper exception
				return NULL;
			}
		}

		default:
			return NULL;
		}
	}

	BinaryOperationParserNode(ParserNode* first, ParserOperator oper, ParserNode* second, ParserVariables& vars) :
		ParserNode(vars), first(first), second(second), oper(oper)
	{

	}
};

class ExpressionParser
{
private:
	const ParserOperatorPriorities& priorities;

public:
	ExpressionParser(const ParserOperatorPriorities& priorities);

	ParserNode* parse(const LexerTreeItem& source, ParserVariables& vars);

	virtual ~ExpressionParser();
};

#endif /* FORMULAPARSER_H_ */
