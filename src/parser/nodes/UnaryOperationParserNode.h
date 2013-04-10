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

	UnaryOperationParserNode(ParserNode *operand, ParserOperator formulaOperator, ParserVariables& vars) :
		ParserNode(vars),
		operand(operand), formulaOperator(formulaOperator)
	{

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

	virtual Value* generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
		return NULL;
	}



};

#endif /* UNARYOPERATIONPARSERNODE_H_ */
