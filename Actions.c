/**************************************************************
*
*                     Actions.c
*
*     Assignment: um
*     Authors: Caleb Pekowsky, Labib Afia
*     Date:     04//2022
*
*     summary:
*
*     this is the .c file for our actions implementation. This stores the 
      implementation for the 14 commands/actions, and manipulates words to call 
      the correct action.
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Actions.h"
#include "bitpack.h"
#include "Memory.h"
#include "assert.h"

//define 2^32 here

typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;

/* getABC
*  Purpose: get bits from 32-bit word that correspond to 3-bit A, B and C 
*           respectively
*  Parameters:
*  int**ABC: pointer to array that will store A, then B, then C
*  uint32_t word: the word we want to get from
*  Return: nothing.
*/
void getABC(uint32_t word, int** ABC) {
    (*ABC)[0] = Bitpack_getu(word, 3, 6);
    (*ABC)[1] = Bitpack_getu(word, 3, 3);
    (*ABC)[2] = Bitpack_getu(word, 3, 0);
}

/* getValue
*  Purpose: gets 25-bit value from a 32-bit word
*  Parameters:
*  uint32_t word: the word we want to get value from
*  Return: nothing.
*/
uint32_t getValue(uint32_t word) {
    //printf("currently loading value: %lu \n", Bitpack_getu(word, 25, 0));
    return Bitpack_getu(word, 25, 0);
}


/* doCommand
*  Purpose: executes appropriate action encoded by word using opcode
*  Parameters:
*  uint32_t currentWord: the word we want to execute
*  Memory_T currMemory: the memory struct we want to manipulate
*  uint32_t* counter:   pointer to counter
*  uint32_t** registers: pointer to array of 8 registers
*  Return: nothing.
*/
void doCommand(uint32_t currentWord, Memory_T currMemory,
                    uint32_t** registers, uint32_t* counter) {

    int opcode = currentWord >> 28;
    int *ABC = malloc(3*sizeof(*ABC));
    getABC(currentWord, &ABC);
    int A = ABC[0];
    int B = ABC[1];
    int C = ABC[2];

    if(opcode == LV) {
        A = Bitpack_getu(currentWord, 3, 25);
    }
    UArray_T segmentZero = Seq_get((currMemory->memory), 0);

    switch(opcode) {

      case CMOV :
        ConditionalMove(registers, A, B, C);
        break;
      case SLOAD :
        (*registers)[A] = Memory_get_Specific_word(currMemory, (*registers)[B],
                                                     (*registers)[C]);
        break;
      case SSTORE :
        Memory_put_Specific_word(currMemory, (*registers)[A], (*registers)[B],
                                                 (*registers)[C]);
        break;

      case ADD :
        Add(registers, A, B, C);
        break;
      case MUL :
        Multiply(registers, A, B, C);
        break;

      case DIV :
        Divide(registers, A, B, C);
        break;

      case NAND :
        BitNAND(registers, A, B, C);
        break;

      case HALT :
        *counter = UArray_length(segmentZero);
        break;

      case ACTIVATE:
        (*registers)[B] = Memory_map_segment(currMemory, (*registers)[C] );
        break;

      case INACTIVATE:
        Memory_unmap_segment(currMemory, (*registers)[C]);
        break;

      case OUT:
        assert((*registers)[C] <= 255);
        putchar((*registers)[C]);
        break;

      case IN:
        In(registers, C);
        break;

      case LOADP:
        LoadProgram(currMemory, registers, B, C, counter);
        break;

      case LV:
        LoadValue(registers, A, getValue(currentWord));
        break;

       default:
        fprintf(stderr,"invalid opcode. Quitting. \n");
        *counter = UArray_length(segmentZero);

    }

    if (opcode != LOADP) {
        (*counter)++;
    }
    free(ABC);

}

