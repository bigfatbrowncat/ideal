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

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Constants.h>

#include "../lexer/Lexer.h"
#include "nodes/ParserNode.h"
#include "ParserVariables.h"
#include "ParserOperatorPriorities.h"

using namespace std;
using namespace llvm;

class Parser
{
private:
	const ParserOperatorPriorities& priorities;

public:
	Parser(const ParserOperatorPriorities& priorities);

	ParserNode* parseExpression(const list<LexerTreeItem*>& source, ParserVariables& vars);
	ParserNode* parseLine(const list<LexerTreeItem*>& source, ParserVariables& vars);
	ParserNode* parseFlow(const list<LexerTreeItem*>& source, ParserVariables& vars);

	virtual ~Parser();
};

#endif /* FORMULAPARSER_H_ */
