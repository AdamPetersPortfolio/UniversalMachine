/****************************************************************************
 *  COMP 40 HW6 Universal Machine                                           *
 *                                                                          *
 *  registers.c                                                             *
 *  Complete Registers Array Implementation                                 *
 *  Created By (UTLN): jlai03 & apeter09                                    *
 *           On      : 11/22/2020                                           *
 *                                                                          *
 ****************************************************************************/

#include "registers.h"

/************************ CONSTRUCTOR / DESTRUCTOR ***************************/
Reg_arr Reg_new(int num_reg) 
{
    assert(num_reg >= 0);
    return UArray_new(num_reg, sizeof(uint32_t));
}

void Reg_free(Reg_arr *r) 
{
    assert(r != NULL);
    UArray_free(r);
}

/**************************** GETTER / SETTER ********************************/
uint32_t Reg_get(Reg_arr r, int index) 
{
    assert(r!= NULL && index >= 0);
    return *(uint32_t *)UArray_at(r, index);
} 

uint32_t Reg_set(Reg_arr r, int index, uint32_t val) 
{
    assert(r != NULL && index >= 0);
    uint32_t *elem = UArray_at(r, index);
    *elem = val;
    return val;
}

