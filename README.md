Ideal
=====

## Building

This project should be built with Eclipse CDT.
It requires llvm 3.2, so in order to build it you should:

1. Git clone it to some folder
2. get LLVM 3.2 and put it to a folder named llvm-3.2 _near_ the "ideal" folder you've just cloned
3. Build llvm 3.2 in its source folder (you may use CMake or configure)
4. Import the "ideal" folder as a CDT project into Eclipse IDE.
5. Build it in the Eclipse IDE.