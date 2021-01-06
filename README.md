# Advent Of Code 2016

A bunch of solutions for the [Advent of Code 2016](http://adventofcode.com/)

Written in C# using [LINQPad](http://www.linqpad.net/)

**Edit January 2021**

Added some more solutions, this time in C++ as a learning experience. Each day is a standalone program which can be compiled with, probably, any standard C++ compiler. Originally was going to be targetting C++11 'cos that's the book I'm reading but it turns out that the MSVC compiler that comes with VS2019 only supports C++14 and up. So I guess that's what we'll go with.

Most are written using Visual Studio Code with the Microsoft C/C++ extension package installed. Debugging within VS Code works fine for MSVC++ on Windows and g++ on Linux/WSL. Haven't yet bothered to set up any debugging options for Clang in `launch.json` and `tasks.json`.

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

Haven't tried with GNU/MSYS/MinGW g++ on Windows yet.

#### Clang on Windows
The `clang` compiler used here is the one that comes with Visual Studio 2019 (currently clang v10.0). If you try and compile using `-std=c++11` some of the projects fail but if you do the same on Linux/WSL then they build fine. Compiling on Windows without the `-std` flag or with `-std=c++14` (or higher) works fine. I guess it's to do with C++ extensions or the standard library and the use of `auto` etc but I haven't dug far enough to find out what. Doesn't really matter. It's old. Move on.

 ### Linux or WSL2
 With appropriate C++ compiler and build essential tools installed. eg:

 ```
    $> clang -o dayNN dayNN.cpp
 ```
or, with optimisations and other bits, perhaps
```
    $> clang -O3 -std=c++17 -o dayNN dayNN.cpp
```
or maybe you prefer GNU
```
    $> g++ -o dayNN dayNN.cpp
```
 etc, etc

#### NB: Day 14
Day 14 needs to be compiled with an MD5 implementation. Hence the build command needs to be adjusted. eg:
```
    $> cl /EHsc /Fe: day14.exe day14.cpp md5.cpp
```
and similar adjustments for the other compilers.