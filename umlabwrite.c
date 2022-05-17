/**************************************************************
*
*                     umlabwrite.c
*
*     Assignment: um
*     Authors: Caleb Pekowsky, Labib Afia
*     Date:     04//2022
*
*     summary:
*
*     this is the .c file for our umlabwrite. this helps write test functions.
*      
**************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"
#include "fmt.h"
#include "seq.h"


extern void Um_write_sequence(FILE *output, Seq_T instructions);

extern void build_halt_test(Seq_T instructions);
extern void build_verbose_halt_test(Seq_T instructions);
extern void build_add(Seq_T instructions);
extern void build_print_six(Seq_T instructions);
extern void build_print_digit(Seq_T instructions);
extern void build_INnOUT(Seq_T stream);
extern void build_Divide(Seq_T stream);
extern void build_Multiply(Seq_T stream);
extern void build_CMOV0(Seq_T stream);
extern void build_CMOV1(Seq_T stream);
extern void build_nand(Seq_T stream);
extern void build_segmented_load(Seq_T stream);
extern void build_segmented_store(Seq_T stream);
extern void build_loadval_test(Seq_T stream);
extern void build_mapbasic(Seq_T stream);

extern void build_unmapbasic(Seq_T stream);
extern void build_mapunmaphard(Seq_T stream);

extern void build_output_test(Seq_T stream);
extern void build_add_verbose(Seq_T stream);
extern void build_input_verbose(Seq_T stream);
extern void build_segmented_store(Seq_T stream);

/* The array `tests` contains all unit tests for the lab. */

static struct test_info {
        const char *name;
        const char *test_input;          /* NULL means no input needed */
        const char *expected_output;
        /* writes instructions into sequence */
        void (*build_test)(Seq_T stream);
} tests[] = {
        { "halt",         NULL, "", build_halt_test },
        { "halt-verbose", NULL, "", build_verbose_halt_test },
        { "add"         , NULL, "", build_add},
        {"print-digit"  , NULL, "", build_print_digit},
        { "print-six",  NULL, "", build_print_six},
        {"in-out"    ,  NULL, "", build_INnOUT},
        {"divide"    ,  NULL, "", build_Divide},
        {"multiply"    ,  NULL, "", build_Multiply},
        {"conditional1", NULL, "", build_CMOV1},
        {"conditional0", NULL, "", build_CMOV0},
        {"nand", NULL, "", build_nand},
        {"sload", NULL, "", build_segmented_load},
        {"sstore", NULL, "", build_segmented_store},
        {"mapbasic", NULL, "", build_mapbasic},
        {"unmapbasic", NULL, "", build_unmapbasic},
        {"mapunmaphard", NULL, "", build_mapunmaphard},
        {"output", NULL, "", build_output_test},
        {"loadval", NULL, "", build_loadval_test},
        {"add-verbose", NULL, "", build_add_verbose},
        {"input-verbose", NULL, "", build_input_verbose},
        {"segstore", NULL, "", build_segmented_store}

};

  
#define NTESTS (sizeof(tests)/sizeof(tests[0]))

/*
 * open file 'path' for writing, then free the pathname;
 * if anything fails, checked runtime error
 */
static FILE *open_and_free_pathname(char *path);

/*
 * if contents is NULL or empty, remove the given 'path', 
 * otherwise write 'contents' into 'path'.  Either way, free 'path'.
 */
static void write_or_remove_file(char *path, const char *contents);

static void write_test_files(struct test_info *test);


int main (int argc, char *argv[])
{
        bool failed = false;
        if (argc == 1)
                for (unsigned i = 0; i < NTESTS; i++) {
                        printf("***** Writing test '%s'.\n", tests[i].name);
                        write_test_files(&tests[i]);
                }
        else
                for (int j = 1; j < argc; j++) {
                        bool tested = false;
                        for (unsigned i = 0; i < NTESTS; i++)
                                if (!strcmp(tests[i].name, argv[j])) {
                                        tested = true;
                                        write_test_files(&tests[i]);
                                }
                        if (!tested) {
                                failed = true;
                                fprintf(stderr,
                                        "***** No test named %s *****\n",
                                        argv[j]);
                        }
                }
        return failed; /* failed nonzero == exit nonzero == failure */
}


static void write_test_files(struct test_info *test)
{
        FILE *binary = open_and_free_pathname(Fmt_string("%s.um", test->name));
        Seq_T instructions = Seq_new(0);
        test->build_test(instructions);
        Um_write_sequence(binary, instructions);
        Seq_free(&instructions);
        fclose(binary);

        write_or_remove_file(Fmt_string("%s.0", test->name),
                             test->test_input);
        write_or_remove_file(Fmt_string("%s.1", test->name),
                             test->expected_output);
}


static void write_or_remove_file(char *path, const char *contents)
{
        if (contents == NULL || *contents == '\0') {
                remove(path);
        } else {
                FILE *input = fopen(path, "wb");
                assert(input != NULL);

                fputs(contents, input);
                fclose(input);
        }
        free(path);
}


static FILE *open_and_free_pathname(char *path)
{
        FILE *fp = fopen(path, "wb");
        assert(fp != NULL);

        free(path);
        return fp;
}
