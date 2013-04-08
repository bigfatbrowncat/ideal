Ideal
=====

## Building

This project should be built with Eclipse CDT.
It requires llvm 3.2, so in order to build it you should:

1. Git clone it to some folder
2. get LLVM 3.2 and put it to a folder named llvm-3.2 _near_ the "ideal" folder you've just cloned
3. Build llvm 3.2 in its source folder (you may use CMake or configure)
4. Import the "ideal" folder as a CDT project into Eclipse IDE.
5. Check if the virtual llvm-3.2 folder inside the "ideal" project points to the folder where you have just build LLVM 3.2 There should be `llvm-3.2/include' and `llvm-3.2/lib'.
6. Build it in the Eclipse IDE.

## Using

Usage:
   ideal <expression>
   
Expression will be translated into LLVM code, JIT-ed and executed.
For now expression can contain "x" variable which will be equal to 12.3 during the evaluation.