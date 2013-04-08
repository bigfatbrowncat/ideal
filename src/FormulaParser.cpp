/*
 * FormulaParser.cpp
 *
 *  Created on: 06.04.2013
 *      Author: il
 */

#include "FormulaParser.h"

struct ParserItemWrapper
{
	enum Type { tOperand, tOperator };

	FormulaItem* opd;
	FormulaOperator opt;

	Type type;

	static ParserItemWrapper withOperand(FormulaItem* operand)
	{
		ParserItemWrapper res;
		res.opd = operand;
		res.type = tOperand;
		return res;
	}
	static ParserItemWrapper withOperator(FormulaOperator opertor)
	{
		ParserItemWrapper res;
		res.opt = opertor;
		res.type = tOperator;
		return res;
	}
};

FormulaItem* FormulaParser::parse(const LexerTreeItem& source, const OperatorPriorities& priorities, Variables& vars)
{
	vector<ParserItemWrapper> operands;

	bool previousIsOperand = false;
	for (list<LexerTreeItem>::const_iterator iter = source.getInnerItems().begin(); iter != source.getInnerItems().end(); iter++)
	{
		if ((*iter).getInnerText() == "+")
		{
			operands.push_back(ParserItemWrapper::withOperator(foAdd));
			previousIsOperand = false;
		}
		else if ((*iter).getInnerText() == "-")
		{
			if (previousIsOperand)
			{
				operands.push_back(ParserItemWrapper::withOperator(foSubtract));
			}
			else
			{
				operands.push_back(ParserItemWrapper::withOperator(foUnaryMinus));
			}
			previousIsOperand = false;
		}
		else if ((*iter).getInnerText() == "*")
		{
			operands.push_back(ParserItemWrapper::withOperator(foMultiply));
			previousIsOperand = false;
		}
		else if ((*iter).getInnerText() == "/")
		{
			operands.push_back(ParserItemWrapper::withOperator(foDivide));
			previousIsOperand = false;
		}
		else if (ConstantFormulaItem::isParseable((*iter).getInnerText()))
		{
			ConstantFormulaItem* cfi = new ConstantFormulaItem(ConstantFormulaItem::parse((*iter).getInnerText(), vars));
			operands.push_back(ParserItemWrapper::withOperand(cfi));
			previousIsOperand = true;
		}
		else if (vars.contains((*iter).getInnerText()))
		{
			VariableFormulaItem* vfi = new VariableFormulaItem((*iter).getInnerText(), vars);
			operands.push_back(ParserItemWrapper::withOperand(vfi));
			previousIsOperand = true;
		}
		else
		{
			throw UNDECLARED_IDENTIFIER;
		}
	}


	while (operands.size() > 1)
	{
		size_t highest_priority_index = 0;
		int highest_priority = 0;

		for (size_t i = 0; i < operands.size(); i++)
		{
			if (operands[i].type == ParserItemWrapper::tOperator)
			{
				if (priorities.priorityOf(operands[i].opt) > highest_priority)
				{
					highest_priority = priorities.priorityOf(operands[i].opt);

					highest_priority_index = i;
				}
			}
		}

		if (operands[highest_priority_index].opt == foUnaryMinus)
		{
			// Unary operators

			if (highest_priority_index > operands.size() - 2 || operands[highest_priority_index + 1].type != ParserItemWrapper::tOperand)
			{
				throw WANTED_OPERAND;
			}

			FormulaUnaryOperation* fuo = new FormulaUnaryOperation(operands[highest_priority_index + 1].opd,
			                                                       operands[highest_priority_index].opt,
			                                                       vars);
			operands.erase(operands.begin() + highest_priority_index + 1);
			operands[highest_priority_index] = ParserItemWrapper::withOperand(fuo);
		}
		else
		{
			// Binary operators

			if (highest_priority_index < 1 || operands[highest_priority_index - 1].type != ParserItemWrapper::tOperand)
			{
				throw WANTED_OPERAND;
			}
			if (highest_priority_index > operands.size() - 2 || operands[highest_priority_index + 1].type != ParserItemWrapper::tOperand)
			{
				throw WANTED_OPERAND;
			}

			// Replacing the binary operation part with a single object
			FormulaBinaryOperation* fbo = new FormulaBinaryOperation(operands[highest_priority_index - 1].opd,
																	 operands[highest_priority_index].opt,
																	 operands[highest_priority_index + 1].opd,
																	 vars);
			operands.erase(operands.begin() + highest_priority_index + 1);
			operands.erase(operands.begin() + highest_priority_index);
			operands[highest_priority_index - 1] = ParserItemWrapper::withOperand(fbo);
		}
	}

	if (operands.front().type != ParserItemWrapper::tOperand)
	{
		throw WANTED_OPERAND;
	}

	return operands.front().opd;
}

FormulaParser::FormulaParser(const LexerTreeItem& lti, const OperatorPriorities& oprior, Variables& vars)
{
	root = parse(lti, oprior, vars);
}

FormulaParser::~FormulaParser() {
	// TODO Auto-generated destructor stub
}

