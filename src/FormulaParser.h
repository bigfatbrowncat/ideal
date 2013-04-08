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

#include "llvm/Value.h"
#include "llvm/IRBuilder.h"
#include "llvm/Constants.h"

#include "Lexer.h"

using namespace std;
using namespace llvm;

#define WANTED_OPERATOR				1
#define WANTED_OPERAND				2

#define VARIABLE_UNDEFINED			3
#define VARIABLE_REDEFINITION		4

#define UNDECLARED_IDENTIFIER		5

enum FormulaOperator
{
	foUnaryMinus,
	foAdd, foSubtract, foMultiply, foDivide
};

class OperatorPriorities
{
private:
	map<FormulaOperator, int> priorities;
public:
	OperatorPriorities()
	{
		priorities.insert(pair<FormulaOperator, int>(foUnaryMinus, 3));
		priorities.insert(pair<FormulaOperator, int>(foMultiply, 2));
		priorities.insert(pair<FormulaOperator, int>(foDivide, 2));
		priorities.insert(pair<FormulaOperator, int>(foAdd, 1));
		priorities.insert(pair<FormulaOperator, int>(foSubtract, 1));
	}

	int priorityOf(FormulaOperator oper) const
	{
		return priorities.at(oper);
	}
};

class Variables
{
private:
	map<string, AllocaInst*> values;
public:
	void add(const string& name, AllocaInst* variable)
	{
		if (values.find(name) == values.end())
		{
			values.insert(pair<string, AllocaInst*>(name, variable));
		}
		else
		{
			throw VARIABLE_REDEFINITION;
		}
	}

	void set(const string& name, AllocaInst* variable)
	{
		map<string, AllocaInst*>::iterator iter = values.find(name);
		if (iter == values.end())
		{
			throw VARIABLE_UNDEFINED;
		}
		else
		{
			(*iter).second = variable;
		}
	}

	AllocaInst* get(const string& name)
	{
		map<string, AllocaInst*>::iterator iter = values.find(name);
		if (iter == values.end())
		{
			throw VARIABLE_UNDEFINED;
		}
		else
		{
			return (*iter).second;
		}
	}

	bool contains(const string& name)
	{
		return values.find(name) != values.end();
	}
};

class FormulaItem
{
private:
	Variables& vars;
protected:
	FormulaItem(Variables& vars) : vars(vars) {}
	virtual ~FormulaItem() {}
public:
	virtual Value* generateLLVMCode(IRBuilder<>& builder) = 0;
	Variables& getVariables() { return vars; }
};

class ConstantFormulaItem : public FormulaItem
{
private:
	double value;
public:
	virtual Value* generateLLVMCode(IRBuilder<>& builder)
	{
		Type* doubleType = Type::getDoubleTy(getGlobalContext());
		return ConstantFP::get(doubleType, value);
	}

	ConstantFormulaItem(double value, Variables& vars) : FormulaItem(vars), value(value)
	{

	}

	static ConstantFormulaItem parse(string str, Variables& vars)
	{
		double res = atof(str.c_str());
		return ConstantFormulaItem(res, vars);
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

class VariableFormulaItem : public FormulaItem
{
private:
	string name;
public:
	virtual Value* generateLLVMCode(IRBuilder<>& builder)
	{
		return builder.CreateLoad(getVariables().get(name), name);
	}

	VariableFormulaItem(string name, Variables& vars) : FormulaItem(vars), name(name)
	{

	}
};

class FormulaUnaryOperation : public FormulaItem
{
private:
	FormulaItem *operand;
	FormulaOperator formulaOperator;
public:
	FormulaUnaryOperation(FormulaItem *operand, FormulaOperator formulaOperator, Variables& vars) :
		FormulaItem(vars),
		operand(operand), formulaOperator(formulaOperator)
	{

	}

	virtual Value* generateLLVMCode(IRBuilder<>& builder)
	{
		Value* operandVal = operand->generateLLVMCode(builder);

		if (operandVal == NULL)
		{
			return NULL;
		}

		switch (formulaOperator)
		{
		case foUnaryMinus:
			return builder.CreateFNeg(operandVal, "neg");
		default:
			return NULL;
		}
	}

};

class FormulaBinaryOperation : public FormulaItem
{
private:
	FormulaItem *first, *second;
	FormulaOperator oper;
public:
	virtual Value* generateLLVMCode(IRBuilder<>& builder)
	{
		Value* firstVal = first->generateLLVMCode(builder);
		Value* secondVal = second->generateLLVMCode(builder);

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
		default:
			return NULL;
		}
	}

	FormulaBinaryOperation(FormulaItem* first, FormulaOperator oper, FormulaItem* second, Variables& vars) :
		FormulaItem(vars), first(first), second(second), oper(oper)
	{

	}
};

class FormulaParser
{
private:
	FormulaItem* root;

	static FormulaItem* parse(const LexerTreeItem& source, const OperatorPriorities& priorities, Variables& vars);
public:
	FormulaParser(const LexerTreeItem& lti, const OperatorPriorities& oprior, Variables& vars);
	virtual ~FormulaParser();

	FormulaItem* getRoot()
	{
		return root;
	}
};

#endif /* FORMULAPARSER_H_ */
