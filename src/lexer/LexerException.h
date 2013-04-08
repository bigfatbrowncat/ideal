/*
 * LexerException.h
 *
 *  Created on: Apr 8, 2013
 *      Author: imizus
 */

#ifndef LEXEREXCEPTION_H_
#define LEXEREXCEPTION_H_

#include <exception>
#include <string>

using namespace std;

class LexerException : public exception
{
private:
	string message;
public:
	LexerException(const string& message);
	const string& getMessage() const { return message; }
	virtual ~LexerException() throw ();
};

class UnexpectedCharLexerException : public LexerException
{
public:
	UnexpectedCharLexerException(char ch) :
		LexerException(string("Unexpected character: ") + ch) {}
};

class UnexpectedClosingRoundBraceException : public LexerException
{
public:
	UnexpectedClosingRoundBraceException() :
		LexerException("Unexpected closing round brace") {}
};

class UnexpectedClosingSquareBraceException : public LexerException
{
public:
	UnexpectedClosingSquareBraceException() :
		LexerException("Unexpected closing square brace") {}
};

class UnexpectedClosingCurlyBraceException : public LexerException
{
public:
	UnexpectedClosingCurlyBraceException() :
		LexerException("Unexpected closing curly brace") {}
};

class MissingClosingBraceException : public LexerException
{
public:
	MissingClosingBraceException() :
		LexerException("Missing closing brace") {}
};

#endif /* LEXEREXCEPTION_H_ */
