#include "lispcal.h"

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <math.h>

int main(void)
{
    stack.val=malloc(sizeof(double)*ALLOC_SIZE);
    stack.allocval=ALLOC_SIZE;
    stack.fn=malloc(sizeof(struct function)*ALLOC_SIZE);
    stack.allocfn=ALLOC_SIZE;
    atexit(exiting);
    if(stack.val==NULL||stack.fn==NULL) throwallocerror,exit(1);
    char current=1;
    puts("\rLispcal: A calculator using a lisp-like syntax\n");
    while(current)
        fputs("CAL> ",stdout),printf("= %lf\n\n",respond(&current));
    return 0;
}

double respond(char *flagptr)
{
    char c;
    stack.vallevel=stack.fnlevel=0;
    while((c=getchar())!='\n'&&c!='\r'&&c!=EOF)
    {
        if(c==' '||c=='('||c==')') continue;
        else if((c>='0'&&c<='9')||c=='.')
        {
            if(pushstack(getnumber(c)))
                throwallocerror,exit(1);
        }
        else if(isalpha(c)||c=='+'||c=='-'||c=='*'||c=='/'||c=='^'||c=='%')
        {
            struct function newf=identify(c);
            if(newf.signature==0)
            {
                throwidenterror;
                return 0;
            }
            else if(newf.signature==1)
            {
                *flagptr=0;
                return 0;
            }
            else
            {
                if(pushfunction(newf)) throwallocerror,exit(1);
            }
        }
        else
        {
            throwidenterror;
            return 0;
        }
    }
    if(stack.vallevel!=1||stack.fnlevel!=0)
        throwundeferror;
    return stack.val[0];
}

void exiting(void)
{
    free(stack.val);
    free(stack.fn);
}

double getnumber(char first)
{
    double number=(first-'0')*1.0;
    int flag=1;
    int decimal=0;
    while(flag)
    {
        first=getchar();
        if(first=='\n'||first=='\r'||first==EOF)
        {
            ungetc(first,stdin);
            break;
        }
        else if(first==' '||first=='('||first==')') break;
        else if(first>='0'&&first<='9')
        {
            if(decimal)
            {
                double k=(first-'0')*1.0;
                for(int i=decimal;i;--i)
                    k/=10;
                number+=k;
                ++decimal;
            }
            else number=number*10+1.0*(first-'0');
        }
        else if(first=='.') decimal=1;
        else
        {
            throwidenterror;
            break;
        }
    }
    return number;
}

int pushstack(double number)
{
    //check if memory space is enough
    if(stack.vallevel==stack.allocval)
    {
        stack.allocval+=ALLOC_SIZE;
        stack.val=realloc(stack.val,sizeof(double)*stack.allocval);
        if(stack.val==NULL) exit(1);
    }
    //push the number
    stack.val[stack.vallevel]=number;
    stack.vallevel++;
    //check if some function should be returned
    if(--stack.fn[stack.fnlevel-1].nargs==0)
        invoke(stack.fn[--stack.fnlevel].signature);
    return 0;
}

int pushfunction(struct function newfn)
{
    //check if the function takes no argument
    if(newfn.nargs==0)
        return pushstack(invoke(newfn.signature));
    //check if space needed to be alloc
    if(stack.fnlevel==stack.allocfn)
    {
        stack.allocfn+=ALLOC_SIZE;
        stack.fn=realloc(stack.fn,sizeof(struct function)*stack.allocfn);
    }
    //push the function
    stack.fn[stack.fnlevel]=newfn;
    stack.fnlevel++;
    return 0;
}

inline char eq(char *s1, char *s2)
{
    for(;*s1;s1++,s2++)
    {
        if(*s1!= *s2) return 0;
    }
    return !(*s2);
}

