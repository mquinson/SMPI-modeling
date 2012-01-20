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

extern int debug_flags;

enum {

  DBG_TEMP    = 1,
  DBG_PARSER1 = 2,
  DBG_PARSER2 = 4,
  DBG_MEAS    = 8,
  DBG_EXEC    = 16,
  DBG_BUFFER  = 32,
  DBG_SYMBOLT = 64,
  DBG_SYNC    = 128,
  DBG_TIMEDIFF = 256,
  DBG_CHECK    = 512,
  DBG_AUTOREFINE = 1024
};


#define DEBUG(flags,A) { if( flags & debug_flags ) { A }; };

void init_debugging(void);
void debug(int level, char *fmt, ...);
void rdebug(int level, char *fmt, ...);
void iodebug(int level, char *fmt, ...);

void init_logging(void);
void finish_logging(void);
void logging(int level, char *fmt, ...);

