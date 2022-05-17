/**************************************************************
*
*                     Memory.c
*
*     Assignment: um
*     Authors: Caleb Pekowsky, Labib Afia
*     Date:     04//2022
*
*     summary:
*
*     this is the .c file for our memory implementation. It holds the 
    implementation
*     for the memory, and the functions to interact with it.
*
**************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "Memory.h"
#include "uarray.h"
#include <assert.h>



/*  Memory_new
*  Purpose: initialize memory on heap with instructions at segment 0
*  Parameters: UArray_T InstructionArray uarray
*  Return: struct of memory
*/
Memory_T Memory_new(UArray_T InstructionArray) {
    static int numSeq = 10;
    Memory_T currMemory = malloc(sizeof(struct Memory_T));
    currMemory -> memory = Seq_new(numSeq);
    currMemory -> unused_memory = Seq_new(0);
    Seq_addlo(currMemory -> memory, InstructionArray);
    return currMemory;
}


/*  Memory_map_segment
*  Purpose: Maps a new segment with a given size of words in memory
*  Parameters:  Memory_T currMemory: struct of memory,
*              int UArraySize: how many words
*  Return: a uint32_t id which is where the algorithm chose to map the segment
*/
uint32_t Memory_map_segment(Memory_T currMemory, int UArraySize ) {
    int numSegment = UArraySize;
    UArray_T new_segment = UArray_new(numSegment, sizeof(uint32_t));

    for(int i = 0; i< UArray_length(new_segment); i++) {
        *(uint32_t*)UArray_at(new_segment, i) = 0;
    }

    int id;
    if(Seq_length(currMemory -> unused_memory) > 0) {
        //stack implementation: last unmapped first recycled algorithm
        int * reused_id = (int*)Seq_remhi(currMemory -> unused_memory);
        id = *reused_id;
        free(reused_id);
        Seq_put(currMemory->memory, id, new_segment);
    } else {
        id = Seq_length(currMemory->memory);
        Seq_addhi(currMemory->memory, new_segment);
    }
    return id;
}


//


/* Memory_free
*  Purpose: frees allocated heap in hardware memory that has our 
*           memory-representing struct.
*  Parameters: Memory_T currMemory: struct of memory
*
* 
*  Return: none
*/
void Memory_free(Memory_T currMemory) {
    int memoryLength = Seq_length(currMemory->memory);
    for(int i = 0; i < memoryLength; i++) {
        UArray_T currArray = Seq_get(currMemory->memory, i);
        if(currArray != NULL) {
            UArray_free(&currArray);
        }
    }
    Seq_free(&(currMemory->memory));
    int unusedLength = Seq_length(currMemory->unused_memory);
    for(int i = 0; i < unusedLength; i++) {
        free((int*)Seq_remhi(currMemory->unused_memory));
    }
    Seq_free(&(currMemory->unused_memory));
    free(currMemory);
}



/* Memory_get_Specific_word
*  Purpose: gets specific word from memory at some offset in a specific segment
*  Parameters: Memory_T currMemory: memory struct
               int SeqIndex:        index of segment in memory
               int UArrayIndex:     index of word in segment
*
*  Return: word retrieved from memory to be stored onto a register
*/
uint32_t Memory_get_Specific_word(Memory_T currMemory, int SeqIndex, 
                            int UArrayIndex) {
    int SequenceIndex = SeqIndex;

    assert(SequenceIndex < Seq_length(currMemory -> memory));

    UArray_T currArray= Seq_get(currMemory -> memory, SequenceIndex);
    assert(UArrayIndex < UArray_length(currArray));

    return *(uint32_t *)UArray_at(currArray, UArrayIndex);

}


/* Memory_put_Specific_word
*  Purpose: puts a word at a specific index of a specific uarray in the 
*   sequence
*  Parameters:
*  Memory_T currMemory: the memory struct we want to manipulate
*  int SeqIndex: the index of the uarray in the sequence we want to change
*  int UArrayIndex: the index of the word in the uarray we want to add
*  uint32_t word: the word we want to add
*  Return: nothing.
*/
void Memory_put_Specific_word(Memory_T currMemory, int SeqIndex, 
                            int UArrayIndex, uint32_t word) {
    int SequenceIndex = SeqIndex;
    assert(SequenceIndex < Seq_length(currMemory -> memory) );

    UArray_T currArray= Seq_get(currMemory -> memory, SequenceIndex);
    assert(UArrayIndex < UArray_length(currArray));

    *(uint32_t *)UArray_at(currArray, UArrayIndex) = word;
}


/* Memory_unmap_segment
*  Purpose: unmaps a sequence, aka either sets it to null or deletes it,
*  depending on it's location in the Sequence
*  Parameters:
*  Memory_T currMemory: the memory struct we want to manipulate
*  int id: the id of the uarray in the memory we want to unmap
*  Return: nothing.
*/
void Memory_unmap_segment(Memory_T currMemory, int id) {
    assert(id < Seq_length(currMemory->memory));
    assert(Seq_get(currMemory->memory,id) != NULL);
    UArray_T currArray = Seq_get(currMemory->memory, id);
    UArray_free(&currArray);

    int* newID = malloc(sizeof(int));
    *newID = id;
    Seq_put(currMemory->memory, *newID, NULL);
    Seq_addhi(currMemory->unused_memory, newID);
}
