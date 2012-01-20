/* SKaMPI   MPI-Benchmark

Copyright 2003-2005 Werner Augustin
Lehrstuhl Informatik fuer Naturwissenschaftler und Ingenieure 
Fakultaet fuer Informatik
University of Karlsruhe

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA */



%option noyywrap nounput

%x comment incl

D			[0-9]
X                       [0-9a-fA-F]
L			[a-zA-Z_]
E			[Ee][+-]?{D}+
ID {L}({L}|{D})*
WS [ \t]
WSP [ \t]+

%{

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/stat.h>

#include "misc.h"
#include "main.h"
#include "parser.h"
#include "output.h"

#define MAX_INCLUDE_DEPTH 10
YY_BUFFER_STATE buffer_stack[MAX_INCLUDE_DEPTH];
char* filename_stack[MAX_INCLUDE_DEPTH];
int line_number_stack[MAX_INCLUDE_DEPTH];
int stack_ptr;

extern int current_line_number;
extern char* current_filename;
extern char* input_filename;
extern char* input_path;
extern int char_counter;

void store_source(int leng, char* text)
{
  if (syntax_check_only) {
    while (source_buf.srcbuf_maxlen < source_buf.srcbuf_len + leng) {
      source_buf.srcbuf = realloc(source_buf.srcbuf, source_buf.srcbuf_maxlen * 2);
      source_buf.srcbuf_maxlen *= 2;
    }
    memcpy(&source_buf.srcbuf[source_buf.srcbuf_len], text, leng);
    source_buf.srcbuf_len += leng;
  }
  char_counter += leng;
}

static char* include_filename(char* text)
{
  char* fname;
  int plen;
  int len = strlen(text);

  if (len < 3) return NULL;
  if (text[0] != '"' || text[len-1] != '"') return NULL;

  if (text[1] == '/') { /* absolute path */
    fname = skampi_malloc_chars(len+1-2);
    memcpy(fname, &text[1], len-2);
    fname[len-2] = 0x00;
  } else { /* path relative to input file */
    plen = strlen(input_path);
    fname = skampi_malloc_chars(len+1-2 + plen);
    memcpy(fname, input_path, plen);
    memcpy(&fname[plen], &text[1], len-2);
    fname[plen+len-2] = 0x00;
  }
  return fname;
}

static void enter_file(char *fname)
{
  char* buffer;
  int buffer_size;
  struct stat status;
  FILE* f;

  filename_stack[stack_ptr] = strdup(fname);
  line_number_stack[stack_ptr] = current_line_number;

  buffer = NULL;

  if (get_my_global_rank() == get_output_rank()) {
    if (stat(filename_stack[stack_ptr], &status) != 0)
      error_with_abort(3, "%s: cannot find input file\n", filename_stack[stack_ptr]);

    buffer_size = status.st_size + 2;
    buffer = skampi_malloc_chars(buffer_size);

    f = fopen(filename_stack[stack_ptr], "r");
    if (f == NULL ) 
      error_with_abort(3, "%s: cannot open input file\n", filename_stack[stack_ptr]);

    if (fread(buffer, 1, buffer_size-2, f) < buffer_size-2)
      error_with_abort(3, "%s: error while reading from input file\n", filename_stack[stack_ptr]);

    buffer[buffer_size-2] = '\0';
    buffer[buffer_size-1] = '\0';

    fclose(f);
  }

  if (!syntax_check_only) {
    MPI_Bcast(&buffer_size, 1, MPI_INT, get_output_rank(), MPI_COMM_WORLD);
    if (get_my_global_rank() != get_output_rank())
      buffer = skampi_malloc_chars(buffer_size);
    MPI_Bcast(buffer, buffer_size, MPI_CHAR, get_output_rank(), MPI_COMM_WORLD);
  }

  buffer_stack[stack_ptr] = yy_scan_buffer(buffer, buffer_size);
  current_filename = filename_stack[stack_ptr];
  current_line_number = 1;
  stack_ptr++;
}

/* store source stream while removing 'include' statements (rule 4,5,6) */
#define YY_USER_ACTION \
  if (yy_act != 4 && yy_act != 5 && yy_act != 6) \
    store_source(yyleng, yytext);

%}

%%

# { BEGIN(comment);}
<comment>.*$ { BEGIN(INITIAL); }
<comment><<EOF>> { BEGIN(INITIAL); }
<comment>.* { }

