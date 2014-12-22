/*
 * VariableParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef RETURNPARSERNODE_H_
#define RETURNPARSERNODE_H_

#include "ParserNode.h"

class ReturnParserNode : public ParserNode
{
public:
	enum Type { Void, Expression };

private:
	Type type;
	ParserNode* node;

public:
	bool canBeAssigned() const
	{
		return true;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
	{
		if (type == Void)
		{
			builder.CreateRetVoid();
		}
		else if (type == Expression)
		{
			Value* value = node->generateGetValueLLVMCode(builder);
			builder.CreateRet(value);
		}
		else
		{
			throw ParserException("Incorrect type");
		}

		return NULL;
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
	}

	virtual set<DataType> getSupportedTypes() const
	{
		return set<DataType>();
	}

	ReturnParserNode(Type type, ParserNode* node, ParserVariables& vars) :
		ParserNode(vars), type(type), node(node)
	{

	}
};
#endif /* RETURNPARSERNODE_H_ */
