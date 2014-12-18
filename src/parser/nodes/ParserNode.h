/*
 * ParserNode.h
 *
 *  Created on: Apr 8, 2013
 *      Author: imizus
 */

#ifndef PARSERNODE_H_
#define PARSERNODE_H_

#include <map>

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include "../ParserVariables.h"

using namespace std;
using namespace llvm;

class ParserNode
{
private:
	ParserVariables& vars;
protected:
	ParserNode(ParserVariables& vars) : vars(vars) {}
public:
	virtual bool canBeAssigned() const = 0;
	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const = 0;
	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const = 0;
	ParserVariables& getVariables() const { return vars; }
	virtual ~ParserNode() {}
};


#endif /* PARSERNODE_H_ */
