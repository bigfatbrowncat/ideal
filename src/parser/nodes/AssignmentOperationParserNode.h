/*
 * AssignOperationParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef ASSIGNOPERATIONPARSERNODE_H_
#define ASSIGNOPERATIONPARSERNODE_H_

#include <iterator>

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

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
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

	virtual set<DataType> getSupportedTypes() const
	{
		set<DataType> s1 = first->getSupportedTypes();
		set<DataType> s2 = second->getSupportedTypes();
		set<DataType> res;
		set_intersection(s1.begin(), s1.end(), s2.begin(), s2.end(), insert_iterator<set<DataType>>(res, res.begin()));
		return res;
	}

	virtual void setActualType(DataType actualType)
	{
		ParserNode::setActualType(actualType);
		first->setActualType(actualType);
		second->setActualType(actualType);
	}

	AssignmentOperationParserNode(ParserNode* first, ParserNode* second, ParserVariables& vars) :
		ParserNode(vars), first(first), second(second)
	{

	}

};

#endif /* ASSIGNOPERATIONPARSERNODE_H_ */
