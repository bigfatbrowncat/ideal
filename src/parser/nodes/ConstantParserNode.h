/*
 * ConstantParserNode.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef CONSTANTPARSERNODE_H_
#define CONSTANTPARSERNODE_H_

#include "ParserNode.h"

class ConstantParserNode : public ParserNode
{
private:
	double doubleVal;
	bool boolVal;
	DataType dataType;

protected:
	ConstantParserNode(DataType dataType, double doubleVal, bool boolVal, ParserVariables& vars) :
		ParserNode(vars), doubleVal(doubleVal), boolVal(boolVal), dataType(dataType)
	{
	}

public:
	bool canBeAssigned() const
	{
		return false;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
	{
		if (getActualType() == tDouble)
		{
			Type* doubleType = builder.getDoubleTy();
			return ConstantFP::get(doubleType, doubleVal);
		}
		else
		{
			Type* boolType = builder.getInt1Ty();
			return boolVal ? ConstantInt::getTrue(boolType) : ConstantInt::getFalse(boolType);
		}
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
	}

	virtual set<DataType> getSupportedTypes() const
	{
		set<DataType> res;
		res.insert(dataType);
		return res;
	}

	static ConstantParserNode parseDouble(const string& str, ParserVariables& vars)
	{
		double res = atof(str.c_str());
		return ConstantParserNode(tDouble, res, false, vars);
	}

	static ConstantParserNode parseBoolean(const string& str, ParserVariables& vars)
	{
		if (str == "true")
		{
			return ConstantParserNode(tBoolean, 0, true, vars);
		}
		else if (str == "false")
		{
			return ConstantParserNode(tBoolean, 0, false, vars);
		}
		else
		{
			throw InvalidTokenParserException(str);
		}
	}

	static bool isBooleanConstant(const string& str)
	{
		return (str == "true") || (str == "false");
	}

	static bool isDoubleConstant(const string& str)
	{
		bool dotFound = false;
		for (size_t i = 0; i < str.length(); i++)
		{
			if (str[i] == '.')
			{
				if (!dotFound)
				{
					dotFound = true;
				}
				else
				{
					// double dot
					return false;
				}
			}
			else
			{
				if (str[i] < '0' || str[i] > '9')
				{
					// Not a digit
					return false;
				}
			}
		}

		// All characters are passed correctly
		return true;
	}
};

#endif /* CONSTANTPARSERNODE_H_ */
