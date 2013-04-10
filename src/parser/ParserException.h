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

class ParserException : public exception
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

class UndefinedVariableParserException : public ParserException
{
public:
	UndefinedVariableParserException(const string& variableName) :
		ParserException(string("Undefined variable: ") + variableName)
	{
	}
};

class VariableRedifinitionParserException : public ParserException
{
public:
	VariableRedifinitionParserException(const string& variableName) :
		ParserException(string("Variable redifinition: ") + variableName)
	{
	}
};

class VariableCreationCodeGeneratedParserException : public ParserException
{
public:
	VariableCreationCodeGeneratedParserException(const string& variableName) :
		ParserException(string("Variable creation code is already generated: ") + variableName)
	{
	}
};

class OperandWantedParserException : public ParserException
{
public:
	OperandWantedParserException() :
		ParserException(string("Operand wanted"))
	{
	}
};

class InvalidTokenParserException : public ParserException
{
public:
	InvalidTokenParserException(const string& token) :
		ParserException(string("invalid token: ") + token)
	{
	}
};

class NotAssignableParserException : public ParserException
{
public:
	NotAssignableParserException() :
		ParserException(string("Trying to assign value to something not assignable"))
	{
	}
};

#endif /* PARSINGEXCEPTION_H_ */