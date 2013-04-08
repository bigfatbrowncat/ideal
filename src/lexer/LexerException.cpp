/*
 * LexerException.cpp
 *
 *  Created on: Apr 8, 2013
 *      Author: imizus
 */

#include "LexerException.h"

LexerException::LexerException(const string& message) : message(message)
{

}

LexerException::~LexerException() throw ()
{
}