@include { BEGIN(incl); }
<incl>{WS}+ { }
<incl>\"[^\n\"]+\" {
  char* fname;
  if (stack_ptr >= MAX_INCLUDE_DEPTH) {
    error_with_abort(3, "Includes nested too deeply");
  }

  fname = include_filename(yytext);
  if (fname == NULL) {
    error_with_abort(3, "%s: Syntax error in include statement in line %d\n", current_filename,
                     current_line_number);
  }
  enter_file(fname);
  free(fname);

  BEGIN(INITIAL); 
}
<incl>. { error_with_abort(3, "%s: Syntax error in include statement in line %d\n", current_filename,
                           current_line_number); }

<<EOF>> { 
  stack_ptr--;
  yy_delete_buffer(buffer_stack[stack_ptr]);
  free(filename_stack[stack_ptr]);
  if (stack_ptr == 0) {
    yyterminate();
  } else {
    yy_switch_to_buffer(buffer_stack[stack_ptr-1]);
    current_line_number = line_number_stack[stack_ptr-1];
    current_filename = filename_stack[stack_ptr-1];
  }
}

"begin" { return(BEGIN_TOKEN); }
"end" { return(END); }
"measurement" { return(MEASUREMENT); }
"method" { return(METHOD); }
"diagram" { return(DIAGRAM); }
"result" { return(RESULT); }
"to" { return(TO); }
"step" { return(STEP); }
"print" { return(PRINT); }
"print_all" { return(PRINTALL); }
"flush" { return(FLUSH); }
"for" { return(FOR); }
"do" { return(DO); }
"od" { return(OD); }
"in" { return(IN); }
"measure" { return(MEASURE); }
"if" { return (IF); }
"then" { return (THEN); }
"else" { return (ELSE); }
"fi" { return (FI); }
"==" { return (EQ); }
"!=" { return (NEQ); }
"<>" { return (NEQ); }
"<=" { return (LE); }
">=" { return (GE); }
"and" { return (AND); }
"or" { return (OR); }
"not" { return (NOT); }
"..." { return (NOTLIMITED); }
"multipleof" { return (MULTIPLEOF); }
"autorefine" { return (AUTOREFINE); }

[<>,()=[\]+*-/{}~&|:%] { return yytext[0]; } 

 /* please optimize @@@ */
 \"[^\"]*\" { yylval.stringv = strdup(&(yytext[1])); yylval.stringv[yyleng-2] = 0; return STRING ; }

{ID} { yylval.identifier = strdup(yytext); return(IDENTIFIER); }

{D}+ { yylval.intv = atoi(yytext); return(INTEGER); }
(0x)?{X}+ { yylval.intv = strtol(yytext,NULL,0); return(INTEGER); }
{D}+"kb" { yylval.intv = atoi(yytext)*1024; return INTEGER; }
{D}+"kB" { yylval.intv = atoi(yytext)*1024; return INTEGER; }
{D}+"KiB" { yylval.intv = atoi(yytext)*1024; return INTEGER; }
{D}+"KB" { yylval.intv = atoi(yytext)*1024; return INTEGER; }
{D}+"mb" { yylval.intv = atoi(yytext)*1024*1024; return INTEGER; }
{D}+"MB" { yylval.intv = atoi(yytext)*1024*1024; return INTEGER; }
{D}+"MiB" { yylval.intv = atoi(yytext)*1024*1024; return INTEGER; }
{D}+"gb" { yylval.intv = atoi(yytext)*1024*1024*1024; return INTEGER; }
{D}+"GB" { yylval.intv = atoi(yytext)*1024*1024*1024; return INTEGER; }
{D}+"GiB" { yylval.intv = atoi(yytext)*1024*1024*1024; return INTEGER; }


{D}*"."{D}+({E})? { yylval.doublev = atof(yytext); return(DOUBLE); }
{D}+"."{D}*({E})? { yylval.doublev = atof(yytext); return(DOUBLE); }  

{WS}+ { } 
[\n] { current_line_number++; } 

. { return ILLEGALCHAR; }

%%



/* initialize lexer */

void init_lexer(char *infilename)
{
  int pos;

  stack_ptr = 0;
  char_counter = 0;
  parser_result = True;

  input_filename = infilename;
  pos = strlen(input_filename) - 1;
  for (pos = strlen(input_filename) - 1; pos >= 0; pos--)
    if (input_filename[pos] == '/') break;
  input_path = skampi_malloc_chars(pos+2);
  memcpy(input_path, input_filename, pos+1);
  input_path[pos+1] = 0x00;

  enter_file(infilename);
}

void finalize_lexer(void)
{
  free(input_path);
}
