#ifndef LISPCAL_H
#define LISPCAL_H

#define ALLOC_SIZE 8

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
} stack;

void exiting(void);

void printstack(void);

double invoke(unsigned sig);

double getnumber(char first);

int pushstack(double number);

int pushfunction(struct function newfn);

inline char eq(char *s1, char *s2);

struct function identify(char first);

double respond(char *flagptr);

#endif
