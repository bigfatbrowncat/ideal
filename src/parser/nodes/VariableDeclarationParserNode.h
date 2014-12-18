/*
 * VariableParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef VARIABLEDECLARATIONPARSERNODE_H_
#define VARIABLEDECLARATIONPARSERNODE_H_

#include "ParserNode.h"

class VariableDeclarationParserNode : public ParserNode
{
private:
	string name;
	double initConst;
	ParserNode *initNode;

public:
	bool canBeAssigned() const
	{
		return true;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
	{
		Value* initNodeVal = initNode->generateGetValueLLVMCode(builder);

		getVariables().generateVariableCreationLLVMCode(name, builder);

		if (initNodeVal != NULL)
		{
			getVariables().generateLLVMVariableSetValueCode(name, initNodeVal, builder);
		}
		else
		{
			getVariables().generateLLVMVariableSetToConstantCode(name, initConst, builder);
		}

		return NULL;
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
	}

	VariableDeclarationParserNode(string name, double initConst, ParserNode* initNode, ParserVariables& vars) :
		ParserNode(vars), name(name), initConst(initConst), initNode(initNode)
	{

	}
};
#endif /* VARIABLEDECLARATIONPARSERNODE_H_ */
