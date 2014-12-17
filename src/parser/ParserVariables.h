/*
 * ParserVariables.h
 *
 *  Created on: Apr 8, 2013
 *      Author: imizus
 */

#ifndef PARSERVARIABLES_H_
#define PARSERVARIABLES_H_

#include <map>
#include <string>

#include <llvm/IR/Value.h>
#include <llvm/IR/IRBuilder.h>

#include "ParserException.h"

using namespace std;
using namespace llvm;

class ParserVariables
{
private:
	map<string, AllocaInst*> values;
protected:
	AllocaInst* getLLVMVariableIdentifier(const string& name) const
	{
		map<string, AllocaInst*>::const_iterator iter = values.find(name);
		if (iter == values.end())
		{
			throw UndefinedVariableParserException(name);
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
			throw VariableRedifinitionParserException(name);
		}
	}

	void generateVariableCreationLLVMCode(const string& name, IRBuilder<>& builder)
	{
		map<string, AllocaInst*>::iterator iter = values.find(name);
		if (iter == values.end())
		{
			throw UndefinedVariableParserException(name);
		}
		else if ((*iter).second != NULL)
		{
			throw VariableCreationCodeGeneratedParserException(name);
		}
		else
		{
		  	AllocaInst* inst = builder.CreateAlloca(builder.getDoubleTy(), 0, name);
			(*iter).second = inst;
		}
	}


	StoreInst* generateLLVMVariableSetToConstantCode(const string& name, double value, IRBuilder<>& builder) const
	{
		Type* doubleType = builder.getDoubleTy();
		Value* xValue = ConstantFP::get(doubleType, value);
		return builder.CreateStore(xValue, getLLVMVariableIdentifier(name));
	}

	StoreInst* generateLLVMVariableSetValueCode(const string& name, Value* value, IRBuilder<>& builder) const
	{
		return builder.CreateStore(value, getLLVMVariableIdentifier(name));
	}

	LoadInst* generateLLVMVariableGetValueCode(const string& name, IRBuilder<>& builder) const
	{
		return builder.CreateLoad(getLLVMVariableIdentifier(name), name);
	}

	bool contains(const string& name) const
	{
		return values.find(name) != values.end();
	}
};


#endif /* PARSERVARIABLES_H_ */
