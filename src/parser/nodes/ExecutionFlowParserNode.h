/*
 * ExecutionFlowParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef EXECUTIONFLOWPARSERNODE_H_
#define EXECUTIONFLOWPARSERNODE_H_

#include <list>

#include "ParserNode.h"

using namespace std;

class ExecutionFlowParserNode: public ParserNode
{
private:
	list<ParserNode*> flow;
public:
	ExecutionFlowParserNode(ParserVariables& vars);

	void addParserNode(ParserNode* node)
	{
		flow.push_back(node);
	}

	virtual bool canBeAssigned() const
	{
		return false;
	}
	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
	{
		Value* res = NULL;
		for (list<ParserNode*>::const_iterator iter = flow.begin(); iter != flow.end(); iter++)
		{
			res = (*iter)->generateGetValueLLVMCode(builder);
		}

		// Execution flow returns the last expression's result
		return res;
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
	}

	virtual set<DataType> getSupportedTypes() const
	{
		return set<DataType>();
	}


	virtual ~ExecutionFlowParserNode();
};

#endif /* EXECUTIONFLOWPARSERNODE_H_ */
