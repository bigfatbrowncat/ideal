Ideal
=====

## Building

This project should be built with Eclipse CDT.
It requires llvm 3.5.0, so in order to build it you should:

1. Git clone it to some folder
2. get LLVM 3.5.0 and put it to a folder named llvm-3.5.0.src _near_ the "ideal" folder you've just cloned
3. Build llvm 3.5.0 with the default `Release+Asserts` target (you may use CMake or just run `./configure && make` without options)
4. Import the "ideal" folder as a CDT project into Eclipse IDE.
5. Check if the virtual llvm folder inside the "ideal" project points to the source llvm-3.5.0.src folder. There should be `llvm/include` and `llvm/Release+Asserts/lib`.
6. Build it in the Eclipse IDE.

## Using

Usage: `ideal <expression>`
   
Expression will be translated into LLVM code, JIT-ed and executed. You can enter some expressions
divided with semicolon. Expressions may contain c-style variable declarations (only double type).

For example,

```
./ideal "double y=1;double xx=y+1; return xx"
``` 
will produce output

```
We just constructed this LLVM module:

; ModuleID = 'Main'
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

define double @main() {
main_entry_block:
  %y = alloca double
  store double 1.000000e+00, double* %y
  %y1 = load double* %y
  %addop = fadd double %y1, 1.000000e+00
  %xx = alloca double
  store double %addop, double* %xx
  %xx2 = load double* %xx
  ret double %xx2
}


Running the main function: Result: 2.000000e+00
```