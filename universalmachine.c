/****************************************************************************
 *  COMP 40 HW6 Universal Machine                                           *
 *                                                                          *
 *  universalmachine.c                                                      *
 *  Complete Universal Machine Implementation                               *
 *  Created By (UTLN): jlai03 & apeter09                                    *
 *           On      : 11/22/2020                                           *
 *                                                                          *
 *  Contains the implementation for a Universal Machine, which reads and    *
 *  processes .um files. The machine allocates and deallocate memory in a   *
 *  memory sequence and a Register Array. Further, the machine performs     *
 *  various operations on this memory with an Operations CPU.               *
 *                                                                          *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include "operations_cpu.h"
#include "bitpack.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

/********************* STRUCT & FUNCTION DECLARATIONS ************************/

/* Universal Machine Opcodes */
typedef enum Um_opcode {
        CMOV = 0, SLOAD, SSTORE, ADD, MUL, DIV,
        NAND, HALT, MAP, UNMAP, OUT, IN, LOADP, LV
} Um_opcode;

/* Instruction Info Struct */
typedef struct instr_info {
    uint32_t value;
    Um_opcode op_type;
    char a;
    char b; 
    char c;
} instr_info;

/* Function Declarations */
void read_file(char *filename, Mem_Seq m);
void exec_um(Mem_Seq m, Reg_arr r, cpu cpu);
Um_opcode op_type(uint32_t instruction);
void op_parse(uint32_t instruction, instr_info *info);
void exec_instr(Mem_Seq m, Reg_arr r, cpu cpu, instr_info *info);
void quit_um(Mem_Seq m, Reg_arr r);

/******************* READ FILE FUNCTION **************************************/
/*
  Function:  read_file
  Arguments: char *filename - name of the file to be opened
             Mem_Seq m - Mem_Seq struct to be intialized
  Does:      Intializes first segment of Mem_Seq's memory with enough 
             indices to hold all the instructions to be read in. Opens the 
             specified file, bitpacks each instruction, and stores it into the 
             first segment of the memory sequence. 
  Return:    none - void
*/
void read_file(char *filename, Mem_Seq m)
{
    /* Add Segment with enough indices to hold instructions */
    struct stat* s = malloc(sizeof(*s));
    stat(filename, s);
    unsigned num_words = (s->st_size) / 4; 
    FILE *f = fopen(filename, "r");
    if (f == NULL) {
        fprintf(stderr, "No such file or directory\n");
        free(s); 
        Mem_free_all(&m);
        exit(EXIT_FAILURE);
    }

    Mem_addseg(m, num_words);

    /* Create instructions word */
    for (unsigned i = 0; i < num_words; i++){
        uint32_t word = 0;
        for (unsigned z = 0; z < 4; z ++) {
            word = Bitpack_newu(word, 8, 24 - z * 8, getc(f));
        }
        uint32_t *elem = Mem_seg_at(m, 0, i);
        *elem = word;
    }
    /* Free stat pointer and close file */
    free(s); 
    fclose(f);    
}

/******************** UM EXECUTION FUNCTIONS *********************************/
/*
  Function:  exec_um
  Arguments: Mem_Seq m - pointer to Mem_Seq struct 
             Reg_arr r - pointer to Register Array 
             cpu cpu - pointer to cpu struct of functions
  Does:      Uses a program counter to iterate through the instructions 
             stored in the first segment of memory. Each instruction is 
             executed and the overarching data structures are changed 
             accordingly.
  Return:    none - void
*/
void exec_um(Mem_Seq m, Reg_arr r, cpu cpu)
{
    int program_counter = 0;
    int instr_length = Mem_seglength(m, 0);
    instr_info info;

    /* Iterate through Instruction Segment */
    while (program_counter < instr_length) {
        /* Parse and Execute Instruction */
        uint32_t instruction = *((uint32_t *) Mem_seg_at(m, 0, 
                                                         program_counter));
        info.op_type = op_type(instruction);
        op_parse(instruction, &info);
        exec_instr(m, r, cpu, &info);

        /* Update Program Counter */
        if (info.op_type == LOADP) {
            program_counter = Reg_get(r, info.c);
            instr_length = Mem_seglength(m, 0);
        } else {
            program_counter ++;
        }
    }
    cpu->halt(r, m, cpu);
}

