/****************************************************************************
 *  COMP 40 HW6 Universal Machine                                           *
 *                                                                          *
 *  operations_cpu.c                                                        *
 *  Complete Operations CPU Implementation                                  *
 *  Created By (UTLN): jlai03 & apeter09                                    *
 *           On      : 11/22/2020                                           *
 *                                                                          *
 ****************************************************************************/

#include "operations_cpu.h"

/********************* MEMORY MOVEMENT FUNCTIONS *****************************/
/*
  Function:  move
  Arguments: Reg_arr r - pointer to Register UArray
             uint32_t a - index of register A (i.e. r[a])
             uint32_t b - index of register B (i.e. r[b])
             uint32_t c - index of register C (i.e. r[c])
  Does:      If r[c] is not equal to 0, then it moves r[b] into r[a]
  Return:    none - void
*/
static void move(Reg_arr r, uint32_t a, uint32_t b, uint32_t c)
{
    if (Reg_get(r, c) != 0) {
        Reg_set(r, a, Reg_get(r, b));
    }
}

/*
  Function:  load
  Arguments: Reg_arr r - pointer to Register UArray
             Mem_Seq m -  pointer to Mem_Seq struct
             uint32_t a - index of register A (i.e. r[a])
             uint32_t b - index of register B (i.e. r[b])
             uint32_t c - index of register C (i.e. r[c])
  Does:      Sets r[a] to the value at index r[c] of segment r[b] in memory
  Return:    none - void
*/
static void load(Reg_arr r, Mem_Seq m, uint32_t a, uint32_t b, uint32_t c)
{
    Reg_set(r, a, *(uint32_t *)Mem_seg_at(m, Reg_get(r, b), Reg_get(r, c)));
}

/*
  Function:  store
  Arguments: Reg_arr r - pointer to Register UArray
             Mem_Seq m -  pointer to Mem_Seq struct
             uint32_t a - index of register A (i.e. r[a])
             uint32_t b - index of register B (i.e. r[b])
             uint32_t c - index of register C (i.e. r[c])
  Does:      Sets the memory at index r[a] in segment r[b] to the value 
             at r[c]
  Return:    none - void
*/
static void store(Reg_arr r, Mem_Seq m, uint32_t a, uint32_t b, uint32_t c)
{
    *(uint32_t *)Mem_seg_at(m, Reg_get(r, a), Reg_get(r, b)) = Reg_get(r, c);
}

/********************* ARITHMETIC FUNCTIONS **********************************/
/*
  Function:  add
  Arguments: Reg_arr r - pointer to Register UArray
             uint32_t a - index of register A (i.e. r[a])
             uint32_t b - index of register B (i.e. r[b])
             uint32_t c - index of register C (i.e. r[c])
  Does:      Sets the memory at r[a] to the sum of r[b] + r[c] 
  Return:    none - void
*/
static void add(Reg_arr r, uint32_t a, uint32_t b, uint32_t c)
{
    Reg_set(r, a, Reg_get(r, b) + Reg_get(r, c));
}

/*
  Function:  mult
  Arguments: Reg_arr r - pointer to Register UArray
             uint32_t a - index of register A (i.e. r[a])
             uint32_t b - index of register B (i.e. r[b])
             uint32_t c - index of register C (i.e. r[c])
  Does:      Sets the memory at r[a] to the product of r[b] * r[c] 
  Return:    none - void
*/
static void mult(Reg_arr r, uint32_t a, uint32_t b, uint32_t c)
{
    Reg_set(r, a, Reg_get(r, b) * Reg_get(r, c));
}

/*
  Function:  divide
  Arguments: Reg_arr r - pointer to Register UArray
             uint32_t a - index of register A (i.e. r[a])
             uint32_t b - index of register B (i.e. r[b])
             uint32_t c - index of register C (i.e. r[c])
  Does:      Assuming r[c] is not equal to 0, this function sets the memory 
             at r[a] to the quotient of r[b] / r[c] 
  Return:    none - void
*/
static void divide(Reg_arr r, uint32_t a, uint32_t b, uint32_t c)
{
    assert(Reg_get(r, c) != 0); 
    Reg_set(r, a, Reg_get(r, b) / Reg_get(r, c));
}

/*
  Function:  nand
  Arguments: Reg_arr r - pointer to Register UArray
             uint32_t a - index of register A (i.e. r[a])
             uint32_t b - index of register B (i.e. r[b])
             uint32_t c - index of register C (i.e. r[c])
  Does:      Sets the memory at r[a] to the bitwise equivalent
             of ~(r[b] & r[c]) 
  Return:    none - void
*/
static void nand(Reg_arr r, uint32_t a, uint32_t b, uint32_t c)
{
    Reg_set(r, a, ~(Reg_get(r, b) & Reg_get(r, c)));
}

