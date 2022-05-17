/**************************************************************
*
*                     Actions.h
*
*     Assignment: um
*     Authors: Caleb Pekowsky, Labib Afia
*     Date:     04//2022
*
*     summary:
*
*     this is the .h file for our actions implementation. This stores the 
      implementation for the 14 commands/actions, and manipulates words to call 
      the correct action.
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Memory.h"

/*
Purpose: extract a,b and c from bits and load the three onto an int ptr
Parameters: word to do the extraction on, pointer of ints to load with A, 
b and c values
*/
void getABC(uint32_t word, int** ABC);

uint32_t getValue(uint32_t word);

void doCommand(uint32_t currentWord, Memory_T currMemory, uint32_t** registers, 
        uint32_t* counter);

void ConditionalMove(uint32_t** registers, int A, int B, int C);

void Add(uint32_t** registers, int A, int B, int C);

void Multiply(uint32_t** registers, int A, int B, int C);

void Divide(uint32_t** registers, int A, int B, int C);

void BitNAND(uint32_t** registers, int A, int B, int C);

void In(uint32_t** registers, int C);

void LoadProgram(Memory_T currMemory, uint32_t** registers, int B, 
    int C, uint32_t* counter);

void LoadValue(uint32_t** registers, int A, uint32_t value);