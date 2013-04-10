#include "llvm/Constants.h"
#include "llvm/DerivedTypes.h"
#include "llvm/IRBuilder.h"
#include "llvm/Instructions.h"
#include "llvm/LLVMContext.h"
#include "llvm/Module.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/raw_ostream.h"

#include "lexer/LexerException.h"

#include "parser/ExpressionParser.h"
#include "parser/ParserException.h"

#include <stdio.h>
#include <string>

using namespace std;
using namespace llvm;

int main(int argc, char** argv)
{
	string expression;

	if (argc != 2)
	{
		printf("Usage:\n   ideal <expression>");
		return -1;
	}
	else
	{
		expression = string(argv[1]);
	}

  	ParserVariables vars;
  	vars.define("x");

  	ParserNode* formulaRoot;
  	try
  	{
		// Parsing the formula
		LexerTree formulaLexerTree(expression);		//"5* (-    x+((-(4))))"

		ParserOperatorPriorities pop;
		ExpressionParser formulaParser(pop);

		formulaRoot = formulaParser.parse(formulaLexerTree.getRoot(), vars);
  	}
  	catch (const LexerException& lexerException)
  	{
  		printf("Lexer is asking for excuse. %s", lexerException.getMessage().c_str());
  		return -1;
  	}
  	catch (const ParserException& parserException)
  	{
  		printf("Parser is asking for excuse. %s", parserException.getMessage().c_str());
  		return -1;
  	}

	// ** Generating the code **

	InitializeNativeTarget();
	LLVMContext Context;
	Type* doubleType = Type::getDoubleTy(getGlobalContext());

	// Creating the main module
	Module *mainModule = new Module("Main", Context);

	// Creating the main function
	Function *mainFunction = cast<Function>(mainModule->getOrInsertFunction("main", doubleType, (Type *)0));

	BasicBlock *mainFunctionEntryBlock = BasicBlock::Create(Context, "main_entry_block", mainFunction);
	IRBuilder<> builder(mainFunctionEntryBlock);

	vars.generateVariableCreationLLVMCode("x", builder);
	vars.generateLLVMVariableSetToConstantCode("x", 12.3, builder);

	// Generating our formula
	Value* formulaVal = formulaRoot->generateGetValueLLVMCode(builder);

	// Returning our formula result
	builder.CreateRet(formulaVal);

	// Now we create the JIT.
	ExecutionEngine* executionEngine = EngineBuilder(mainModule).create();

	outs() << "We just constructed this LLVM module:\n\n" << *mainModule;
	outs() << "\n\nRunning foo: ";
	outs().flush();

	// Call the `foo' function with no arguments:
	std::vector<GenericValue> noargs;

	GenericValue gv = executionEngine->runFunction(mainFunction, noargs);

	// Import result of execution:
	outs() << "Result: " << gv.DoubleVal << "\n";
	executionEngine->freeMachineCodeForFunction(mainFunction);

	delete executionEngine;
	llvm_shutdown();
	return 0;
}
