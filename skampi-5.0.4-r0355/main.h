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

extern bool syntax_check_only;

extern bool parser_result;

extern FILE *output_file;

extern int inside_measurement_block;

extern int just_entered_measurement_block;

extern int measurement_num;

extern bool log_source;

typedef struct {
	int srcbuf_len;
	char* srcbuf;
	int srcbuf_maxlen;
	int* measurement_pos;
	int max_measurements;
	int num_measurements;
} SourceBuf;

extern SourceBuf source_buf;

