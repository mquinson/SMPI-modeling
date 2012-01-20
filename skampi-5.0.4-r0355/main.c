/* SKaMPI   MPI-Benchmark

Copyright 2003-2008 Werner Augustin, Lars Diesselberg
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

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "version.h"

#include "misc.h"
#include "output.h"
#include "debug.h"
#include "main.h"
#include "private_misc.h"

#include "exec.h"
#include "symboltable.h"
#include "synchronize.h"

extern void yyparse(void);
extern void init_lexer(char *infilename);
extern void finalize_lexer(void);

bool syntax_check_only;
bool parser_result;
int inside_measurement_block;
int just_entered_measurement_block;
int measurement_num;
SourceBuf source_buf;
bool log_source;

void print_version_info(void)
{
  printf(" This is SKaMPI, version %1d.%1d.%1d, revision %s.\n",
         skampi_maj_version, skampi_min_version, skampi_sub_release, svn_revision);
}

void print_help_info(void)
{
  printf("Usage: %s [-h] [-V] [-n] [-s] [-d debugflags] [-i inputfile] [-o outputfile]\n", 
	 get_progname());
  printf("\n");
  printf("Options:\n");
  printf("  -h        print this help information and exit\n");
  printf("  -V        print skampi version number and exit\n");
  printf("  -n        dry-run, syntax check only\n");
  printf("  -s        log measurement source in outputfile\n");
  printf("  -d flags  set debug flags, 0 is no debug information\n");
  printf("  -i file   set inputfile\n");
  printf("  -o file   set outputfile, if none given standard out is used\n");
  printf("\n");
}

int main(int argc, char* argv[]) 
{

  int arg_it;
  int dry_run;

  char *input_filename = NULL;
  int input_filename_len;
  char *output_filename = NULL;
  FILE *output_file;

  time_t t1, t2;

  set_progname(argv[0]);
  MPI_Init(&argc, &argv);
  init_globals();
  init_output();

  if( get_my_global_rank() == get_output_rank() ) {

    debug_flags = 0;
    dry_run = False;
    log_source = False;

    /* pre-processing the command line */
    for( arg_it = 1; arg_it < argc; arg_it++ ) 
      if( argv[arg_it][0] == '-' ) {
	switch( argv[arg_it][1] ) {
	case 'i': input_filename = argv[++arg_it]; break;
	case 'o': output_filename = argv[++arg_it]; break;
	case 'd': debug_flags = atoi(argv[++arg_it]); break;
	case 'n': dry_run = True; break;
	case 's': log_source = True; break;
	case 'V': print_version_info(); mpi_abort(1); break;
	case 'h': print_help_info(); mpi_abort(1); break;
  
	default:
	  error_without_abort("unknown option '%s'.\n\n", argv[arg_it]);
	  print_help_info();
	  mpi_abort(1);
	}
      } else {
	error_without_abort("unknown option '%s'.\n\n", argv[arg_it]);
	print_help_info(); 
	mpi_abort(1);
      }
    
    if( input_filename == NULL  ) {
      error_with_abort(2, "no input file specified, see skampi -h for help\n"); 
      mpi_abort(2);
    }

    input_filename_len = strlen(input_filename);
    
    if( output_filename != NULL ) {
      output_file = fopen(output_filename, "w");
      if( output_file == NULL ) 
	error_with_abort(4, "cannot open output file \"%s\".\n", 
			 output_filename); 
    } else 
      output_file = stdout;
    set_output_file(output_file);

    inside_measurement_block = False;
    syntax_check_only = True;

    init_lexer(input_filename);
    init_symboltable();

    source_buf.srcbuf_maxlen = 8192; /* will be increased dynamically when needed */
    source_buf.srcbuf = skampi_malloc_chars(source_buf.srcbuf_maxlen);
    source_buf.srcbuf_len = 0;
    source_buf.max_measurements = 64; /* this one, too */
    source_buf.measurement_pos = skampi_malloc_ints(source_buf.max_measurements);
    source_buf.num_measurements = 1;
    source_buf.measurement_pos[0] = 0;
    measurement_num = 0;

    yyparse();
    finalize_lexer();

    if( parser_result ) 
      debug(DBG_PARSER1, "syntax check successfull\n");
    else
      mpi_abort(4);

    /* clear_symboltable();  @@@ */

  }

  MPI_Bcast(&debug_flags, 1, MPI_INT, get_output_rank(), MPI_COMM_WORLD); /* @@@ optimize!!! @@@ */
  MPI_Bcast(&dry_run, 1, MPI_INT, get_output_rank(), MPI_COMM_WORLD);
  MPI_Bcast(&input_filename_len, 1, MPI_INT, get_output_rank(), MPI_COMM_WORLD); 
  if (get_my_global_rank() != get_output_rank()) {
    input_filename = skampi_malloc_chars(input_filename_len + 1);
  }
  MPI_Bcast(input_filename, input_filename_len+1, MPI_CHAR, get_output_rank(), MPI_COMM_WORLD);
  
  init_logging();
  init_debugging();
  init_synchronization_module();

  if( ! dry_run ) {

    inside_measurement_block = False;
    syntax_check_only = False;
    init_lexer(input_filename);
    init_symboltable();

    if( get_my_global_rank() == get_output_rank() ) {
      time(&t1);
      print_output("# SKaMPI Version %d.%d.%d rev. %s\n\n", 
		   skampi_maj_version, skampi_min_version, skampi_sub_release, svn_revision);
      print_output("# Started at %s\n\n", ctime(&t1));
    }

    measurement_num = 0;
    yyparse();
    finalize_lexer();

    if( get_my_global_rank() == get_output_rank() ) {
      time(&t2);
      print_output("\n# Finished at %s", ctime(&t2));
      print_output("# Total runtime %0.0f seconds\n\n", difftime(t2, t1));
    }
  }

  free(source_buf.srcbuf);
  free(source_buf.measurement_pos);

  /*  print_symboltable(); */
  /* print_global_time_differences();  @@ */
  finish_logging();
  MPI_Finalize();
  return 0;
}
