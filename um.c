/*
 * um.c
 *
 * This is the main file for the um homework.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "IO.h"
#include "Memory.h"
#include "Actions.h"
int main(int argc, char **argv) {

    if(argc != 2) {
        fprintf(stderr, "Usage: ./um [filename]\n");
    } else {
        UArray_T segmentZero = setInstructions(argv[1]);
        Memory_T currMemory = Memory_new(segmentZero);
  
        uint32_t counter = 0;
        uint32_t currentWord = 0;

        uint32_t *registers = malloc(8*(sizeof(uint32_t)));
        for(int i = 0; i < 8; i++) {
            registers[i] = 0;
        }
        while((int)counter < UArray_length(Seq_get(currMemory->memory, 0)) ) {
            currentWord = *(uint32_t*)UArray_at(Seq_get(currMemory->memory, 0),
                 counter);

            doCommand(currentWord, currMemory, &registers, &counter);
        }
        free(registers);
        Memory_free(currMemory);
    }
    


    return 0;
}