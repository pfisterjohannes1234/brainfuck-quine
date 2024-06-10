//We split the (infinite) array into blocks. Each block can handle a single code character.
// It is used like a C struct See further down for a description of the 8 cells in a block
#define STEP 4

#define STEP_N(M) M M M M

//Set this to 1 when we want to generate code that is easier to convert
//Set this to 0 when we want to have normal C code with the possibility to debug
#ifndef GENERATE_SIMPLE
  #define GENERATE_SIMPLE 0
#endif //GENERATE_SIMPLE

#define TESTCODE 1

#if GENERATE_SIMPLE

#define DEBUG 0
#define DEBUG_ADDRESS 0

#else //!GENERATE_SIMPLE

#include <unistd.h>
#include <execinfo.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>


//#define SIZE (1024*1024*16)
#define SIZE (1024*1024*16)
#define START 80
#define START_EXTRA 480

int p=START+START_EXTRA;




//#define write_char putchar

#define DEBUG 0
#define DEBUG_ADDRESS 1


unsigned char data[SIZE];

void write_char(unsigned char data)
  {
    #if DEBUG
    if( data>=0x20 && data<0x7F )
      {
        printf("%3u 0x%02X '%c'\n",data,data,data);
      }
    else
      {
        printf("%3u 0x%02X\n",data,data);
      }
    #else // DEBUG
      putchar(data);
    #endif // DEBUG
  }


void printContent(unsigned char *data, size_t n, unsigned offset)
  {
    fprintf(stderr,"D: %2u ",offset);
    for(size_t i=0;i<n;i++)
      {
        unsigned char c = data[i*STEP+offset+START_EXTRA];
        unsigned char p = isprint(c) ? c : ' ';
        fprintf(stderr,"%c_%02X  ", p, c );
      }
    fprintf(stderr,"\n");
  }

void printData(unsigned char *data, size_t n)
  {
    printContent(data,n,2);
  }

void printCode(unsigned char *data, size_t n)
  {
    printContent(data,n,0);
  }

unsigned char read_char()
  {
    int i = getchar();
    if(i<0)
      {
        return 0;
      }
    return i;
  }

void *symbol_array[32];
void catch_pagefault(int signal, siginfo_t *i, void *ignored)
  {
    (void)signal;
    (void)ignored;
    char buffer[4096];
    write
      (
        STDERR_FILENO,
        buffer,
        snprintf
          (
            buffer,
            sizeof buffer,
            "ERROR %p %p %i %i %i %i\n",
            i->si_lower,
            i->si_upper,
            p,
            signal,
            i->si_signo,
            i->si_errno
          )
      );
    int r = backtrace(symbol_array,32);
    backtrace_symbols_fd(symbol_array,r,STDERR_FILENO);
    exit(EXIT_FAILURE);
  }


#define move_from(N)  N; N=0;
#define moveAdd_from(N)  N; N=0;
#define copyAdd_from(N)  N;

#endif // !GENERATE_SIMPLE

//OS means offset
#define OS_CHAR       0 //Which code character we generate or print
#define OS_TEMP       1 //Temporary cell to copy data, used in convert.py and not here. Don't touch this
#define OS_COUNER     2 //Store how many times we need to print something
#define OS_DATA       3 //Print a printing character



//Goes To the zell 0
#define GO_0()                                                   \
  while( data[p-STEP+OS_GESETZT] )                               \
    {                                                            \
      p = p-STEP;                                                \
    }                                                            \


//Goes To the zell 0 and carry the ADD value of OS_ADDIERE with
#define GO_0_ADD()                                               \
  while( data[p-STEP+OS_GESETZT] )                               \
    {                                                            \
      data[p+OS_ADDIERE-STEP] = moveAdd_from(data[p+OS_ADDIERE]);\
      p = p-STEP;                                                \
    }                                                            \

//Goes OS_ADDIERE block to the right. Loses the value in OS_ADDIERE
#define GO_RIGTH_ADD()                                           \
  while( data[p+OS_ADDIERE] )                                    \
    {                                                            \
      p = p+STEP;                                                \
      data[p+OS_ADDIERE]   = moveAdd_from(data[p+OS_ADDIERE-STEP]);\
      data[p+OS_GESETZT]   = data[p+OS_GESETZT]+1;               \
      data[p+OS_ADDIERE]   = data[p+OS_ADDIERE] - 1;             \
    }                                                            \

//Goes to the currently used zell
#define GO_RIGTH_CURRENT()                                       \
  while( data[p+OS_HIER] )                                       \
    { p = p+STEP; }                                              \

//Goes To the zell 0 and carry the ADD value of OS_ADDIERE with
#define GO_0_CLEAR()                                             \
  while( data[p-STEP+OS_GESETZT] )                               \
    {                                                            \
      data[p+OS_HIER] = data[p+OS_HIER] - 1;                     \
      p = p-STEP;                                                \
    }                                                            \



#if !GENERATE_SIMPLE
int main()
  {
#endif
    #if !GENERATE_SIMPLE
      memset(data,0,sizeof data);
      #if TESTCODE
        const char *testdata="<<<<[<<<<]>>>>[.>>>>]";
        for(size_t i=0;i<strlen(testdata);i++)
          {
            data[p+OS_CHAR]=testdata[i];
            p=p+STEP;
          }
        p = p-STEP;
        while(data[p+OS_CHAR])
          { p = p-STEP; }
        p = p+STEP;
      #endif // !TESTCODE
    #endif // GENERATE_SIMPLE
    #if DEBUG_ADDRESS
      fprintf(stderr,"DATA START %p END %p\n",(void*)data,(void*)(data+sizeof data));
      struct sigaction a;
      memset(&a,0,sizeof a);
      a.sa_sigaction=catch_pagefault;
      a.sa_flags=SA_RESETHAND|SA_SIGINFO;
      sigaction(SIGSEGV,&a,NULL);
    #endif // !GENERATE_SIMPLE

    while(data[p+OS_CHAR])
      { p = p-STEP; }
    p = p+STEP;



    while(data[p+OS_CHAR])
      {
        data[p+OS_DATA] = data[p+OS_DATA] + '>';
        STEP_N( write_char(data[p+OS_DATA]); )

        data[p+OS_DATA] += '+' - '>';
        data[p+OS_COUNER] += data[p+OS_CHAR];
        while(data[p+OS_COUNER])
          {
            write_char(data[p+OS_DATA]);
            data[p+OS_COUNER]--;
          }
        p=p+STEP;
      }

    p = p-STEP;
    while(data[p+OS_CHAR])
      { p = p-STEP; }
    p = p+STEP;


    while(data[p+OS_CHAR])
      {
        write_char(data[p+OS_CHAR]);
        p = p+STEP;
      }



    #if DEBUG
      fprintf(stderr,"Q       %*u\n",(p-START_EXTRA)*6/8,p);
      printContent(data,38,0);
      printContent(data,38,2);
      printContent(data,38,4);
      printContent(data,38,5);
    #endif
#if !GENERATE_SIMPLE
  }
#endif // !GENERATE_SIMPLE



