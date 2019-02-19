#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "lispcal.h"

int main(void)
{
    if((stack.val=malloc(sizeof(double)*ALLOC_SIZE))==NULL)
        puts("RAM error!"),exit(1);
    stack.allocval=ALLOC_SIZE;
    if((stack.fn=malloc(sizeof(struct function)*ALLOC_SIZE))==NULL)
        puts("RAM error!"),free(stack.val),exit(1);
    stack.allocfn=ALLOC_SIZE;
    stack.lastvalue=0;
    puts("Lispcal: A calculator using a lisp-like syntax\n");
    for(;;)
    {
        fputs("\n>>> ",stdout);
        if(respond()) break;
        for(int i=0;i<stack.vallevel;i++) printf("=== %lf\n",stack.val[i]);
        if(stack.vallevel) stack.lastvalue=stack.val[stack.vallevel-1];
    }
    free(stack.fn);
    free(stack.val);
    return 0;
}

int respond(void)
{
    stack.vallevel=stack.fnlevel=0;
    int errorflag=0;
    for(char c=getchar();c!='\n'&&c!='\r'&&c!=EOF;c=getchar())
    {
        if(errorflag||c==' '||c=='('||c==')') continue;
        else if((c>='0'&&c<='9')||c=='.') pushstack(getnumber(c));
        else if((c>='a'&&c<='z')
                ||c=='+'||c=='-'||c=='*'||c=='/'||c=='^'||c=='%')
        {
            struct function newf=identify(c);
            if(newf.signature==0) errorflag=1;
            else if(newf.signature==1) return 1;
            else pushfunction(newf);
        }
        else printf("\nUnknown identifier: %c\n",c),errorflag=1;
    }
    if(errorflag||stack.fnlevel!=0)
        stack.vallevel=0,puts("\nUndefined behaviour!");
    return 0;
}

double getnumber(char c)
{
    double number,decimal;
    if(c=='.') number=0.0,decimal=0.1;
    else number=c-'0',decimal=1.0;
    for(c=getchar();c!=' '&&c!='('&&c!=')';c=getchar())
    {
        if(c=='\n'||c=='\r'||c==EOF)
        {
            ungetc(c,stdin);
            break;
        }
        else if(c>='0'&&c<='9')
        {
            if(decimal!=1.0) number+=(c-'0')*decimal,decimal/=10;
            else number=number*10+c-'0';
        }
        else if(c=='.'&&decimal==1.0) decimal=0.1;
        else
        {
            puts("\nWrongly formatted number!");
            break;
        }
    }
    return number;
}

int pushstack(double number)
{
    if(stack.vallevel==stack.allocval)
    {
        stack.allocval+=ALLOC_SIZE;
        stack.val=realloc(stack.val,sizeof(double)*stack.allocval);
        if(stack.val==NULL) puts("RAM error!"),free(stack.fn),exit(1);
    }
    stack.val[stack.vallevel++]=number;
    if(stack.fnlevel&&(--stack.fn[stack.fnlevel-1].nargs==0))
        invoke(stack.fn[--stack.fnlevel].signature);
    return 0;
}

int pushfunction(struct function newfn)
{
    if(newfn.nargs==0) return pushstack(invoke(newfn.signature));
    if(stack.fnlevel==stack.allocfn)
    {
        stack.allocfn+=ALLOC_SIZE;
        stack.fn=realloc(stack.fn,sizeof(struct function)*stack.allocfn);
        if(stack.fn==NULL) puts("RAM error!"),free(stack.val),exit(1);
    }
    stack.fn[stack.fnlevel++]=newfn;
    return 0;
}

inline char eq(char *s1, char *s2)
{
    for(;*s1;s1++,s2++) if(*s1!= *s2) return 0;
    return !(*s2);
}

struct function identify(char c)
{
    struct function fn={0,0};
    char buf[6];
    buf[5]='\0';
    buf[0]=c;
    for(int i=1;i<5;i++)
    {
        c=getchar();
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
        else if(c>='a'&&c<='z') buf[i]=c;
        else
        {
            buf[i]=c;
            buf[i+1]='\0';
            printf("\nUnknown identifier: %s\n",buf);
            return fn;
        }
    }
    if(eq(buf,"exit")||eq(buf,"quit")) fn.signature=1,fn.nargs=0;
    else if(eq(buf,"ans")||eq(buf,"ditto")) fn.signature=2,fn.nargs=0;
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
    else printf("\nUnknown identifier: %s\n",buf);
    return fn;
}

double invoke(unsigned sig)
{
    switch(sig)
    {
        case 2: return stack.lastvalue;
        case 9: stack.vallevel-=2;
                pushstack(stack.val[stack.vallevel]
                        +stack.val[stack.vallevel+1]);
                break;
        case 10: stack.vallevel-=2;
                pushstack(stack.val[stack.vallevel]
                        -stack.val[stack.vallevel+1]);
                break;
        case 11: stack.vallevel-=2;
                pushstack(stack.val[stack.vallevel]
                        *stack.val[stack.vallevel+1]);
                break;
        case 12: stack.vallevel-=2;
                pushstack(stack.val[stack.vallevel]
                        /stack.val[stack.vallevel+1]);
                break;
        case 13: stack.vallevel-=2;
                pushstack(pow(stack.val[stack.vallevel]
                            ,stack.val[stack.vallevel+1]));
                break;
        case 14: stack.vallevel-=2;
                pushstack(fmod(stack.val[stack.vallevel]
                            ,stack.val[stack.vallevel+1]));
                break;
        case 17: pushstack(sin(stack.val[--stack.vallevel])); break;
        case 18: pushstack(cos(stack.val[--stack.vallevel])); break;
        case 19: pushstack(tan(stack.val[--stack.vallevel])); break;
        case 20: pushstack(asin(stack.val[--stack.vallevel])); break;
        case 21: pushstack(acos(stack.val[--stack.vallevel])); break;
        case 22: pushstack(atan(stack.val[--stack.vallevel])); break;
        case 23: pushstack(sinh(stack.val[--stack.vallevel])); break;
        case 24: pushstack(cosh(stack.val[--stack.vallevel])); break;
        case 25: pushstack(tanh(stack.val[--stack.vallevel])); break;
        case 26: pushstack(exp(stack.val[--stack.vallevel])); break;
        case 27: pushstack(sqrt(stack.val[--stack.vallevel])); break;
        case 28: pushstack(log(stack.val[--stack.vallevel])); break;
        case 29: pushstack(log10(stack.val[--stack.vallevel])); break;
        case 30: pushstack(ceil(stack.val[--stack.vallevel])); break;
        case 31: pushstack(floor(stack.val[--stack.vallevel])); break;
        case 32: pushstack(fabs(stack.val[--stack.vallevel])); break;
        case 41: return 3.1415926535897932384626;
        case 42: return 2.7182818284590452353603;
        case 43: return 0.5772156649015328606065;
    }
    return 0;
}
