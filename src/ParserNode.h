/*
 * ParserNode.h
 *
 *  Created on: Apr 8, 2013
 *      Author: imizus
 */

#ifndef PARSERNODE_H_
#define PARSERNODE_H_

#include <llvm/Value.h>
#include <llvm/IRBuilder.h>

#include <map>

using namespace std;
using namespace llvm;

#define WANTED_OPERATOR				1
#define WANTED_OPERAND				2

#define VARIABLE_UNDEFINED			3
#define VARIABLE_REDEFINITION		4

#define VARIABLE_CREATION_CODE_ALREADY_GENERATED		5

class ParserVariables
{
private:
	map<string, AllocaInst*> values;
protected:
	AllocaInst* getLLVMVariableIdentifier(const string& name)
	{
		map<string, AllocaInst*>::iterator iter = values.find(name);
		if (iter == values.end())
		{
			throw VARIABLE_UNDEFINED;
		}
		else
		{
			return (*iter).second;
		}
	}
public:
	void define(const string& name)
	{
		if (values.find(name) == values.end())
		{
			values.insert(pair<string, AllocaInst*>(name, NULL));
		}
		else
		{
			throw VARIABLE_REDEFINITION;
		}
	}

	void generateVariableCreationLLVMCode(const string& name, IRBuilder<>& builder)
	{
		map<string, AllocaInst*>::iterator iter = values.find(name);
		if (iter == values.end())
		{
			throw VARIABLE_UNDEFINED;
		}
		else if ((*iter).second != NULL)
		{
			throw VARIABLE_CREATION_CODE_ALREADY_GENERATED;
		}
		else
		{
		  	AllocaInst* inst = builder.CreateAlloca(builder.getDoubleTy(), 0, name);
			(*iter).second = inst;
		}
	}


	StoreInst* generateLLVMVariableSetValueCode(const string& name, double value, IRBuilder<>& builder)
	{
		Type* doubleType = builder.getDoubleTy();
		Value* xValue = ConstantFP::get(doubleType, value);
		return builder.CreateStore(xValue, getLLVMVariableIdentifier(name));
	}

	LoadInst* generateLLVMVariableGetValueCode(const string& name, IRBuilder<>& builder)
	{
		return builder.CreateLoad(getLLVMVariableIdentifier(name), name);
	}

	bool contains(const string& name)
	{
		return values.find(name) != values.end();
	}
};

class ParserNode
{
private:
	ParserVariables& vars;
protected:
	ParserNode(ParserVariables& vars) : vars(vars) {}
	virtual ~ParserNode() {}
public:
	virtual Value* generateLLVMCode(IRBuilder<>& builder) = 0;
	ParserVariables& getVariables() { return vars; }
};


#endif /* PARSERNODE_H_ */
