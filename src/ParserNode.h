/*
 * ParserNode.h
 *
 *  Created on: Apr 8, 2013
 *      Author: imizus
 */

#ifndef PARSERNODE_H_
#define PARSERNODE_H_

#include <map>

#include <llvm/Value.h>
#include <llvm/IRBuilder.h>

#include "ParserVariables.h"

using namespace std;
using namespace llvm;

class ParserNode
{
private:
	ParserVariables& vars;
protected:
	ParserNode(ParserVariables& vars) : vars(vars) {}
	virtual ~ParserNode() {}
public:
	virtual Value* generateLLVMCode(IRBuilder<>& builder) = 0;
	ParserVariables& getVariables() { return vars; }
};


#endif /* PARSERNODE_H_ */