/*
  Function:  op_type
  Arguments: uint32_t instruction - bitpacked 
  Does:      Takes a given (uint32_t) instruction and returns the encoded 
             optype in the bit representation. 
  Return:    none - void
*/
Um_opcode op_type(uint32_t instruction)
{
    return Bitpack_getu(instruction, 4, 28);
}

/*
  Function:  op_parse
  Arguments: uint32_t instruction
             instr_info *info - pointer to instruction info struct
  Does:      Parses a given (uint32_t) instruction and initializes the 
             passed in instr_info struct based on the instruction's op_type.
  Return:    none - void
*/
void op_parse(uint32_t instruction, instr_info *info)
{
    if (info->op_type == LV) {
        info->value = Bitpack_getu(instruction, 25, 0);
        info->a = Bitpack_getu(instruction, 3, 25);
    } else {
        info->a = Bitpack_getu(instruction, 3, 6);
        info->b = Bitpack_getu(instruction, 3, 3);
        info->c = Bitpack_getu(instruction, 3, 0);
    }
}

/*
  Function:  exec_instr
  Arguments: Mem_seq m - pointer to Mem_Seq struct
             Reg_arr r - pointer to Register Array 
             cpu cpu - pointer to cpu struct of functions 
             instr_info *info - pointer to instruction info struct
  Does:      Takes in a pointer to an instr_info struct and executes the 
             encoded instruction indicated by the struct's optype. Each optype
             aligns with a function which exists in the cpu function struct. 
  Return:    none - void
*/
void exec_instr(Mem_Seq m, Reg_arr r, cpu cpu, instr_info *info)
{
    switch (info->op_type) {
        case CMOV: 
            cpu->move(r, info->a, info->b, info->c);         
            break;
        case SLOAD: 
            cpu->load(r, m, info->a, info->b, info->c);     
            break;
        case SSTORE: 
            cpu->store(r, m, info->a, info->b, info->c);   
            break;
        case ADD: 
            cpu->add(r, info->a, info->b, info->c);           
            break;
        case MUL: 
            cpu->mult(r, info->a, info->b, info->c);          
            break;
        case DIV: 
            cpu->divide(r, info->a, info->b, info->c);        
            break;
        case NAND: 
            cpu->nand(r, info->a, info->b, info->c);         
            break; 
        case HALT: 
            cpu->halt(r, m, cpu);                            
            break; /* TODO */
        case MAP: 
            cpu->map(r, m, info->b, info->c);                 
            break;
        case UNMAP: 
            cpu->unmap(r, m, info->c);                      
            break;
        case OUT: 
            cpu->output(r, info->c);                  
            break;
        case IN: 
            cpu->input(r, info->c);                    
            break;
        case LOADP: 
            cpu->load_program(r, m, info->b);               
            break;
        case LV: 
            cpu->load_value(r, info->a, info->value);          
            break;
        default: 
            assert(info->op_type < 14);

    }
}

/********************* MAIN FUNCTION *****************************************/
int main(int argc, char *argv[]) 
{
    /* Create Memory Sequence, Register Array, and CPU function struct */
    Mem_Seq m = Mem_new(4);
    Reg_arr r = Reg_new(8);
    cpu cpu = Create_cpu();

    /* Read in File */
    if (argc > 2 ) {
        fprintf(stderr, "Too many files specified.\n");
        exit(EXIT_FAILURE);
    } else if (argc == 2) {
        read_file(argv[1], m);
    } else {
        fprintf(stderr, "No file specified.\n");
        exit(EXIT_FAILURE);
    }

    /* Universal Machine Execution */
    exec_um(m, r, cpu);

    return 0;
}
