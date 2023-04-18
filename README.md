# LLVM IR ANALYSIS

LLVM provides passes to perform analysis or tansformation on LLVM IR. These passes can be used to improve the performance of the code or to analyze the code for bottlenecks.

There are three types of passes in LLVM:
- Analysis Passes
- Transformation Passes
- Utility Passes

This repository contains 2 examples of analysis passes:
- ListFunction: Lists all the functions along with their argument size, basic block count and instruction count.
- StaticCallCounter: Counts the number of static calls to a function.

> These passes are __out-of-tree__. Because adding any pass into LLVM requires building it again and again, which was not feasible for our case.

## Requirements

- LLVM 15.x.x
- clang

## Installation

1. Change the path of your LLVM installation directory in root CMakeLists.txt.
1. `mkdir build`
2. `cd build`
3. `cmake ../`
4. `cmake --build .`

<br></br>
## Usage

### List Function

`opt -load-pass-plugin ./lib/libListFunction.so -passes=list-function -disable-output <input ll file>`

### Static Call Counter

`opt -load-pass-plugin ./lib/libStaticCallCounter.so -passes=static-call-counter -disable-output <input ll file>`

## Screenshots

### List Function

![](https://i.ibb.co/5RM3yJx/Screenshot-from-2022-12-02-18-18-51.png)

### Static Call Counter

![](https://i.ibb.co/j8yb5FH/Screenshot-from-2022-12-02-18-14-01.png)
