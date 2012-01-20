/* SKaMPI   MPI-Benchmark

Copyright 2003-2008 Werner Augustin
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

#include <stdarg.h>
#include <stdio.h>
#include <mpi.h>
#include <unistd.h>

#include "misc.h"
#include "private_misc.h"
#include "output.h"
#include "debug.h"

int debug_flags;

void init_debugging(void)
{
  char name[MPI_MAX_PROCESSOR_NAME];
  int name_len, pid;

  if( debug_flags ) {
    MPI_Get_processor_name(name, &name_len);
    name[name_len] = '\0';
    pid = getpid();

    fprintf(stderr, "#proc %d: pid=%d name='%s'\n", 
	    get_my_global_rank(), pid, name);
  }
}

void debug(int flags, char *fmt, ...)
{
  va_list args;
  
  if( flags & debug_flags ) {
    va_start(args, fmt);
    fprintf(stderr, "#proc %d: ", get_my_global_rank());
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
}

void rdebug(int flags, char *fmt, ...)
{
  va_list args;
  
  if( get_my_global_rank() == 0 && (flags & debug_flags) ) {
    va_start(args, fmt);
    fprintf(stderr, "#root: ");
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
}

void iodebug(int flags, char *fmt, ...)
{
  va_list args;
  
  if( get_my_global_rank() == get_output_rank() && (flags & debug_flags) ) {
    va_start(args, fmt);
    fprintf(stderr, "#io: ");
    vfprintf(stderr, fmt, args);
    va_end(args);
  }
}

/*---------------------------------------------------------------------------*/

static char logfile_name[] = "proc000.log";
static FILE *logfile;
static bool used_logfile = False;

void init_logging(void)
{
  if( debug_flags > 0 ) {
    sprintf(logfile_name, "proc%04d.log", get_my_global_rank());
    logfile = fopen(logfile_name, "w");
    if( logfile == NULL )
      error_with_abort(11, "couldn't open logfile \"%s\"", logfile_name);
  }
}

void finish_logging(void)
{
  if( debug_flags > 0 ) fclose(logfile);
  if( !used_logfile ) remove(logfile_name);
  
}

void logging(int flags, char *fmt, ...)
{
  va_list args;

  if( flags & debug_flags ) {
    used_logfile = True;
    va_start(args, fmt);
    vfprintf(logfile, fmt, args);
    va_end(args);
    fflush(logfile);
  }
}
