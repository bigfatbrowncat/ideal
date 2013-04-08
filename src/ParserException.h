/*
 * ParsingException.h
 *
 *  Created on: Apr 8, 2013
 *      Author: imizus
 */

#ifndef PARSEREXCEPTION_H_
#define PARSEREXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

class ParserException : exception
{
private:
	string message;
public:
	ParserException(const string& message);
	const string& getMessage() const
	{
		return message;
	}

	virtual ~ParserException() throw ();
};

class UndefinedVariableParserException : ParserException
{
public:
	UndefinedVariableParserException(const string& variableName) :
		ParserException(string("Undefined variable: ") + variableName)
	{
	}
};

class VariableRedifinitionParserException : ParserException
{
public:
	VariableRedifinitionParserException(const string& variableName) :
		ParserException(string("Variable redifinition: ") + variableName)
	{
	}
};

class VariableCreationCodeGeneratedParserException : ParserException
{
public:
	VariableCreationCodeGeneratedParserException(const string& variableName) :
		ParserException(string("Variable creation code is already generated: ") + variableName)
	{
	}
};

class OperandWantedParserException : ParserException
{
public:
	OperandWantedParserException() :
		ParserException(string("Operand wanted"))
	{
	}
};

class InvalidTokenParserException : ParserException
{
public:
	InvalidTokenParserException(const string& token) :
		ParserException(string("invalid token: ") + token)
	{
	}
};

#endif /* PARSINGEXCEPTION_H_ */
