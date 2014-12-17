#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/ExecutionEngine/JIT.h"
#include "llvm/ExecutionEngine/Interpreter.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Support/ManagedStatic.h"
#include "llvm/Support/raw_ostream.h"

#include "lexer/LexerException.h"

#include "parser/Parser.h"
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
		printf("Usage:\n   ideal <expression>\n");
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
		Parser formulaParser(pop);

		formulaRoot = formulaParser.parseFlow(formulaLexerTree.getRoot().getInnerItems(), vars);
  	}
  	catch (const LexerException& lexerException)
  	{
  		printf("Lexer is asking for excuse. %s", lexerException.getMessage().c_str());
  		fflush(stdout);
  		return -1;
  	}
  	catch (const ParserException& parserException)
  	{
  		printf("Parser is asking for excuse. %s", parserException.getMessage().c_str());
  		fflush(stdout);
  		return -1;
  	}

	// ** Generating the code **

	InitializeNativeTarget();
	LLVMContext context;
	Type* doubleType = Type::getDoubleTy(context);

	// Creating the main module
	Module *mainModule = new Module("Main", context);

	// Creating the main function
	Function *mainFunction = cast<Function>(mainModule->getOrInsertFunction("main", doubleType, (Type *)0));

	BasicBlock *mainFunctionEntryBlock = BasicBlock::Create(context, "main_entry_block", mainFunction);
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
	outs() << "\n\nRunning the main function: ";
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
