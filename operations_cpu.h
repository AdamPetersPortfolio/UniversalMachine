/****************************************************************************
 *  COMP 40 HW6 Universal Machine                                           *
 *                                                                          *
 *  operations_cpu.h                                                        *
 *  Complete Operations CPU Interface                                       *
 *  Created By (UTLN): jlai03 & apeter09                                    *
 *           On      : 11/22/2020                                           *
 *                                                                          *
 *  Contains cpu struct of functions for operating on Memory Sequence and   *
 *  Register Array. This interface is responsible for controlling where     *
 *  memory is moved and stored in a variety operations. Operations include  *
 *  basic arithmetic, loading/storing/moving data, mapping data,            *
 *  input/output of data, and more. Functions are instantiated via          *
 *  Create_cpu() and Free_cpu().                                            *
 ****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <uarray.h>
#include "registers.h"
#include "mem_seq.h"

/* CPU Operations Struct */
typedef struct cpu{
    void (*move) (Reg_arr r, uint32_t a, uint32_t b, uint32_t c);
    void (*load) (Reg_arr r, Mem_Seq m, uint32_t a, uint32_t b, uint32_t c);
    void (*store) (Reg_arr r, Mem_Seq m, uint32_t a, uint32_t b, uint32_t c);
    void (*add) (Reg_arr r, uint32_t a, uint32_t b, uint32_t c);
    void (*mult) (Reg_arr r, uint32_t a, uint32_t b, uint32_t c);
    void (*divide) (Reg_arr r, uint32_t a, uint32_t b, uint32_t c);
    void (*nand) (Reg_arr r, uint32_t a, uint32_t b, uint32_t c);
    void (*halt) (Reg_arr r, Mem_Seq m, struct cpu *cpu);
    void (*map) (Reg_arr r, Mem_Seq m, uint32_t b, uint32_t c);
    void (*unmap) (Reg_arr r, Mem_Seq m, uint32_t c);
    void (*output) (Reg_arr r, uint32_t c);
    void (*input) (Reg_arr r, uint32_t c);
    void (*load_program) (Reg_arr r, Mem_Seq m, uint32_t b);
    void (*load_value) (Reg_arr r, uint32_t a, uint32_t val);
} *cpu;
 
/*
  Function:  Create_cpu
  Arguments: none
  Does:      Creates cpu struct and intializes all the function pointers to the
             struct. This pointer is then returned so that client has access
             to all the function pointers.
  Return:    cpu c - pointer to cpu struct of functions
*/
extern cpu Create_cpu();

/*
  Function:  Free_cpu
  Arguments: cpu *c - take double pointer to the cpu struct
  Does:      Dereferences and frees the cpu struct that was created
  Return:    none - void
*/
extern void Free_cpu(cpu *c);
