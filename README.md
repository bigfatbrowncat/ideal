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

Usage: ideal <expression>
   
Expression will be translated into LLVM code, JIT-ed and executed. You can enter some expressions
divided with semicolon. Expressions may contain "x" variable which is initialized with 12.3.

For example

`./ideal "x = x + 2; x / 2"` will produce output

```
We just constructed this LLVM module:

; ModuleID = 'Main'
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"

define double @main() {
main_entry_block:
  %x = alloca double
  store double 1.230000e+01, double* %x
  %x1 = load double* %x
  %addop = fadd double %x1, 2.000000e+00
  store double %addop, double* %x
  %x2 = load double* %x
  %x3 = load double* %x
  %divop = fdiv double %x3, 2.000000e+00
  ret double %divop
}


Running the main function: Result: 7.150000e+00
```