struct function identify(char first)
{
    struct function fn={0,0};
    char buf[6];
    buf[5]='\0';
    buf[0]=first;
    for(int i=1;i<5;i++)
    {
        char c=getchar();
        if(c=='\n'||c=='\r'||c==EOF)
        {
            ungetc(c,stdin);
            buf[i]='\0';
            break;
        }
        else if(c==' '||c=='('||c==')')
        {
            buf[i]='\0';
            break;
        }
        else if(isalpha(c)) buf[i]=c;
        else return fn;
    }
    if(eq(buf,"exit")||eq(buf,"quit")) fn.signature=1,fn.nargs=0;
    else if(eq(buf,"+")||eq(buf,"add")||eq(buf,"plus"))
        fn.signature=9,fn.nargs=2;
    else if(eq(buf,"-")||eq(buf,"minus")) fn.signature=10,fn.nargs=2;
    else if(eq(buf,"*")||eq(buf,"times")||eq(buf,"mult"))
        fn.signature=11,fn.nargs=2;
    else if(eq(buf,"/")||eq(buf,"div")) fn.signature=12,fn.nargs=2;
    else if(eq(buf,"^")||eq(buf,"pow")) fn.signature=13,fn.nargs=2;
    else if(eq(buf,"%")||eq(buf,"mod")) fn.signature=14,fn.nargs=2;
    else if(eq(buf,"sin")) fn.signature=17,fn.nargs=1;
    else if(eq(buf,"cos")) fn.signature=18,fn.nargs=1;
    else if(eq(buf,"tan")) fn.signature=19,fn.nargs=1;
    else if(eq(buf,"asin")) fn.signature=20,fn.nargs=1;
    else if(eq(buf,"acos")) fn.signature=21,fn.nargs=1;
    else if(eq(buf,"atan")) fn.signature=22,fn.nargs=1;
    else if(eq(buf,"sinh")) fn.signature=23,fn.nargs=1;
    else if(eq(buf,"cosh")) fn.signature=24,fn.nargs=1;
    else if(eq(buf,"tanh")) fn.signature=25,fn.nargs=1;
    else if(eq(buf,"exp")) fn.signature=26,fn.nargs=1;
    else if(eq(buf,"sqrt")) fn.signature=27,fn.nargs=1;
    else if(eq(buf,"ln")) fn.signature=28,fn.nargs=1;
    else if(eq(buf,"log")) fn.signature=29,fn.nargs=1;
    else if(eq(buf,"ceil")) fn.signature=30,fn.nargs=1;
    else if(eq(buf,"floor")) fn.signature=31,fn.nargs=1;
    else if(eq(buf,"abs")) fn.signature=32,fn.nargs=1;
    else if(eq(buf,"pi")) fn.signature=41,fn.nargs=0;
    else if(eq(buf,"e")) fn.signature=42,fn.nargs=0;
    else if(eq(buf,"gamma")) fn.signature=43,fn.nargs=0;
    return fn;
}

double invoke(unsigned sig)
{
    double temp;
    switch(sig)
    {
        case 9:
            temp=stack.val[stack.vallevel-2]+stack.val[stack.vallevel-1];
            stack.vallevel-=2;
            pushstack(temp);
            break;
        case 10:
            temp=stack.val[stack.vallevel-2]-stack.val[stack.vallevel-1];
            stack.vallevel-=2;
            pushstack(temp);
            break;
        case 11:
            temp=stack.val[stack.vallevel-2]*stack.val[stack.vallevel-1];
            stack.vallevel-=2;
            pushstack(temp);
            break;
        case 12:
            temp=stack.val[stack.vallevel-2]/stack.val[stack.vallevel-1];
            stack.vallevel-=2;
            pushstack(temp);
            break;
        case 13:
            temp=pow(stack.val[stack.vallevel-2],
                    stack.val[stack.vallevel-1]);
            stack.vallevel-=2;
            pushstack(temp);
            break;
        case 14:
            temp=fmod(stack.val[stack.vallevel-2],
                    stack.val[stack.vallevel-1]);
            stack.vallevel-=2;
            pushstack(temp);
            break;
        case 17:
            temp=sin(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 18:
            temp=cos(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 19:
            temp=tan(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 20:
            temp=asin(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 21:
            temp=acos(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 22:
            temp=atan(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 23:
            temp=sinh(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 24:
            temp=cosh(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 25:
            temp=tanh(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 26:
            temp=exp(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 27:
            temp=sqrt(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 28:
            temp=log(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 29:
            temp=log10(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 30:
            temp=ceil(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 31:
            temp=floor(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 32:
            temp=fabs(stack.val[--stack.vallevel]);
            pushstack(temp);
            break;
        case 41:
            return 3.1415926535897932384626;
        case 42:
            return 2.7182818284590452353603;
        case 43:
            return 0.5772156649015328606065;
    }
    return 0;
}
