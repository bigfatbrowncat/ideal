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
	bool canBeAssigned() const
	{
		return false;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
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
			return builder.CreateFAdd(firstVal, secondVal, "addOp");
		case foSubtract:
			return builder.CreateFSub(firstVal, secondVal, "subOp");
		case foMultiply:
			return builder.CreateFMul(firstVal, secondVal, "mulOp");
		case foDivide:
			return builder.CreateFDiv(firstVal, secondVal, "divOp");
		case foGreater:
			return builder.CreateFCmpOGT(firstVal, secondVal, "ordGrtOp");
		case foLower:
			return builder.CreateFCmpOLT(firstVal, secondVal, "ordLwrOp");
		case foGreaterEqual:
			return builder.CreateFCmpOGE(firstVal, secondVal, "ordGrtEqOp");
		case foLowerEqual:
			return builder.CreateFCmpOLE(firstVal, secondVal, "ordLwrEqOp");
		case foEqual:
			return builder.CreateFCmpOEQ(firstVal, secondVal, "ordEqOp");
		case foNonEqual:
			return builder.CreateFCmpONE(firstVal, secondVal, "ordNEqOp");

		default:
			return NULL;
		}
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
	}

	virtual set<DataType> getSupportedTypes() const
	{
		set<DataType> s1 = first->getSupportedTypes();
		set<DataType> s2 = second->getSupportedTypes();

		set<DataType> arithmeticTypes;
		arithmeticTypes.insert(tDouble);
		set<DataType> res1;
		set<DataType> res2;

		switch (oper) {
		case foEquate:
			set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), insert_iterator<set<DataType>>(res1, res1.begin()));
			return res1;

		case foAdd:
		case foSubtract:
		case foMultiply:
		case foDivide:
			set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), insert_iterator<set<DataType>>(res1, res1.begin()));
			set_intersection(res1.begin(), res1.end(), arithmeticTypes.begin(), arithmeticTypes.end(), insert_iterator<set<DataType>>(res2, res2.begin()));
			return res2;
		case foGreater:
		case foGreaterEqual:
		case foLower:
		case foLowerEqual:
		case foEqual:
		case foNonEqual:
			set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), insert_iterator<set<DataType>>(res1, res1.begin()));
			if (!res1.empty())
			{
				res2.insert(tBoolean);
			}
			return res2;
		default:
			throw ParserException("Incorrect operator");
		}
	}

	virtual void setActualType(DataType actualType)
	{
		ParserNode::setActualType(actualType);
		first->setActualType(actualType);
		second->setActualType(actualType);
	}

	BinaryOperationParserNode(ParserNode* first, ParserOperator oper, ParserNode* second, ParserVariables& vars) :
		ParserNode(vars), first(first), second(second), oper(oper)
	{

	}
};


#endif /* BINARYOPERATIONPARSERNODE_H_ */
