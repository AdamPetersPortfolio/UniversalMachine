# Universal Machine

The following repository implements a simple virtual-machine with eight general-purpose registers, an address space divided into memory segments, and an I/O device capable of displaying ASCII characters and performing input and output. The machine can process 14 different instructions. Instructions are unpacked from a binary file of 32 bit words. This project was originally assigned and created by the comp40 department at Tufts University, and the project specifications can be found here: https://www.cs.tufts.edu/comp/40/assignments/hw06/um.pdf.

Creators: Jonathan Lai (jlai03) & Adam Peters (apeter09)

    mem_seq.h/c - Mem_Seq was tested with multiple edges cases and is fully
                  functional to our knowledge. The interface allows for
                  a) creating and freeing a memory sequence, b) Adding,
                  Accessing, Freeing, and Loading Memory Segments. The
                  implementation relies on the creation of a dynamic UArray
                  for the Identifier Array (which was also tested with
                  a variety of inputs).

    registers.h/c - This interface for Reg_arr was tested with a variety of
                    inputs and is fully functional to our knowledge. The
                    interface allows for a) creating and freeing the Register
                    Array and b) Getting and Setting Memory in the Register
                    Array. Each function was able to to handle all edge cases
                    and inputs.

    operations_cpu.h/c - The functions contained in the cpu struct were
                         each tested with edge case and normally inputs for
                         a, b, and c's values. This interface is able to
                         execute a variety of operations in Universal Machine
                         including functions that a) move around memory,
                         b) perform simple arithmetic computations,
                         c) map and unmap memory segments, d) handle input
                         and output, and e) load values / programs (altering
                         the data structures accordingly). Each function was
                         tested rigorously and thus makes for a robust
                         struct of functions in cpu.

    universalmachine.c - The individual functions in this file were each tested
                         with a variety of inputs - correctly performing
                         against edge cases and assert triggers. Collectively,
                         the program was able to perform correctly against most
                         of the .um test files that were provided and the
                         .um tests that we created. Thus, to our knowledge,
                         this interface was implemented correctly and is fully
                         functional. This interface specifically allows for
                         reading in a .um file into a Segment of instructions
                         in memory then iterating over said segment and
                         executing the instructions accordingly. It was able
                         to handle instruction cycles of high volumes.


Departures from Design: Other than a few minor departures in syntax, we had
                        one major addition to our initial design. We forgot
                        that we needed a way to keep track of unmapped indices
                        in our sequence that could be reused to save space
                        in memory. Thus, we changed Mem_Seq to a struct
                        internally containing a) the memory sequence alongside
                        b) an auxiliary UArray that we programmed to be
                        dynamic / resizable.

                        This Array (called identifiers) would contain a list of
                        the indices at which memory is unmapped. Thus every
                        time a segment must be added, we first check the
                        identifiers array to see if there are any open indices
                        in memory that we can reuse - if not, we then continue
                        to add an index in the memory sequence. We specifically
                        self programmed a resizable UArray to avoid pointer
                        chasing (which Hanson's data structures rely on heavily
                        - drastically slowing runtime).


## Architecture of Universal Machine

        The general Architecture for Universal Machine is as follows:

        a) [Mem_Seq] struct containing a Memory Sequence and a
           Identifier Array

           The Memory Sequence provides an interface for a sequence (Seq_T)
           of segments (UArray_T). By providing functions for Adding and
           Maneuvering segments, the client can not directly control or
           alter the placing of segments - rather the interface allows
           a structured, clean, and error free way of maintaining memory
           using this intersection of two data structures.

           The Identifier Array is a UArray that is implemented to be
           dynamic/resizable. The resizing and adding function is not provided
           to the client so that memory and space of the UArray can be
           controlled within the parameters of the program. We chose to use
           UArray over other Hansons data structures to avoid pointer chasing
           thus conserving runtime.

        b) [Reg_arr] array holding the 8 auxiliary register values for
           sustaining locality in our machine system

           The Reg_Arr is basically just a UArray with further defined
           functions. The Reg_arr interface only allows for
           a) constructing / destructing and b) getting / setting. By defining
           and restricting the client access to these four functions, we avoid
           errors that arise form direct client access.

        c) [cpu] struct containing functions that operate on [Mem_Seq] and
           [Reg_arr] to perform basic maneuvers of memory and simple
           arithmetic computations

           cpu itself is a pointer to a struct containing function pointers.
           Thus, the client can only create and free the cpu struct, but
           can NOT directly access the function pointers themselves (rather
           they can only call them). In other words, the implementations of
           the functions are not conventionally listed in the .h file, rather
           they statically implemented in the .c file such that clients
           can only call the functions by creating an instance of the cpu
           struct. This allows for both a more modular and abstract solution
           to the list of operations that are used in Universal Machine's
           processing of instructions.

        The first step of the program is to read in a file of .um instructions
        into the first segment of [Mem_Seq]. Then an integer program counter
        is used to iterate through the instruction segment and execute
        each instruction separately. The universalmachine.c file contains
        helper functions that parse and process the bitpacked instructions.
        Each instruction subsequently calls functions in the cpu struct that
        directly alter the memory in [Mem_Seq] and [Reg_arr] via the interfaces
        provide for them. Thus the memory flow between modules is as follows

        1. universalmachine.c parses instructions and calls functions in the
           cpu struct

        2. The cpu struct calls functions in the Register Array and Mem_Seq
           interface. This way cpu can maneuver memory and perform computations
           as requested.

        3. The Mem_Seq and Register Array interfaces allow regulated control
           and access of the underlying data structures - as to prevent errors
           that can arise from direct access/control to the underlying data
           structures.


