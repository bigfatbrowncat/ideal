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
	DataType dataType;
	double doubleInitConst;
	bool booleanInitConst;
	ParserNode *initNode;

public:
	bool canBeAssigned() const
	{
		return true;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
	{
		Value* initNodeVal = NULL;
		if (initNode != NULL) initNodeVal = initNode->generateGetValueLLVMCode(builder);

		getVariables().generateVariableCreationLLVMCode(name, dataType, builder);

		if (initNodeVal != NULL)
		{
			getVariables().generateLLVMVariableSetValueCode(name, dataType, initNodeVal, builder);
		}
		else
		{
			if (dataType == tDouble)
			{
				getVariables().generateLLVMVariableDoubleSetToConstantCode(name, doubleInitConst, builder);
			}
			else if (dataType == tBoolean)
			{
				getVariables().generateLLVMVariableBooleanSetToConstantCode(name, booleanInitConst, builder);
			}
			else
			{
				throw ParserException("Invalid type case");
			}
		}

		return NULL;
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
	}

	virtual set<DataType> getSupportedTypes() const
	{
		set<DataType> res;
		res.insert(dataType);
		return res;
	}

	VariableDeclarationParserNode(
			const string& name,
			DataType dataType,
			double doubleInitConst,
			bool booleanInitConst,
			ParserNode* initNode,
			ParserVariables& vars) :
		ParserNode(vars),
		name(name),
		dataType(dataType),
		doubleInitConst(doubleInitConst),
		booleanInitConst(booleanInitConst),
		initNode(initNode)
	{

	}
};
#endif /* VARIABLEDECLARATIONPARSERNODE_H_ */
