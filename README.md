# LLVM IR ANALYSIS

LLVM provides passes to perform analsis or tansformation on LLVM IR. These passes can be used to improve the performance of the code or to analyze the code for bottlenecks.

There are three types of passes in LLVM:
- Analysis Passes
- Transformation Passes
- Utility Passes

This repository contains 2 examples of analysis passes:
- ListFunction: Lists all the functions along with their argument size, basic block count and instruction count.
- StaticCallCounter: Counts the number of static calls to a function.

> These passes are __out-of-tree__, and are used using Pass Plugin.

## Requirements

- LLVM 15.x.x
- clang

## Installation

1. Change the path of your LLVM installation directory in root CMakeLists.txt.
1. `mkdir build`
2. `cd build`
3. `cmake ../`
4. `cmake --build .`

## Usage

### List Function

`opt -load-pass-plugin ./build/lib/libStaticCallCounter.so -passes=static-call-counter -disable-output <input ll file>`

### Static Call Counter

`opt -load-pass-plugin ./build/lib/libListFunction.so -passes=list-function -disable-output <input ll file>`
