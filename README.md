# Advent Of Code 2016

A bunch of solutions for the [Advent of Code 2016](http://adventofcode.com/)

First 10 days written in C# using [LINQPad](http://www.linqpad.net/)

**Edit January 2021**

Added some more solutions, this time in C++ as a learning experience. Each day is a standalone program which can be compiled with, probably, any modern, standard C++ compiler.

Mostly targetting C++ 14 (and higher) although most should work when targetting C++11 too. The compilers installed with the current version of Visual Studio 2019 default to C++14, I believe. Don't think you can specifically target C++11 in the 2019 MSVC++ compiler.

Most are written using Visual Studio Code with the Microsoft C/C++ extension package installed. Debugging within VS Code works fine for MSVC++ on Windows and g++ on Linux/WSL. Other combinations (eg g++/Win, clang/Linux etc) may or may not work well for debugging. See `launch.json` and `tasks.json` files and the [VS Code page for C++](https://code.visualstudio.com/docs/cpp).

### Windows
With Visual Studio installed, or appropriate C++ build/compile tools (where NN is the required day, eg day12.cpp / day12.exe):

```
    $> cl /EHsc /Fe: dayNN.exe dayNN.cpp
```
with debug info
```
    $> cl /Zi /EHsc /Fe: dayNN.exe dayNN.cpp
```
and with optimisations
```
    $> cl /EHsc /O2 /Fe: dayNN.exe dayNN.cpp
```
or with Clang:
```
     $> clang -o dayNN.exe dayNN.cpp
```
or GNU/gcc (using MinGW from https://nuwen.net/mingw.html):
```
     $> g++ -o dayNN.exe dayNN.cpp
     or
     $> g++ -O -o dayNN.exe dayNN.cpp
```
etc

 Make sure that the compiler and necessary build tools are in the current path. The best way to do this is to run the builds from the Visual Studio command prompt.

#### Clang on Windows
The `clang` compiler used here is the one that comes with Visual Studio 2019 (currently clang v10.0). If you try and compile using `-std=c++11` some (most?) of the projects fail but if you do the same on Linux/WSL then they build fine. Compiling on Windows without the `-std` flag or with `-std=c++14` (or higher) works fine. I guess it's to do with C++ extensions or the standard library and the use of `auto` etc but I haven't dug far enough to find out what. `g++` seems to work fine with the C++11 flag on Windows. Ho well, doesn't really matter. Move on.

#### GNU/g++ on Windows
A suitable toolchain for building C++ with GNU/g++ on Windows can be found at https://nuwen.net/mingw.html or https://www.msys2.org/. Follow the relevant instructions to install it all. Again, building is best done from the appropriate command prompt so that all necessary tools/compilers etc are in the expected places and on the Windows system path.

 ### Linux or WSL2
 With appropriate C++ compiler and build essential tools installed. eg:

 ```
    $> clang -o dayNN dayNN.cpp
 ```
or, with optimisations and other bits, perhaps
```
    $> clang -O3 -std=c++17 -o dayNN dayNN.cpp
```
GNU/g++:
```
    $> g++ -o dayNN dayNN.cpp
```
 etc, etc

#### NB: Days 14 & 17
Days 14 and 17 need to be compiled with an MD5 implementation. Hence the build command needs to be adjusted. eg:
```
    $> cl /EHsc /Fe: day14.exe day14.cpp md5.cpp
```
and similar adjustments for the other compilers.