/* ConditionalMove
*  Purpose: execute the cmov command
*  Parameters:
*  uint32_t** registers: pointer to array of 8 registers
*  int A: decimal value in 3 bits that represent A
*  int B: decimal value in 3 bits that represent B
*  int C: decimal value in 3 bits that represent C
*  Return: nothing.
*/
void ConditionalMove(uint32_t** registers, int A, int B, int C) {
    if( (*registers)[C] != 0) {
        (*registers)[A] = (*registers)[B];
    }
}


/* Add
*  Purpose: execute the add command
*  Parameters:
*  uint32_t** registers: pointer to array of 8 registers
*  int A: decimal value in 3 bits that represent A
*  int B: decimal value in 3 bits that represent B
*  int C: decimal value in 3 bits that represent C
*  Return: nothing.
*/
void Add(uint32_t** registers, int A, int B, int C) {
    (*registers)[A] = ((*registers)[B] + (*registers)[C] ) % 4294967296;
}


/* Multiply
*  Purpose: execute the multiply command
*  Parameters:
*  uint32_t** registers: pointer to array of 8 registers
*  int A: decimal value in 3 bits that represent A
*  int B: decimal value in 3 bits that represent B
*  int C: decimal value in 3 bits that represent C
*  Return: nothing.
*/
void Multiply(uint32_t** registers, int A, int B, int C) {
    (*registers)[A] = ((*registers)[B] * (*registers)[C] ) % 4294967296;
}

/* Divide
*  Purpose: execute the divide command
*  Parameters:
*  uint32_t** registers: pointer to array of 8 registers
*  int A: decimal value in 3 bits that represent A
*  int B: decimal value in 3 bits that represent B
*  int C: decimal value in 3 bits that represent C
*  Return: nothing.
*/
void Divide(uint32_t** registers, int A, int B, int C) {
    (*registers)[A] = ((*registers)[B] / (*registers)[C] ) % 4294967296;
}


/* BitNAND
*  Purpose: execute the bitnand command
*  Parameters:
*  uint32_t** registers: pointer to array of 8 registers
*  int A: decimal value in 3 bits that represent A
*  int B: decimal value in 3 bits that represent B
*  int C: decimal value in 3 bits that represent C
*  Return: nothing.
*/
void BitNAND(uint32_t** registers, int A, int B, int C) {
    (*registers)[A] = ~( (*registers)[B] & (*registers)[C] );
}


/* In
*  Purpose: execute the input command
*  Parameters:
*  uint32_t** registers: pointer to array of 8 registers
*  int C: decimal value in 3 bits that represent C
*  Return: nothing.
*/
void In(uint32_t** registers, int C) {
    int currInput = getc(stdin);

    if(currInput == EOF) {
        (*registers)[C] = ~(uint32_t)0;
    } else {
        assert(0 <= currInput && currInput <= 255);
        (*registers)[C] = currInput;
    }

}

/* LoadProgram
*  Purpose: execute the loadprogram command
*  Parameters:
*  Memory_T currMemory: the memory struct we want to manipulate
*  uint32_t** registers: pointer to array of 8 registers
*  int A: decimal value in 3 bits that represent A
*  int B: decimal value in 3 bits that represent B
*  int C: decimal value in 3 bits that represent C
*  uint32_t* counter:   pointer to counter
*  Return: nothing.
*/
void LoadProgram(Memory_T currMemory, uint32_t** registers, int B, int C, 
                                uint32_t* counter) {
    if( (*registers)[B] != 0) {
        UArray_T NewCommands =  Seq_get(currMemory->memory, (*registers)[B]);
        UArray_T NewSeqZero = UArray_copy(NewCommands, 
                                        UArray_length(NewCommands));
        UArray_T toReplace = Seq_get(currMemory->memory, 0);
        UArray_free(&toReplace);
        Seq_put(currMemory->memory, 0, NewSeqZero);
    }
    *counter = (*registers)[C];

}

/* LoadValue
*  Purpose: execute the loadvalue command
*  Parameters:
*  uint32_t** registers: pointer to array of 8 registers
*  int value: decimal value in 25 bits that represent value
*  Return: nothing.
*/
void LoadValue(uint32_t** registers, int A, uint32_t value) {
    (*registers)[A] = value;

}
