/*
 * UnaryOperationParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef UNARYOPERATIONPARSERNODE_H_
#define UNARYOPERATIONPARSERNODE_H_

#include "ParserNode.h"
#include "../ParserOperator.h"

class UnaryOperationParserNode : public ParserNode
{
private:
	ParserNode *operand;
	ParserOperator formulaOperator;
public:
	bool canBeAssigned() const
	{
		return false;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
	{
		Value* operandVal = operand->generateGetValueLLVMCode(builder);

		if (operandVal == NULL)
		{
			return NULL;
		}

		switch (formulaOperator)
		{
		case foNegate:
			return builder.CreateFNeg(operandVal, "neg");
		default:
			return NULL;
		}
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
	}

	virtual set<DataType> getSupportedTypes() const
	{
		return operand->getSupportedTypes();
	}

	UnaryOperationParserNode(ParserNode *operand, ParserOperator formulaOperator, ParserVariables& vars) :
		ParserNode(vars),
		operand(operand), formulaOperator(formulaOperator)
	{

	}

};

#endif /* UNARYOPERATIONPARSERNODE_H_ */
