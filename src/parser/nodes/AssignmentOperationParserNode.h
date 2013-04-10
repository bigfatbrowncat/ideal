/*
 * AssignOperationParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef ASSIGNOPERATIONPARSERNODE_H_
#define ASSIGNOPERATIONPARSERNODE_H_

#include "ParserNode.h"

class AssignmentOperationParserNode : public ParserNode
{
public:
private:
	ParserNode *first, *second;
public:
	bool canBeAssigned() const
	{
		return false;
	}

	virtual Value* generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
		return NULL;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
	{
		Value* secondVal = second->generateGetValueLLVMCode(builder);

		if (secondVal == NULL)
		{
			return NULL;
		}

		if (first->canBeAssigned())
		{
			first->generateSetValueLLVMCode(secondVal, builder);
			return first->generateGetValueLLVMCode(builder);
		}
		else
		{
			// TODO throw a proper exception
			return NULL;
		}

	}

	AssignmentOperationParserNode(ParserNode* first, ParserNode* second, ParserVariables& vars) :
		ParserNode(vars), first(first), second(second)
	{

	}

};

#endif /* ASSIGNOPERATIONPARSERNODE_H_ */
