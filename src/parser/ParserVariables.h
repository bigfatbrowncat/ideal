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
public:
	struct Variable
	{
		enum Type { tDouble, tBoolean };

		Type type;
		AllocaInst* value;
		Variable(Type type, AllocaInst* value) : type(type), value(value) {}
	};
private:
	map<string, Variable> values;

protected:
	const Variable& getLLVMVariableIdentifier(const string& name) const
	{
		map<string, Variable>::const_iterator iter = values.find(name);
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
	static Type* variableTypeToLLVMType(Variable::Type vt, IRBuilder<>& builder) {
		switch (vt)
		{
		case Variable::tDouble:
			return builder.getDoubleTy();
		case Variable::tBoolean:
			return builder.getInt1Ty();
		}
	}

	void define(const string& name, Variable::Type type) {
		if (values.find(name) == values.end())
		{
			values.insert(pair<string, Variable>(name, Variable(type, NULL)));
		}
		else
		{
			throw VariableRedifinitionParserException(name);
		}
	}

	void generateVariableCreationLLVMCode(const string& name, Variable::Type type, IRBuilder<>& builder)
	{
		map<string, Variable>::iterator iter = values.find(name);
		if (iter != values.end() && (*iter).second.type == type)
		{
		  	AllocaInst* inst = builder.CreateAlloca(variableTypeToLLVMType(type, builder), 0, name);

		  	// Saving the value for the new variable
		  	(*iter).second = Variable(type, inst);
		}
		else if (iter != values.end())
		{
			throw InvalidVariableTypeParserException(name);
		}
		else
		{
			throw UndefinedVariableParserException(name);
		}
	}

	StoreInst* generateLLVMVariableBooleanSetToConstantCode(const string& name, bool value, IRBuilder<>& builder) const
	{
		const Variable& var = getLLVMVariableIdentifier(name);
		if (var.type == Variable::tBoolean)
		{
			Type* booleanType = builder.getInt1Ty();
			Value* xValue = value ? ConstantInt::getTrue(booleanType) : ConstantInt::getFalse(booleanType);
			return builder.CreateStore(xValue, var.value);
		}
		else
		{
			throw InvalidVariableTypeParserException(name);
		}
	}

	StoreInst* generateLLVMVariableDoubleSetToConstantCode(const string& name, double value, IRBuilder<>& builder) const
	{
		const Variable& var = getLLVMVariableIdentifier(name);
		if (var.type == Variable::tDouble)
		{
			Type* doubleType = builder.getDoubleTy();
			Value* xValue = ConstantFP::get(doubleType, value);
			return builder.CreateStore(xValue, var.value);
		}
		else
		{
			throw InvalidVariableTypeParserException(name);
		}
	}

	StoreInst* generateLLVMVariableSetValueCode(const string& name, Variable::Type type, Value* value, IRBuilder<>& builder) const
	{
		if (getLLVMVariableIdentifier(name).type == type)
		{
			return builder.CreateStore(value, getLLVMVariableIdentifier(name).value);
		}
		else
		{
			throw InvalidVariableTypeParserException(name);
		}
	}

	LoadInst* generateLLVMVariableGetValueCode(const string& name, Variable::Type type, IRBuilder<>& builder) const
	{
		if (getLLVMVariableIdentifier(name).type == type)
		{
			return builder.CreateLoad(getLLVMVariableIdentifier(name).value, name);
		}
		else
		{
			throw InvalidVariableTypeParserException(name);
		}
	}

	bool contains(const string& name, Variable::Type type) const
	{
		map<string, Variable>::const_iterator iter = values.find(name);
		return iter != values.end() && iter->second.type == type;
	}
};


#endif /* PARSERVARIABLES_H_ */
