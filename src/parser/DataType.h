/*
 * FormulaParser.h
 *
 *  Created on: 06.04.2013
 *      Author: il
 */

#ifndef DATATYPE_H_
#define DATATYPE_H_

#include <llvm/IR/Type.h>

using namespace llvm;

enum DataType { tInvalid, tDouble, tBoolean };

static Type* variableTypeToLLVMType(DataType vt, IRBuilder<>& builder) {
	switch (vt)
	{
	case tDouble:
		return builder.getDoubleTy();
	case tBoolean:
		return builder.getInt1Ty();
	default:
		throw ParserException("Invalid type");
	}
}

#endif
