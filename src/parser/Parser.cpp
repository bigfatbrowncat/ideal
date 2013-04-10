/*
 * FormulaParser.cpp
 *
 *  Created on: 06.04.2013
 *      Author: il
 */

#include "Parser.h"

#include "nodes/AssignmentOperationParserNode.h"
#include "nodes/BinaryOperationParserNode.h"
#include "nodes/UnaryOperationParserNode.h"
#include "nodes/ConstantParserNode.h"
#include "nodes/VariableParserNode.h"
#include "nodes/ExecutionFlowParserNode.h"

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

ParserNode* Parser::parseExpression(const list<LexerTreeItem>& source, ParserVariables& vars)
{
	vector<ParserItemWrapper> items;

	// Wrapping lexer primitives into parser ParserItemWrapper objects
	bool previousIsOperand = false;
	for (list<LexerTreeItem>::const_iterator iter = source.begin(); iter != source.end(); iter++)
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
				items.push_back(ParserItemWrapper::withOperator(foNegate));
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
		else if ((*iter).getInnerText() == "=")
		{
			items.push_back(ParserItemWrapper::withOperator(foEquate));
			previousIsOperand = false;
		}
		else if ((*iter).getOuterBraces() == brRound)
		{
			items.push_back(ParserItemWrapper::withOperand(parseExpression((*iter).getInnerItems(), vars)));
			previousIsOperand = true;
		}
		else if ((*iter).getOuterBraces() == brCurly)
		{
			items.push_back(ParserItemWrapper::withOperand(parseFlow((*iter).getInnerItems(), vars)));
			previousIsOperand = true;
		}
		else if (ConstantParserNode::isParsable((*iter).getInnerText()))
		{
			ConstantParserNode* cfi = new ConstantParserNode(ConstantParserNode::parse((*iter).getInnerText(), vars));
			items.push_back(ParserItemWrapper::withOperand(cfi));
			previousIsOperand = true;
		}
		else if (vars.contains((*iter).getInnerText()))
		{
			VariableParserNode* vfi = new VariableParserNode((*iter).getInnerText(), vars);
			items.push_back(ParserItemWrapper::withOperand(vfi));
			previousIsOperand = true;
		}
		else
		{
			throw InvalidTokenParserException((*iter).getInnerText());
		}
	}

	// Processing operators
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

		if (items[highest_priority_index].parserOperator == foEquate)
		{
			// Assign operator

			if (highest_priority_index < 1 || items[highest_priority_index - 1].type != ParserItemWrapper::tOperand)
			{
				throw OperandWantedParserException();
			}
			if (highest_priority_index > items.size() - 2 || items[highest_priority_index + 1].type != ParserItemWrapper::tOperand)
			{
				throw OperandWantedParserException();
			}
			if (!items[highest_priority_index - 1].parserOperand->canBeAssigned())
			{
				throw NotAssignableParserException();
			}

			AssignmentOperationParserNode* fao = new AssignmentOperationParserNode(items[highest_priority_index - 1].parserOperand,
			                                                               items[highest_priority_index + 1].parserOperand,
			                                                               vars);
			items.erase(items.begin() + highest_priority_index + 1);
			items.erase(items.begin() + highest_priority_index);
			items[highest_priority_index - 1] = ParserItemWrapper::withOperand(fao);
		}
		else if (items[highest_priority_index].parserOperator == foNegate)
		{
			// Unary operators

			if (highest_priority_index > items.size() - 2 || items[highest_priority_index + 1].type != ParserItemWrapper::tOperand)
			{
				throw OperandWantedParserException();
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
				throw OperandWantedParserException();
			}
			if (highest_priority_index > items.size() - 2 || items[highest_priority_index + 1].type != ParserItemWrapper::tOperand)
			{
				throw OperandWantedParserException();
			}

			if (items[highest_priority_index].parserOperator == foEquate)
			{
				if (!items[highest_priority_index - 1].parserOperand->canBeAssigned())
				{
					throw NotAssignableParserException();
				}
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
		throw OperandWantedParserException();
	}

	return items.front().parserOperand;
}

ParserNode* Parser::parseFlow(const list<LexerTreeItem>& source, ParserVariables& vars)
{
	ExecutionFlowParserNode* res = new ExecutionFlowParserNode(vars);
	list<LexerTreeItem> currentExpression;

	for (list<LexerTreeItem>::const_iterator iter = source.begin(); iter != source.end(); iter++)
	{
		if ((*iter).getInnerText() == ";")
		{
			res->addParserNode(parseExpression(currentExpression, vars));
			currentExpression.clear();
		}
		else
		{
			currentExpression.push_back(*iter);
		}
	}

	if (currentExpression.size() > 0)
	{
		res->addParserNode(parseExpression(currentExpression, vars));
	}

	return res;
}


Parser::Parser(const ParserOperatorPriorities& priorities) : priorities(priorities)
{
}

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}