/*
  Function:  halt
  Arguments: Reg_arr r - pointer to Register UArray
             Mem_Seq m -  pointer to Mem_Seq struct
             cpu cpu - cpu struct of functions
  Does:      Frees the Register Array, the Mem_Seq Struct, and the cpu struct. 
             Then it exits the program with exit code (EXIT_SUCCESS)
  Return:    none - void
*/
static void halt(Reg_arr r, Mem_Seq m, cpu cpu) 
{
    Reg_free(&r);
    Mem_free_all(&m);
    Free_cpu(&cpu);
    exit(EXIT_SUCCESS);
}

/********************* MAPPING FUNCTIONS *************************************/
/*
  Function:  map
  Arguments: Reg_arr r - pointer to Register UArray
             Mem_Seq m -  pointer to Mem_Seq struct
             uint32_t b - index of register B (i.e. r[b])
             uint32_t c - index of register C (i.e. r[c])
  Does:      Creates a new memory seqment of length r[c] and initializes each 
             index to 0. Then it sets the value in r[b] to the index of that 
             created segment. 
  Return:    none - void
*/ 
static void map(Reg_arr r, Mem_Seq m, uint32_t b, uint32_t c)
{
    unsigned placement = Mem_addseg(m, Reg_get(r, c));
    /* Ensure that there was enough memory to map a new segment */
    assert(placement != 0);
    Reg_set(r, b, placement);
}

/*
  Function:  unmap
  Arguments: Reg_arr r - pointer to Register UArray
             Mem_Seq m -  pointer to Mem_Seq struct
             uint32_t c - index of register C (i.e. r[c]) 
  Does:      Frees the specified segment at index r[c] in memory.
  Return:    none - void
*/ 
static void unmap(Reg_arr r, Mem_Seq m, uint32_t c)
{
    Mem_freeseg(m, Reg_get(r, c));
}

/*********************** I/O FUNCTIONS ***************************************/
/*
  Function:  output
  Arguments: Reg_arr r - pointer to Register UArray
             Mem_Seq m -  pointer to Mem_Seq struct
             uint32_t c - index of register C (i.e. r[c]) 
  Does:      Retrieves value at r[c] and checks that it is within the 
             ASCII range [0,255]. Then it outputs r[c] to stdout with putchar.
  Return:    none - void
*/ 
static void output(Reg_arr r, uint32_t c)
{
    unsigned num = Reg_get(r,c);
    assert(num <= 255);
    putchar(Reg_get(r,c));
}

/*
  Function:  input
  Arguments: Reg_arr r - pointer to Register UArray
             Mem_Seq m -  pointer to Mem_Seq struct
             uint32_t c - index of register C (i.e. r[c]) 
  Does:      Retrieves an input value from stdin using getchar() and initalizes
             it to r[c] in memory. Checks to see if getchar() reaches the 
             End of the File.
  Return:    none - void
*/ 
static void input(Reg_arr r, uint32_t c)
{
    int val = getchar();
    assert(val >= 0 && val <= 255);
    /* Set the register to all 1's if the end of input has been signalled */
    if (val == EOF) {
        Reg_set(r, c, ~((unsigned)0));
    } else {
        Reg_set(r, c, val);
    }
}

/*********************** LOAD FUNCTIONS **************************************/
/*
  Function:  load_program
  Arguments: Reg_arr r - pointer to Register UArray
             Mem_Seq m -  pointer to Mem_Seq struct
             uint32_t b - index of register B (i.e. r[b]) 
  Does:      Calls Memory Load on the segment at index r[b]. In other words, 
             Mem_Load moves the segment at r[b] into the first index of the 
             memory sequence.
  Return:    none - void
*/ 
static void load_program(Reg_arr r, Mem_Seq m, uint32_t b)
{
    Mem_load(m, Reg_get(r, b));
}

/*
  Function:  load_value
  Arguments: Reg_arr r - pointer to Register UArray
             uint32_t a - index of register A (i.e. r[a]) 
             uint32_t val - value to be initialized
  Does:      Sets the value at r[a] in memory to (uint32_t val)
  Return:    none - void
*/ 
static void load_value(Reg_arr r, uint32_t a, uint32_t val)
{
    Reg_set(r, a, val);
}

/*********************** CPU FUNCTIONS ***************************************/
cpu Create_cpu() {
    cpu c = malloc(sizeof(*c));
    assert(c != NULL);
    c->move = move;
    c->load = load;
    c->store = store;
    c->add = add;
    c->mult = mult;
    c->divide = divide;
    c->nand = nand;
    c->halt = halt;
    c->map = map;
    c->unmap = unmap;
    c->output = output;
    c->input = input;
    c->load_program = load_program;
    c->load_value = load_value;
    return c;
}

void Free_cpu(cpu *c) {
    free(*c);
}
