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
	double value;
public:
	bool canBeAssigned() const
	{
		return false;
	}

	virtual Value* generateGetValueLLVMCode(IRBuilder<>& builder) const
	{
		Type* doubleType = builder.getDoubleTy();
		return ConstantFP::get(doubleType, value);
	}

	virtual void generateSetValueLLVMCode(Value* value, IRBuilder<>& builder) const
	{
	}

	ConstantParserNode(double value, ParserVariables& vars) : ParserNode(vars), value(value)
	{

	}

	static ConstantParserNode parse(string str, ParserVariables& vars)
	{
		double res = atof(str.c_str());
		return ConstantParserNode(res, vars);
	}

	static bool isParsable(string str)
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
