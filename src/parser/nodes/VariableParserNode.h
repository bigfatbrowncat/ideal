/*
 * VariableParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef VARIABLEPARSERNODE_H_
#define VARIABLEPARSERNODE_H_

#include "ParserNode.h"

class VariableParserNode : public ParserNode
{
private:
	string name;
public:
	bool canBeAssigned() const
	{
		return true;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
	{
		return getVariables().generateLLVMVariableGetValueCode(name, builder);
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
		getVariables().generateLLVMVariableSetValueCode(name, value, builder);
	}

	VariableParserNode(string name, ParserVariables& vars) : ParserNode(vars), name(name)
	{

	}
};
#endif /* VARIABLEPARSERNODE_H_ */
