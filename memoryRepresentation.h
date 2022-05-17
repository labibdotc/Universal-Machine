/*
    This file contains memory representation for UM simulator program owned by
    Caleb Pekowsky and Labib A. Afia
    Date: Apr 14 2022
*/



typedef struct Memory_T *Memory_T;
struct Memory_T {
    Seq_T memory; 
    Seq_T unused_memory;
};
