/*
 * testing.c
 *
 * this contains the logic for the testing we did in the very 
 * beginning of this homework.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "IO.h"
#include "Memory.h"

int main(int argc, char **argv) {

    // assert(argc == 2);
    if(argc != 2) {
        fprintf(stderr, "Usage: ./um [filename]\n");
    } else {
         
        Memory_T currMemory = Memory_new(setInstructions(argv[1]));
        int UArraySize = 10; //TODO: make it retrieve registers
        uint32_t firstID = Memory_map_segment(currMemory, UArraySize); 
        printf("the first ID we get is %d \n", firstID);
        //TODO: access r[c] to pass it in firstID
        //Memory_unmap_segment(currMemory, firstID);

        uint32_t secondID = Memory_map_segment(currMemory, UArraySize);

        printf("the second ID we get is %d \n", secondID);
        Memory_put_Specific_word(currMemory, 1, 2, (uint32_t) 33);
        uint32_t gottenWord = Memory_get_Specific_word(currMemory, 1, 2);
        uint32_t gottenWord2 = Memory_get_Specific_word(currMemory, 1, 3);

        printf("the gotten word we get is %d \n", gottenWord);
        printf("the second gotten word we didn't write is %d \n", gottenWord2);

        //AKAdeconstructor();
        Memory_free(currMemory);
    }

    //getting instruction array using IO.h 
    
    //calling run, using IO.h
    


    return 0;
}