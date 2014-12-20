/*
 * LexerTools.h
 *
 *  Created on: 18 ???. 2014 ?.
 *      Author: il
 */

#ifndef LEXERTOOLS_H_
#define LEXERTOOLS_H_

static bool isDigit(char ch)
{
	return (ch >= '0' && ch <= '9');
}

static bool isDot(char ch)
{
	return ch == '.';
}

static bool isLetter(char ch)
{
	return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z');
}

static bool isValidFirstIdentifierChar(char ch)
{
	return isLetter(ch) || (ch == '_');
}

static bool isValidNonFirstIdentifierChar(char ch)
{
	return isLetter(ch) || (ch == '_') || isDigit(ch);
}

static bool isOperator(char ch)
{
	return (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/') || (ch == '=') || (ch == ';');
}

static bool isWhitespace(char ch)
{
	return (ch == ' ') || (ch == '\t') || (ch == '\r') || (ch == '\n');
}

// This function is used for lexing double-char operators such as '+=', '-=', '*=', '/='
static bool isValidOpWithEq(char ch)
{
	return (ch == '+') || (ch == '-') || (ch == '*') || (ch == '/');
}

// This function is used for lexing double-char operators such as '++', '--', '=='
static bool isValidOpDouble(char ch)
{
	return (ch == '+') || (ch == '-') || (ch == '=');
}


#endif /* LEXERTOOLS_H_ */
