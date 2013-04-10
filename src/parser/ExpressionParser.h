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

#include "../lexer/Lexer.h"
#include "nodes/ParserNode.h"
#include "ParserVariables.h"
#include "ParserOperatorPriorities.h"

using namespace std;
using namespace llvm;

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
