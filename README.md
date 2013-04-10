Ideal
=====

## Building

This project should be built with Eclipse CDT.
It requires llvm 3.2, so in order to build it you should:

1. Git clone it to some folder
2. get LLVM 3.2 and put it to a folder named llvm-3.2.src _near_ the "ideal" folder you've just cloned
3. Build llvm 3.2 in a folder llvm-3.2 near its source folder (you may use CMake or ./configure && make)
4. Import the "ideal" folder as a CDT project into Eclipse IDE.
5. Check if the virtual llvm-3.2 and llvm-3.2.src folders inside the "ideal" project points to the source llvm-3.2.src folder and the folder llvm-3.2, where you have just built LLVM 3.2 There should be `llvm-3.2/include`, `llvm-3.2.src/include` and `llvm-3.2/lib`.
6. Build it in the Eclipse IDE.

## Using

Usage: ideal <expression>
   
Expression will be translated into LLVM code, JIT-ed and executed.
For now expression can contain "x" variable which will be equal to 12.3 during the evaluation.