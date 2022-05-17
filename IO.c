#include <stdio.h>
#include <stdlib.h>
#include "IO.h"
#include <assert.h>
#include <sys/stat.h>
#include <bitpack.h>
#include "uarray.h"
#include <stdint.h>
#include "Memory.h"

/* setInstructions
*  Purpose: opens um file and read in 32-bit words onto a uarray of words
*           it verifies that file has a group of complete 32 bit words 
*  Parameters:
    char* filename: filename
*  Return: UArray_T which has all the words stored
*/
UArray_T setInstructions(char* filename) {
    struct stat sb;
    long long file_size;
    stat(filename, &sb);

    if (stat(filename, &sb) == -1) {
        perror("stat");
    } else {
        file_size = (long long) sb.st_size;
    }
    
    FILE *input = fopen(filename, "rb");
    assert(input != NULL);
    UArray_T segment = UArray_new(file_size / 4, sizeof(uint32_t));

    for(int i = 0; i < file_size; i+=4) {
        
        uint32_t currword = 0;
        unsigned currchar;
        /* getting the current code word, by using getc 4x, because char is 
        8 bytes. */
        for(int x = 3; x >=0 ; x-- ) {
            currchar = getc(input);
            currword = Bitpack_newu(currword, 8, 0+(8*x), currchar);
            if(ferror(input) != 0 || (int)currchar == EOF) {
                fprintf(stderr, "A word is incomplete\n");
                exit(1);
            }
        }
        *( uint32_t*)(UArray_at(segment, i/4)) = currword;
    }

    fclose(input);

    return segment;
}