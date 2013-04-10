/*
 * ParserOperatorPriorities.h
 *
 *  Created on: Apr 10, 2013
 *      Author: imizus
 */

#ifndef PARSEROPERATORPRIORITIES_H_
#define PARSEROPERATORPRIORITIES_H_

#include <map>

#include "ParserOperator.h"

using namespace std;

class ParserOperatorPriorities
{
private:
	map<ParserOperator, int> priorities;
public:
	ParserOperatorPriorities()
	{
		priorities.insert(pair<ParserOperator, int>(foNegate, 4));
		priorities.insert(pair<ParserOperator, int>(foMultiply, 3));
		priorities.insert(pair<ParserOperator, int>(foDivide, 3));
		priorities.insert(pair<ParserOperator, int>(foAdd, 2));
		priorities.insert(pair<ParserOperator, int>(foSubtract, 2));
		priorities.insert(pair<ParserOperator, int>(foEquate, 1));
	}

	int priorityOf(ParserOperator oper) const
	{
		return priorities.at(oper);
	}
};

#endif /* PARSEROPERATORPRIORITIES_H_ */
