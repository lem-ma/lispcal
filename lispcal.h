#ifndef LISPCAL_H
#define LISPCAL_H

#define ALLOC_SIZE 16

#define throwallocerror puts("RAM error!")
#define throwidenterror puts("Unknown identifier!")
#define throwundeferror puts("Undefined behaviour!")

struct function
{
    unsigned signature;
    unsigned nargs;
};

struct stack_structure
{
    double *val;
    size_t vallevel;
    size_t allocval;
    struct function *fn;
    size_t fnlevel;
    size_t allocfn;
    double lastvalue;
} stack;

void printstack(void);

double invoke(unsigned);

double getnumber(char);

int pushstack(double);

int pushfunction(struct function);

struct function identify(char);

double respond(char*);

#endif
