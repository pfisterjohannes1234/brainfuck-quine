#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

void die(const char *m)
  {
    fprintf
      (stderr,"Exit because of an error: %s",m);
     exit(1);
  }

void dieStrerror(const char *m)
  {
    fprintf
      (stderr,"Exit because of an error: %s: %s",m,strerror(errno));
     exit(1);
  }

void help(void)
  {
    fprintf
      (
        stderr,
        "Generate Brainfuck values to generate the file/data at input\n"
        "The generated Brainfuck code fills (zeored) cells with the data. "
        "If this data is printed in the brainfuck program, the output would be equal to the input file\n"
        " given to this program.\n\n"
        "Arguments:\n"
        "    -h | --help\n"
        "        Print this helptext\n"
        "    -i | --input <filename>\n"
        "        Input file to generate code for. Default is stdin\n"
        "    -o | --output <filename>\n"
        "        Output file to write brainfuck code to. Default is stdout\n"
        "    -n | --stepsize <value>\n"
        "        How for apart should the value cells be. A value greater than 1 means that there\n"
        "        are untouched cells between different bytes. Default is 1\n"
      );
  }

void printN(FILE *out, int c, size_t n)
  {
    for(size_t i=0;i<n;i++)
      {
        if(fputc(c,out)<0)
          {
            dieStrerror("Can not write to output");
          }
      }
  }

void generate(FILE *in, FILE *out, unsigned stepsize)
  {
    int c = 0;
    while( ( c=fgetc(in))!=EOF )
      {
        printN(out,'+',c);
        printN(out,'>',stepsize);
//        fputs("\n",out);
      }
  }

int main(int argc, char **argv)
  {
    int stepsize=1;
    FILE *input=NULL;
    FILE *output=NULL;
    for(int i=1;i<argc;i++)
      {
        char *arg = argv[i];
        if( !strcmp(arg,"--help") || !strcmp(arg,"-h") ) 
          {
            help();
            exit(1);
          }
        if( !strcmp(arg,"--input") || !strcmp(arg,"-i") ) 
          {
            if( input )
              { die("input already defined, use --help for more information"); }
            if( argc<i )
              { die("Missing argument after -i / --input"); }
            input = fopen(argv[i+1],"rb");
            if( !input )
              { dieStrerror("Can not open input file"); }
            arg++;
          }
        if( !strcmp(arg,"--output") || !strcmp(arg,"-o") ) 
          {
            if( output )
              { die("output already defined, use --help for more information"); }
            if( argc<i )
              { die("Missing argument after -o / --output"); }
            output = fopen(argv[i+1],"rbw");
            if( !input )
              { dieStrerror("Can not open output file"); }
            arg++;
          }
        if( !strcmp(arg,"--stepsize") || !strcmp(arg,"-n") ) 
          {
            if( argc<i )
              { die("Missing argument after -n / --stepsize"); }
            stepsize = atoi(argv[i+1]);
            if( stepsize<=0 )
              { dieStrerror("Can eitern not convert stepsize or stepsize was set to <=0 (has to be >0 )"); }
          }
      }

    generate( input?input:stdin , output?output:stdout, stepsize );
    return 0;
    
  }


