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

	ParserNode* parserOperand;
	ParserOperator parserOperator;

	Type type;

	static ParserItemWrapper withOperand(ParserNode* parserOperand)
	{
		ParserItemWrapper res;
		res.parserOperand = parserOperand;
		res.type = tOperand;
		return res;
	}
	static ParserItemWrapper withOperator(ParserOperator parserOperator)
	{
		ParserItemWrapper res;
		res.parserOperator = parserOperator;
		res.type = tOperator;
		return res;
	}
};

ParserNode* ExpressionParser::parse(const LexerTreeItem& source, ParserVariables& vars)
{
	vector<ParserItemWrapper> items;

	bool previousIsOperand = false;
	for (list<LexerTreeItem>::const_iterator iter = source.getInnerItems().begin(); iter != source.getInnerItems().end(); iter++)
	{
		if ((*iter).getInnerText() == "+")
		{
			items.push_back(ParserItemWrapper::withOperator(foAdd));
			previousIsOperand = false;
		}
		else if ((*iter).getInnerText() == "-")
		{
			if (previousIsOperand)
			{
				items.push_back(ParserItemWrapper::withOperator(foSubtract));
			}
			else
			{
				items.push_back(ParserItemWrapper::withOperator(foUnaryMinus));
			}
			previousIsOperand = false;
		}
		else if ((*iter).getInnerText() == "*")
		{
			items.push_back(ParserItemWrapper::withOperator(foMultiply));
			previousIsOperand = false;
		}
		else if ((*iter).getInnerText() == "/")
		{
			items.push_back(ParserItemWrapper::withOperator(foDivide));
			previousIsOperand = false;
		}
		else if ((*iter).getOuterBraces() == brRound)
		{
			items.push_back(ParserItemWrapper::withOperand(parse(*iter, vars)));
			previousIsOperand = true;
		}
		else if (ConstantParserNode::isParseable((*iter).getInnerText()))
		{
			ConstantParserNode* cfi = new ConstantParserNode(ConstantParserNode::parse((*iter).getInnerText(), vars));
			items.push_back(ParserItemWrapper::withOperand(cfi));
			previousIsOperand = true;
		}
		else if (vars.contains((*iter).getInnerText()))
		{
			RValueParserNode* vfi = new RValueParserNode((*iter).getInnerText(), vars);
			items.push_back(ParserItemWrapper::withOperand(vfi));
			previousIsOperand = true;
		}
		else
		{
			throw UNDECLARED_IDENTIFIER;
		}
	}


	while (items.size() > 1)
	{
		size_t highest_priority_index = 0;
		int highest_priority = 0;

		for (size_t i = 0; i < items.size(); i++)
		{
			if (items[i].type == ParserItemWrapper::tOperator)
			{
				if (priorities.priorityOf(items[i].parserOperator) > highest_priority)
				{
					highest_priority = priorities.priorityOf(items[i].parserOperator);

					highest_priority_index = i;
				}
			}
		}

		if (items[highest_priority_index].parserOperator == foUnaryMinus)
		{
			// Unary operators

			if (highest_priority_index > items.size() - 2 || items[highest_priority_index + 1].type != ParserItemWrapper::tOperand)
			{
				throw WANTED_OPERAND;
			}

			UnaryOperationParserNode* fuo = new UnaryOperationParserNode(items[highest_priority_index + 1].parserOperand,
			                                                       items[highest_priority_index].parserOperator,
			                                                       vars);
			items.erase(items.begin() + highest_priority_index + 1);
			items[highest_priority_index] = ParserItemWrapper::withOperand(fuo);
		}
		else
		{
			// Binary operators

			if (highest_priority_index < 1 || items[highest_priority_index - 1].type != ParserItemWrapper::tOperand)
			{
				throw WANTED_OPERAND;
			}
			if (highest_priority_index > items.size() - 2 || items[highest_priority_index + 1].type != ParserItemWrapper::tOperand)
			{
				throw WANTED_OPERAND;
			}

			// Replacing the binary operation part with a single object
			BinaryOperationParserNode* fbo = new BinaryOperationParserNode(items[highest_priority_index - 1].parserOperand,
																	 items[highest_priority_index].parserOperator,
																	 items[highest_priority_index + 1].parserOperand,
																	 vars);
			items.erase(items.begin() + highest_priority_index + 1);
			items.erase(items.begin() + highest_priority_index);
			items[highest_priority_index - 1] = ParserItemWrapper::withOperand(fbo);
		}
	}

	if (items.front().type != ParserItemWrapper::tOperand)
	{
		throw WANTED_OPERAND;
	}

	return items.front().parserOperand;
}

ExpressionParser::ExpressionParser(const ParserOperatorPriorities& priorities) : priorities(priorities)
{
}

ExpressionParser::~ExpressionParser() {
	// TODO Auto-generated destructor stub
}

