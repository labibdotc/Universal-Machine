/*
 * umlab.c
 *
 * Functions to generate UM unit tests. Once complete, this module
 * should be augmented and then linked against umlabwrite.c to produce
 * a unit test writing program.
 *
 * A unit test is a stream of UM instructions, represented as a Hanson
 * Seq_T of 32-bit words adhering to the UM's instruction format.
 *
 * Any additional functions and unit tests written for the lab go
 * here.
 *
 */


#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <seq.h>
#include <bitpack.h>


typedef uint32_t Um_instruction;
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, ACTIVATE, INACTIVATE, OUT, IN, LOADP, LV
} Um_opcode;


/* Functions that return the two instruction types */



/*  three_register
*  Purpose: bitpacks ra, rb, rc and opcode to a word
*  Parameters:  int ra: what's in register[a]
*               int rb: what's in register[b]
*               int rc: what's in register[c]
*  Return: word as an uminstruction
*/
Um_instruction three_register(Um_opcode op, int ra, int rb, int rc) {
    //takes this info to bitpack all this info int

    Um_instruction word = 0;
    word = Bitpack_newu(word, 3, 0,rc);
    word = Bitpack_newu(word, 3, 3,rb);
    word = Bitpack_newu(word, 3, 6,ra);
    word = Bitpack_newu(word, 4, 28,op);
    return word;
}



/*  three_register
*  Purpose: bitpacks ra, and value into a word and opcode of load val
*  Parameters:  unsigned ra: what's in register[a]
                unsigned val: what's to be loaded
*  Return: word as an uminstruction
*/
Um_instruction loadval(unsigned ra, unsigned val) {
    Um_instruction word = 0;
    word = Bitpack_newu(word, 25, 0,val);
    word = Bitpack_newu(word, 3, 25,ra);
    word = Bitpack_newu(word, 4, 28,13);
    return word;
}


/* Wrapper functions for each of the instructions */


/*  halt
*  Purpose: creates word with a halt instruction 
*  Parameters:
*  Return: word as an uminstruction
*/
static inline Um_instruction halt(void)
{
        return three_register(HALT, 0, 0, 0);
}

typedef enum Um_register { r0 = 0, r1, r2, r3, r4, r5, r6, r7 } Um_register;

/*  add
*  Purpose: creates word with an add instruction 
*  Parameters: all the three registers a, b and c
*  Return: word as an uminstruction
*/
static inline Um_instruction add(Um_register a, Um_register b, Um_register c)
{
        return three_register(ADD, a, b, c);
}

/*  multiply
*  Purpose: creates word with a multiply instruction 
*  Parameters: all the three registers a, b and c
*  Return: word as an uminstruction
*/
static inline Um_instruction multiply(Um_register a, Um_register b, 
                        Um_register c)
{
        return three_register(MUL, a, b, c);
}

/*  divide
*  Purpose: creates word with a divide instruction 
*  Parameters: all the three registers a, b and c
*  Return: word as an uminstruction
*/
static inline Um_instruction divide(Um_register a, Um_register b, 
                                    Um_register c)
{
        return three_register(DIV, a, b, c);
}


/*  nand
*  Purpose: creates word with a nand instruction 
*  Parameters: all the three registers a, b and c
*  Return: word as an uminstruction
*/
static inline Um_instruction nand(Um_register a, Um_register b, 
                                Um_register c)
{
        return three_register(NAND, a, b, c);
}


/*  segmentload
*  Purpose: creates words with a segmentload instruction 
*  Parameters: all the three registers a, b and c
*  Return: word as an uminstruction
*/
static inline Um_instruction segmentload(Um_register a, Um_register b, 
            Um_register c)
{
        return three_register(SLOAD, a, b, c);
}


/*  segmentstore
*  Purpose: creates words with a segmentstore instruction 
*  Parameters: all the three registers a, b and c
*  Return: word as an uminstruction
*/
static inline Um_instruction segmentstore(Um_register a, Um_register b, 
                                        Um_register c)
{
        return three_register(SSTORE, a, b, c);
}


/*  memorymap
*  Purpose: creates word with a memorymap instruction 
*  Parameters: all the three registers a, b and c
*  Return: word as an uminstruction
*/
static inline Um_instruction memorymap(Um_register a, Um_register b, 
                                        Um_register c)
{
        return three_register(ACTIVATE, a, b, c);
}


/*  memoryunmap
*  Purpose: creates word with a memory unmap instruction 
*  Parameters: what's on register c
*  Return: word as an uminstruction
*/
static inline Um_instruction memoryunmap(Um_register c)
{
        return three_register(INACTIVATE, 0, 0, c);
}


