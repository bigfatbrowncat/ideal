/*
 * BinaryOperationParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef BINARYOPERATIONPARSERNODE_H_
#define BINARYOPERATIONPARSERNODE_H_

#include "ParserNode.h"
#include "../ParserOperator.h"

class BinaryOperationParserNode : public ParserNode
{
private:
	ParserNode *first, *second;
	ParserOperator oper;
public:
	bool canBeAssigned()
	{
		return false;
	}

	virtual Value* generateSetValueLLVMCode(Value* value, IRBuilder<>& builder)
	{
		return NULL;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder)
	{
		Value* firstVal = first->generateGetValueLLVMCode(builder);
		Value* secondVal = second->generateGetValueLLVMCode(builder);

		if (firstVal == NULL || secondVal == NULL)
		{
			return NULL;
		}

		switch (oper)
		{
		case foAdd:
			return builder.CreateFAdd(firstVal, secondVal, "addop");
		case foSubtract:
			return builder.CreateFSub(firstVal, secondVal, "subop");
		case foMultiply:
			return builder.CreateFMul(firstVal, secondVal, "mulop");
		case foDivide:
			return builder.CreateFDiv(firstVal, secondVal, "divop");

		default:
			return NULL;
		}
	}

	BinaryOperationParserNode(ParserNode* first, ParserOperator oper, ParserNode* second, ParserVariables& vars) :
		ParserNode(vars), first(first), second(second), oper(oper)
	{

	}
};


#endif /* BINARYOPERATIONPARSERNODE_H_ */
