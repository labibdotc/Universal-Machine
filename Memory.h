#ifndef MemoryMod
#define MemoryMod
#include <stdio.h>
#include <stdlib.h>
#include "seq.h"
#include <stdint.h>
#include "uarray.h"
#include "memoryRepresentation.h"


/*creates new memory instance, with InstructionArray as first thing*/
Memory_T Memory_new(UArray_T InstructionArray);

/*deletes memory struct stuff*/
void Memory_free(Memory_T currMemory);

/*returns new ID, parameters are the memory your accessing, and 
how big you want it to be*/
uint32_t Memory_map_segment(Memory_T currMemory, int UArraySize);

/*unmaps a segment, using an ID*/
void Memory_unmap_segment(Memory_T currMemory, int ID);


/*gets a specific word,  at a specific index of the sequence, at a 
specific index of that uarray
*/
uint32_t Memory_get_Specific_word(Memory_T currMemory, int SeqIndex, 
                                    int UArrayIndex);

/*puts a specific word,  at a specific index of the sequence, at a specific 
index of that uarray*/
void Memory_put_Specific_word(Memory_T currMemory, int SeqIndex, 
                                int UArrayIndex, uint32_t word);

/*Purpose: checks opcode and call relevant function*/
void run(UArray_T InstructionArray); 


#endif