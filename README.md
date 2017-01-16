# Simplz
A very simple 8 bit CPU built in TTL.

(based on the MCPU -built in a CPLD: https://github.com/cpldcpu/MCPU)

Currently the CPU has currently been built on a stripboard through wirewrap. A working memory and IO board is in progress.
The CPU also has a simulation built in logism, and a working assembler written in C.

## Instruction set

The CPU is accumulator based, supports 4 different instructions and has a 6bit address space.
Instructions are encoded with:
``` 
iiaa aaaa
```

aaaaaa contains a 6bit memory address
The valid instructions are as follows:

ii | Instruction| Notes
---|------------|-------
00 | ADD [addr] | Adds the accumulator with the value stored at the memory address
01 | NOR [addr] | Nors the accumulator with the value stored at the memory address
10 | STR [addr] | Stores the accumulator to the memory address
11 | JPC [addr] | If the carry bit is clear, jump to addr, otherwise, clear the carry bit

With these four instructions all common operations can be performed.

For instance - to compare the accumulator with zero, and jump if so, the following may be used:
```
  ADD @_ff
  JPC @is_zero ; If you add 0xff to a number, the carry bit is only clear if the number is zero.
  ...
  
:_ff
  LIT $ff
```
