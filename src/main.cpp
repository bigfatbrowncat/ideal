//===-- examples/HowToUseJIT/HowToUseJIT.cpp - An example use of the JIT --===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
//  This small program provides an example of how to quickly build a small
//  module with two functions and execute it with the JIT.
//
// Goal:
//  The goal of this snippet is to create in the memory
//  the LLVM module consisting of two functions as follow:
//
// int add1(int x) {
//   return x+1;
// }
//
// int foo() {
//   return add1(10);
// }
//
// then compile the module via JIT, then execute the `foo'
// function and return result to a driver, i.e. to a "host program".
//
// Some remarks and questions:
//
// - could we invoke some code using noname functions too?
//   e.g. evaluate "foo()+foo()" without fears to introduce
//   conflict of temporary function name with some real
//   existing function name?
//
//===----------------------------------------------------------------------===//

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

#include "FormulaParser.h"

using namespace llvm;

int main()
{
/*	LexerTree lt("1+2+3");
	ParserTree<double> pt(lt, DoubleParser());
	pt.execute()*/


  InitializeNativeTarget();

  LLVMContext Context;

  Type* doubleType = Type::getDoubleTy(getGlobalContext());

  // Create some module to put our function into it.
  Module *M = new Module("test", Context);

  Function *FooF =
    cast<Function>(M->getOrInsertFunction("foo", doubleType,
                                          (Type *)0));

  BasicBlock *BB = BasicBlock::Create(Context, "foo_entry_block", FooF);

  IRBuilder<> builder(BB);

  	// Adding a variable
  	AllocaInst* inst = builder.CreateAlloca(doubleType, 0, "x");
  	Variables vars;
  	vars.add("x", inst);

  	// Lexing the formula
  	LexerTree lt("5* -    x+4");
	FormulaParser formulaParser(lt.getRoot(), OperatorPriorities(), vars);
	FormulaItem* fi = formulaParser.getRoot();

	// ** Generating the code **

	// Setting the value for x
	Value* xValue = ConstantFP::get(doubleType, 10.0);
	builder.CreateStore(xValue, vars.get("x"));

	// Generating our formula
	Value* formulaVal = fi->generateLLVMCode(builder);

  // Returning our formula result
  builder.CreateRet(formulaVal);

  // Now we create the JIT.
  ExecutionEngine* EE = EngineBuilder(M).create();

  outs() << "We just constructed this LLVM module:\n\n" << *M;
  outs() << "\n\nRunning foo: ";
  outs().flush();

  // Call the `foo' function with no arguments:
  std::vector<GenericValue> noargs;
  GenericValue gv = EE->runFunction(FooF, noargs);

  // Import result of execution:
  outs() << "Result: " << gv.DoubleVal << "\n";
  EE->freeMachineCodeForFunction(FooF);
  delete EE;
  llvm_shutdown();
  return 0;
}