/*  conditionalmove
*  Purpose: creates word with a conditional move instruction 
*  Parameters: all the three registers a, b and c
*  Return: word as an uminstruction
*/
static inline Um_instruction conditionalmove(Um_register a, Um_register b, 
                    Um_register c)
{
        return three_register(CMOV, a, b, c);
}

/*  loadprogram
*  Purpose: creates word with a loadprogram instruction 
*  Parameters: all the three registers a, b and c
*  Return: word as an uminstruction
*/
static inline Um_instruction loadProgram(Um_register a, Um_register b,
                                 Um_register c)
{
        (void) a;
        return three_register(LOADP, 0, b, c);
}

/*  output
*  Purpose: creates word with a output instruction 
*  Parameters: what's on register c
*  Return: word as an uminstruction
*/
Um_instruction output(Um_register c) {
        return three_register(OUT, 0, 0, c);
}

/*  input
*  Purpose: creates word with a input instruction 
*  Parameters: what's on register c
*  Return: word as an uminstruction
*/
Um_instruction input(Um_register c) {
        return three_register(IN, 0, 0, c);
}

/* Functions for working with streams */

/*  append
*  Purpose: push to the sequence representing the stream a word i.e. instruction
*  Parameters: instruction in 32 bits and stream as a sequence
*  Return: none
*/
static inline void append(Seq_T stream, Um_instruction inst)
{
        assert(sizeof(inst) <= sizeof(uintptr_t));
        Seq_addhi(stream, (void *)(uintptr_t)inst);
}

const uint32_t Um_word_width = 32;

/*  Um_write_sequence
*  Purpose: push to the sequence representing the stream a word i.e. instruction
*  Parameters: sequence of words to go on the output stream, 
                    output file pointer
*  Return: none
*/
void Um_write_sequence(FILE *output, Seq_T stream)
{
        assert(output != NULL && stream != NULL);
        int stream_length = Seq_length(stream);
        for (int i = 0; i < stream_length; i++) {
                Um_instruction inst = (uintptr_t)Seq_remlo(stream);
                for (int lsb = Um_word_width - 8; lsb >= 0; lsb -= 8) {
                        fputc(Bitpack_getu(inst, 8, lsb), output);
                }
        }

}


/* Unit tests for the UM */

