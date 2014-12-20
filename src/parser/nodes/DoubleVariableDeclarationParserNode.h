/*
 * VariableParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef DOUBLEVARIABLEDECLARATIONPARSERNODE_H_
#define DOUBLEVARIABLEDECLARATIONPARSERNODE_H_

#include "ParserNode.h"

class DoubleVariableDeclarationParserNode : public ParserNode
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

		getVariables().generateVariableCreationLLVMCode(name, ParserVariables::Variable::tDouble, builder);

		if (initNodeVal != NULL)
		{
			getVariables().generateLLVMVariableSetValueCode(name, ParserVariables::Variable::tDouble, initNodeVal, builder);
		}
		else
		{
			getVariables().generateLLVMVariableDoubleSetToConstantCode(name, initConst, builder);
		}

		return NULL;
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
	}

	DoubleVariableDeclarationParserNode(string name, double initConst, ParserNode* initNode, ParserVariables& vars) :
		ParserNode(vars), name(name), initConst(initConst), initNode(initNode)
	{

	}
};
#endif /* DOUBLEVARIABLEDECLARATIONPARSERNODE_H_ */
