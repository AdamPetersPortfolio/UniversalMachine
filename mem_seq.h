/****************************************************************************
 *  COMP 40 HW6 Universal Machine                                           *
 *                                                                          *
 *  mem_seq.h                                                               *
 *  Complete Memory Sequence Interface                                       *
 *  Created By (UTLN): jlai03 & apeter09                                    *
 *           On      : 11/22/2020                                           *
 *                                                                          *
 *  Contains functions for Memory Sequence Data Structure. Memory Sequence  *
 *  contains pointers to UArry Segments. in each index. Functions allow     *
 *  for a) Creating and Freeing a whole Memory Sequence and b) Adding,      *
 *  Accessing, Loading, and Freeing a segment in memory.                    *
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>
#include <uarray.h>
#include <seq.h>
#include <stack.h>

typedef struct Mem_Seq {
    Seq_T memory;
    UArray_T identifiers;
} *Mem_Seq;

/*
  Function:  Mem_new
  Arguments: int mem_len - length of Memory Sequence (must be >= 0)
  Does:      Creates and returns a Mem_Seq struct. The memory sequence (Seq_T) 
             of Mem_Seq is created with a length of (int mem_len) and the 
             identifier UArray of Mem_Seq is created with 20 indices. The first
             index of the identifier array is initialized with 1 (for count).
  Return:    Mem_Seq struct 
*/
extern Mem_Seq Mem_new(int mem_len);

/*
  Function:  Mem_free_all
  Arguments: Mem_Seq *m - pointer to a Mem_Seq struct 
  Does:      Takes in a pointer to a Mem_Seq struct, frees the corresponding 
             segments in memory, the memory sequence, the identifier array, 
             and the struct itself
  Return:    none - void
*/
extern void Mem_free_all(Mem_Seq *m);

/*
  Function:  Mem_addseg
  Arguments: Mem_Seq m - Mem_Seq struct 
             int length - length of requested segment
  Does:      Adds a segment of memory to the memory sequence. Utilizes 
             identifier array for determining whether to use an old index
             in memory or to add a new segment in the sequence 
  Return:    unsigned - index where the segment was mapped to. Returns 0
                        if no more memory was available to map a segment. 
*/
extern unsigned Mem_addseg(Mem_Seq m, int length);

/*
  Function:  Mem_seg_at
  Arguments: Mem_Seq m - Mem_Seq struct 
             int seg_index - index of requested segment in memory
             int index - index of requested value in segment
  Does:      Goes to the memory sequence, accesses the segment at (seg_index), 
             and returns a pointer to the value at (index) in the accessed 
             segment. This essentially allows the client to access a given 
             value in memory.
  Return:    void * - pointer to index
*/
extern void *Mem_seg_at(Mem_Seq m, int seg_index, int index);

/*
  Function:  Mem_seglength
  Arguments: Mem_Seq m - Mem_Seq struct 
             int seg_index - index of requested segment in memory
  Does:      Goes to the memory sequence and returns the length of the
             specified segment at (seg_index)
  Return:    int length - legnth of requested segment
*/
extern int  Mem_seglength(Mem_Seq m, int seg_index);  

/*
  Function:  Mem_freeseg
  Arguments: Mem_Seq m - Mem_Seq struct 
             int seg_index - index of specified segment in memory
  Does:      Goes to the memory sequence and frees the specified segment
             at (seg_index). Pushes seg_index to identifiers array for 
             conservation of space in memory sequence.
  Return:    none - void
*/
extern void Mem_freeseg(Mem_Seq m, int seg_index);   

/*
  Function:  Mem_load
  Arguments: Mem_Seq m - Mem_Seq struct 
             int seg_index - index of specified segment in memory
  Does:      Goes to the memory sequence and loads the segment at the specified
             (seg_index) into the first index of the sequence.
  Return:    none - void
*/
extern void Mem_load(Mem_Seq m, int seg_index);  


