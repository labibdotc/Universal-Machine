#include <stdlib.h>
#include <stdio.h>
#include "Memory.h"


void runCommands(UArray_T InstructionArray) {

    uint32_t registers[8];

    for(int i = 0; i < 8; i++) {
        registers[i] = 0;
    }

    Memory_T memory = Memory_new(UArray_T InstructionArray);
    uint32_t counter = 0;
    while(counter != UArray_length(InstructionArray)-1) {
        //check word in the uarray and call action.h or memory.h
        uint32_t currWord = *(uint32_t*)UArray_at(InstructionArray, counter)
        //appropriately passing either registers or memory or both
        // counter++;
    }
}