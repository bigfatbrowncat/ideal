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
#include "nodes/VariableDeclarationParserNode.h"
#include "nodes/ReturnParserNode.h"
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

ParserNode* Parser::parseExpression(const list<LexerTreeItem*>& source, ParserVariables& vars)
{
	vector<ParserItemWrapper> items;

	// Wrapping lexer primitives into parser ParserItemWrapper objects
	bool previousIsOperand = false;
	for (list<LexerTreeItem*>::const_iterator iter = source.begin(); iter != source.end(); iter++)
	{
		if ((*iter)->getInnerText() == "+")
		{
			items.push_back(ParserItemWrapper::withOperator(foAdd));
			previousIsOperand = false;
		}
		else if ((*iter)->getInnerText() == "-")
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
		else if ((*iter)->getInnerText() == "*")
		{
			items.push_back(ParserItemWrapper::withOperator(foMultiply));
			previousIsOperand = false;
		}
		else if ((*iter)->getInnerText() == "/")
		{
			items.push_back(ParserItemWrapper::withOperator(foDivide));
			previousIsOperand = false;
		}
		else if ((*iter)->getInnerText() == "=")
		{
			items.push_back(ParserItemWrapper::withOperator(foEquate));
			previousIsOperand = false;
		}
		else if ((*iter)->getOuterBraces() == brRound)
		{
			items.push_back(ParserItemWrapper::withOperand(parseExpression((*iter)->getInnerItems(), vars)));
			previousIsOperand = true;
		}
		else if ((*iter)->getOuterBraces() == brCurly)
		{
			items.push_back(ParserItemWrapper::withOperand(parseFlow((*iter)->getInnerItems(), vars)));
			previousIsOperand = true;
		}
		else if (ConstantParserNode::isDoubleConstant((*iter)->getInnerText()))
		{
			ConstantParserNode* cfi = new ConstantParserNode(ConstantParserNode::parseDouble((*iter)->getInnerText(), vars));
			items.push_back(ParserItemWrapper::withOperand(cfi));
			previousIsOperand = true;
		}
		else if (vars.contains((*iter)->getInnerText(), tDouble))
		{
			VariableParserNode* vfi = new VariableParserNode((*iter)->getInnerText(), tDouble, vars);
			items.push_back(ParserItemWrapper::withOperand(vfi));
			previousIsOperand = true;
		}
		else
		{
			throw InvalidTokenParserException((*iter)->getInnerText());
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

			UnaryOperationParserNode* fuo = new UnaryOperationParserNode(
			        items[highest_priority_index + 1].parserOperand,
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

ParserNode* Parser::parseLine(const list<LexerTreeItem*>& source, ParserVariables& vars)
{
	// Checking if we have a variable declaration
	list<LexerTreeItem*>::const_iterator iter = source.begin();
	if ((*iter)->getInnerText() == "double")
	{
		iter++;
		if (iter != source.end())
		{
			if ((*iter)->isValidVariable())
			{
				string variableName = (*iter)->getInnerText();
				if (!vars.contains(variableName, tDouble))
				{
					ParserNode* initNode;

					iter++;
					if (iter != source.end())
					{
						if ((*iter)->getInnerText() == "=")
						{
							iter++;
							list<LexerTreeItem*> currentLineCopy;
							while (iter != source.end())
							{
								currentLineCopy.push_back(*iter);
								iter++;
							}

							initNode = parseExpression(currentLineCopy, vars);
							// Checking type compatibility
							set<DataType> rightSideTypes = initNode->getSupportedTypes();
							if (rightSideTypes.find(tDouble) != rightSideTypes.end())
							{
								// Theright side is compatible with double type
								initNode->setActualType(tDouble);
							}
							else
							{
								throw TypeMismatchParserException(string("Incompatible types between the variable ") + variableName + " and its initialization value");
							}
						}
						else
						{
							throw ParserException("Is this a variable initialization? Then '=' expected.");
						}
					}
					else
					{
						initNode = NULL;	// No definition
					}

					vars.define(variableName, tDouble);
					return new VariableDeclarationParserNode(variableName, tDouble, 0.0, false, initNode, vars);
				}
				else
				{
					throw ParserException(((string)"Variable '") + (*iter)->getInnerText() + "' is already defined");
				}
			}
			else
			{
				throw ParserException(((string)"Invalid variable name: '") + (*iter)->getInnerText() + "'");
			}
		}
		else
		{
			throw ParserException("Missing variable name in the declaration");
		}
	}
	else if ((*iter)->getInnerText() == "return")
	{
		iter++;
		if (iter != source.end())
		{
			list<LexerTreeItem*> currentLineCopy;
			while (iter != source.end())
			{
				currentLineCopy.push_back(*iter);
				iter++;
			}
			// TODO Take the actual return type from the function declaration
			DataType returnType = tDouble;

			ParserNode* res = parseExpression(currentLineCopy, vars);
			set<DataType> resTypes = res->getSupportedTypes();
			if (resTypes.find(returnType) != resTypes.end())
			{
				res->setActualType(returnType);
			}
			else
			{
				throw TypeMismatchParserException("Invalid return type");
			}

			return new ReturnParserNode(ReturnParserNode::Expression, res, vars);
		}
		else
		{
			throw ParserException("Missing expression after 'return'");
		}
	}
	else
	{
		ParserNode* res = parseExpression(source, vars);
		set<DataType> resTypes = res->getSupportedTypes();
		if (resTypes.size() > 0)
		{
			// If we have at least one type, setting the first one as the actual.
			res->setActualType(*resTypes.begin());
			return res;
		}
		else
		{
			throw TypeMismatchParserException("We have a type incompatibility here");
		}

	}
}

ParserNode* Parser::parseFlow(const list<LexerTreeItem*>& source, ParserVariables& vars)
{
	ExecutionFlowParserNode* res = new ExecutionFlowParserNode(vars);
	list<LexerTreeItem*> currentLine;

	for (list<LexerTreeItem*>::const_iterator iter = source.begin(); iter != source.end(); iter++)
	{
		if ((*iter)->getInnerText() == ";")
		{
			if (currentLine.size() > 0)
			{
				res->addParserNode(parseLine(currentLine, vars));
			}
			else
			{
				// Empty operator. Just doing nothing
			}
			currentLine.clear();
		}
		else
		{
			currentLine.push_back(*iter);
		}
	}

	// And the last line...
	if (currentLine.size() > 0)
	{
		res->addParserNode(parseLine(currentLine, vars));
	}

	return res;
}


Parser::Parser(const ParserOperatorPriorities& priorities) : priorities(priorities)
{
}

Parser::~Parser() {
	// TODO Auto-generated destructor stub
}

