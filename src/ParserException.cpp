/*
 * ParsingException.cpp
 *
 *  Created on: Apr 8, 2013
 *      Author: imizus
 */

#include "ParserException.h"

ParserException::ParserException(const string& message) : message(message)
{
}

ParserException::~ParserException() throw ()
{
}

