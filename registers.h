/****************************************************************************
 *  COMP 40 HW6 Universal Machine                                           *
 *                                                                          *
 *  registers.h                                                             *
 *  Complete Registers Array Interface                                      *
 *  Created By (UTLN): jlai03 & apeter09                                    *
 *           On      : 11/22/2020                                           *
 *                                                                          *
 *  Contains the functions for the Registers data structure. Represents     *
 *  each register as an element in a hanson UArray_T. Functions allow       *
 *  for creating and freeing of the whole data structure, and retrieval and *
 *  setting of the data stored within each register.                          *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <uarray.h>

typedef UArray_T Reg_arr; 

/*
  Function:  Reg_new
  Arguments: int num_reg - the number of indices for the Register Array
  Does:      Creates a Reg_Arr UArray with (int num_reg) indices. The UArray 
             stores (uint32_t) values
  Return:    Reg_Arr - pointer to Register UArray
*/
extern Reg_arr Reg_new(int num_reg);

/*
  Function:  Reg_free
  Arguments: Reg_arr *r - double pointer to Reg_Arr 
  Does:      Frees the Reg_Arr UArray using UArray_free
  Return:    none - void
*/
extern void Reg_free(Reg_arr *r);

/*
  Function:  Reg_get
  Arguments: Reg_arr r - pointer to Reg_Arr
             int index - index of requested value 
  Does:      Accesses the value at (int index) of the Register Array and 
             returns it to the client.
  Return:    uint32_t - value at the specified index of Reg_arr
*/
extern uint32_t Reg_get(Reg_arr r, int index); 

/*
  Function:  Reg_set
  Arguments: Reg_arr r - pointer to Reg_Arr
             int index - requested index to be initialized
             uint32_t val - value to be initialized
  Does:      Initializes (uint32_t val) to the specified index (int index) in 
             the provided Register Array. Returns the value that was 
             intialized.
  Return:    uint32_t - value that was initalized
*/
extern uint32_t Reg_set(Reg_arr r, int index, uint32_t val);