/*  build_halt_test
*  Purpose: should halt
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_halt_test(Seq_T stream)
{
        append(stream, halt());
}


/*  build_output_test
*  Purpose: should ouput 0 and then halt
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_output_test(Seq_T stream)
{
        append(stream, output(r1));
        append(stream, halt());
}

/*  build_loadval_test
*  Purpose: should test loadval and put chars onto stdout printing good\n
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_loadval_test(Seq_T stream)
{
        append(stream, loadval(r1, 'G'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'o'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'o'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
        append(stream, halt());
}




/*  build_verbose_halt_test
*  Purpose: should halt and not excute anything after
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_verbose_halt_test(Seq_T stream)
{
        append(stream, halt());
        append(stream, loadval(r1, 'B'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'a'));
        append(stream, output(r1));
        append(stream, loadval(r1, 'd'));
        append(stream, output(r1));
        append(stream, loadval(r1, '!'));
        append(stream, output(r1));
        append(stream, loadval(r1, '\n'));
        append(stream, output(r1));
}


/*  build_add
*  Purpose: should add registers and then halt (no output)
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_add(Seq_T stream) {
        append(stream, add(r1, r2, r3));
        append(stream, halt());
}

/*  build_add_verbose
*  Purpose: should add but then output result and then halt. result is 48+35
*               ouput is that ascii value of this decimal computation
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_add_verbose(Seq_T stream) {
        append(stream, loadval(r5, 48));
        append(stream, loadval(r6, 35));
        append(stream, add(r7, r5, r6));
        append(stream, output(r7));
        append(stream, halt());
}

/*  build_print_digit
*  Purpose: should add 48 to 6 and then output result as ascii
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_print_digit(Seq_T stream) {
        append(stream, loadval(r1, 48));
        append(stream, loadval(r2, 6));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

/*  build_print_six
*  Purpose: should print 6 which is something that's not 6 in ascii
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_print_six(Seq_T stream) {
        append(stream, loadval(r1, 2));
        append(stream, loadval(r2, 4));
        append(stream, add(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}

/*  build_INnOUT
*  Purpose: output whatever char was input and then halt
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_INnOUT(Seq_T stream) {
        append(stream, input(r1));
        append(stream, output(r1));
        append(stream, halt());
}

/*  build_Multiply
*  Purpose: should test loadval and put chars onto stdout printing good\n
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_Multiply(Seq_T stream) {
        append(stream, loadval(r1, 16));
        append(stream, loadval(r2, 4));
        append(stream, multiply(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}


/*  build_Divide
*  Purpose: should divide and then output 50 in ascii
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_Divide(Seq_T stream) {
        append(stream, loadval(r1, 100));
        append(stream, loadval(r2, 2));
        append(stream, divide(r3, r1, r2));
        append(stream, output(r3));
        append(stream, halt());
}


/*  build_CMOV0
*  Purpose: conditional move should make no change
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_CMOV0(Seq_T stream) {
        append(stream, loadval(r1, 100));
        append(stream, loadval(r2, 2));
        append(stream, loadval(r3, 0));
        append(stream, conditionalmove(r1,r2,r3));
        append(stream, output(r1));
        append(stream, output(r2));
        append(stream, output(r3));
        append(stream, halt());
}


/*  build_CMOV1
*  Purpose: conditional move should have r1 store r2
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_CMOV1(Seq_T stream) {
        append(stream, loadval(r1, 20));
        append(stream, loadval(r2, 250));
        append(stream, loadval(r3, 1));
        append(stream, conditionalmove(r1,r2,r3));
        append(stream, output(r1));
        append(stream, output(r2));
        append(stream, output(r3));
        append(stream, halt());
}

/*  build_input_verbose
*  Purpose: input two times and then ouput
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_input_verbose(Seq_T stream) {
        append(stream, input(r1));
        append(stream, input(r2));

        append(stream, output(r1));
        append(stream, loadval(r3, '\n'));
        append(stream, output(r3));
        append(stream, output(r2));
        append(stream, output(r3));
        append(stream, halt());
}

/*  build_nand
*  Purpose: should test loadval and put chars onto stdout printing good\n
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_nand(Seq_T stream) {
        append(stream, loadval(r1, 0));
        append(stream, loadval(r2, 0));
        append(stream, nand(r3,r1,r2));
        append(stream, halt());
}


/*  build_segmented_load
*  Purpose: should test loadval and put chars onto stdout printing good\n
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_segmented_load(Seq_T stream) {
    append(stream, loadval(r1, 0));
    append(stream, loadval(r2, 2));
    append(stream, loadval(r3, 50));
    append(stream, segmentload(r5,r1, r2));//m[0][1]=0
    append(stream, halt());
}

/* build_segmented_store
*  Purpose: should test loadval and put chars onto stdout printing good\n
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_segmented_store(Seq_T stream) {
    append(stream, loadval(r1, 0));
    append(stream, loadval(r2, 1));

     //should be word corresponding to this line append(stream, loadval(r2, 1));
    append(stream, segmentstore(r1,r2, r1));//m[0][1]=0
    append(stream, segmentload(r3, r1, r2)); //r3=0
    append(stream, output(r3));
    append(stream, loadval(r5, 54));
    append(stream, add(r4, r3, r5)); //r3 should be 0, so 0+6 should output 6
    append(stream, output(r4)); //should print 6

    append(stream, halt());
}

/*  build_mapbasic
*  Purpose: should test loadval and put chars onto stdout printing good\n
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_mapbasic(Seq_T stream) {
    append(stream, loadval(r1, 0));
    append(stream, loadval(r2, 1));
    append(stream, loadval(r3, 0));
    append(stream, loadval(r5, 54));
    append(stream, memorymap(r1, r3, r5));
    append(stream, halt());

}

/*  build_unmapbasic
*  Purpose: should test loadval and put chars onto stdout printing good\n
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_unmapbasic(Seq_T stream){
    append(stream, loadval(r1, 0));
    append(stream, loadval(r2, 1));
    append(stream, loadval(r3, 0));
    append(stream, loadval(r5, 54));
    append(stream, memorymap(r1, r3, r5));
    append(stream, memorymap(r1, r2, r5));
    append(stream, memorymap(r1, r2, r5));
    append(stream, memoryunmap(r3));
    append(stream, halt());
}


/*  build_mapunmaphard
*  Purpose: should test loadval and put chars onto stdout printing good\n
*  Parameters: outputstream as a sequence
*  Return: none
*/
void build_mapunmaphard(Seq_T stream){
    append(stream, loadval(r1, 0));
    append(stream, loadval(r2, 1));
    append(stream, loadval(r3, 0));
    append(stream, loadval(r5, 54));

    append(stream, memorymap(r1, r3, r5));
    append(stream, memorymap(r1, r2, r5));
    append(stream, memorymap(r1, r6, r5)); //r6 should have id = 3
    append(stream, memorymap(r1, r3, r5));
    append(stream, memorymap(r1, r2, r5));
    append(stream, memorymap(r1, r2, r5));
    append(stream, memorymap(r1, r3, r5));
    append(stream, memorymap(r1, r2, r5));
    //word instruction segment + 8 memory segments

    append(stream, memoryunmap(r6)); //removes segment at index 3
    append(stream, memoryunmap(r3)); //removes segment at index 7

    append(stream, halt());
}