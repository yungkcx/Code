# Introduction

After read the book Code:The Hidden Language of Computer Hardware and Software, I want to simulate computer's hardwares using C language, such as Half Adder, Adder, Selector, RAM.

At last, I have used these elements to make a "calculator".

## Instructions

All numbers are 8bits.

LOAD   Load a number from RAM.
STORE  Store a number to RAM.
ADD
SUB
AWC    Add with carry.
SWB    Subtract with borrow.
HALT

## Usage

```
$ cd translator && make
$ ./translator samples/addWithCarry.s -o ../add.b
$ cd ..
$ make && ./calculator add.b
```