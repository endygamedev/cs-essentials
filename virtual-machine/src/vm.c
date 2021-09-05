/* Name: Virtual stack machine implementation */
/* Author: Egor Bronnikov */
/* Last edited: 4-09-2021 */


#include <stdio.h>
#include <stdbool.h>

#define STACK_SIZE 256


// Variables
int stack[STACK_SIZE];  /* Stack */
bool running = true;


// Instructions for our program
typedef enum {
    PSH,    /* Add an item to the stack; */
    ADD,    /* Pop two elements from the top of the stack and adds them, the result pushes to the stack; */
    POP,    /* Pop an element from the top of the stack and display it on the screen; */
    SET,    /* Set register to value; */
    HLT     /* Stop the program; */
} InstructionSet;


// Registers
typedef enum {
    A, B, C, D, E, F, IP, SP,
    REGISTER_SIZE    /* This necessary to get number of registers */
} Registers;


int registers[REGISTER_SIZE];

#define sp (registers[SP])
#define ip (registers[IP])


// Example of a program
const int program[] = {
    PSH, 5,
    PSH, 6,
    ADD,
    POP,
    HLT
};


// Evaluate the instruction
void eval(int instr) {
    switch (instr) {
        case HLT: {
            running = false;
            printf("HLT\t;\n");
            break;
        }
        case PSH: {
            sp++;
            stack[sp] = program[++ip];  /* Get the argument of PSH */
            printf("PSH %d\t;\n", program[ip]);
            break;
        }
        case POP: {
            int val_popped = stack[sp--];   /* Get the last element from `stack` */
            printf("POP %d\t;\n", val_popped);
            break;
        }
        case ADD: {
            int a = stack[sp--];    /* Get the last element from `stack` */
            int b = stack[sp--];    /* Get the last but one element from `stack` */
            int result = b + a;     /* Add this elements */
            printf("ADD %d %d\t;\n", b, a);
            sp++;
            stack[sp] = result;
            break;
        }
    }
}


// Get the last instruction from program
int fetch() {
    return program[ip];
}


int main(void) {
    while (running) {
        eval(fetch());
        ip++;
    }
    return 0;
}
