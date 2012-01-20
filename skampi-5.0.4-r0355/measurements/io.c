/* SKaMPI   MPI-Benchmark

Copyright 2006-2008 Argirios Perogiannakis
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

/* ************************************************************************* */

#include <mpi.h>
#include "../mpiversiontest.h"

#ifdef USE_MPI_IO

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <dirent.h>
#include <errno.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "../misc.h"
#include "../mem.h"
#include "../synchronize.h"
#include "../output.h"
#include "../exec.h"

#pragma weak begin_skampi_extensions

/* number of MPI_BARRIERs before starting measurement */
#define N_BARRIER 1
#define FILENAME  "skampi_io"

#define FILE_DELETE(fn)							\
  {									\
    MPI_File fh;							\
    assert(io_fh == MPI_FILE_NULL);					\
    MPI_File_open(get_measurement_comm(), fn, MPI_MODE_RDWR |		\
		  MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE,		\
		  io_info, &fh);					\
    MPI_File_close(&fh);						\
  }

#define MAX_INT(a,b) ((a) >= (b)) ? (a) : (b) 
#define MIN_INT(a,b) ((a) <= (b)) ? (a) : (b)

/* ------------------------------------------------------------------------*/
/* Global variables                                                        */
/* ------------------------------------------------------------------------*/

static char     *io_working_dir;
static char     *io_filename;
static char     *io_data_representation = "native";
static int      io_preallocate=0;    /* force preallocation of file space  */
static int      io_atomicity=0;      /* use atomic mode if needed          */
static int      io_unique_open=0;    /* use MPI_MODE_UNIQUE_OPEN           */
static int      io_file_sync=1;      /* sync files before closing file     */
static MPI_Info io_info=MPI_INFO_NULL;
static MPI_File io_fh;

static MPI_Comm io_comm2=MPI_COMM_NULL; /* used in MPI_IO_write_and_read   */

static int n_seek = 0;               /* used in MPI_IO_file_seek           */
static int r_seek = 0;               /* and MPI_IO_file_seek_shared        */

static struct datatype_param {
  MPI_Datatype memtype;
  MPI_Datatype etype;
  MPI_Datatype filetype;
  int memtype_elem;              /* number of elements in datatype typemap */
  int etype_elem;
  int filetype_elem;
  double elem_ratio;              /* MPI-2, 9.4.1 Positioning              */
} io_dtype;

enum data_access {
  IO_EXPLICIT_OFFSETS         = 0x0001,   /* positioning */
  IO_INDIVIDUAL_FILE_POINTERS = 0x0002,
  IO_SHARED_FILE_POINTER      = 0x0004,
  IO_BLOCKING                 = 0x0010,   /* synchronism */
  IO_NONBLOCKING_SPLIT        = 0x0020,
  IO_NONCOLLECTIVE            = 0x0100,   /* coordination */
  IO_COLLECTIVE               = 0x0200
};


#ifdef IO_CHECK
static long io_chk_ref;
static int  io_chk_msg;
#endif

/* ------------------------------------------------------------------------*/
/* Declaration of "Utility" functions                                      */
/* ------------------------------------------------------------------------*/

#ifdef IO_CHECK
unsigned int  get_random_seed(void);
unsigned char get_random_byte(void);
long buffer_fill(unsigned char *, int, MPI_Datatype);
void io_chk_add_sum(unsigned char *, int, MPI_Datatype, long *);
void io_chk_compare(long, const char *, int); 
#endif

/* ------------------------------------------------------------------------*/
/* "Skampi input file" functions                                           */
/* ------------------------------------------------------------------------*/

/* *************************************************************************
 * set_io_working_dir
 * 
 * @description
 * Sets the working directory where the benchmark will be performed.
 *
 * This function is intended to be used in the 'SKaMPI input file' to set
 * the global working directory. It should be called by all processes.
 * 
 * @parameter
 * *cwd                  string with path to working directory
 *
 * @return
 * void
 * *************************************************************************
 */
void func_set_io_working_dir(char *cwd)
{  
  size_t cwdlen;
  DIR    *dir;

  /* Check if working directory exists.
    (opendir, closedir should be conform to POSIX.1-2001) */
  dir = opendir(cwd);
  if (dir == NULL) {
    error_with_abort(MPI_ERR_BAD_FILE,
		     "\nset_io_working_dir: set_io_working_dir(cwd=%s) failed."
		     "\nset_io_working_dir: %s (%d)\n",
		     cwd, strerror(errno), errno );
  }
  assert( closedir(dir) == 0);

  if (io_working_dir) {
    mpi_free(io_working_dir);
  }
  cwdlen = strlen(cwd);
  io_working_dir = strcpy(mpi_malloc_chars(cwdlen), cwd);  
}


/* *************************************************************************
 * get_io_working_dir
 *
 * @description
 * Returns the current directory name in which skampi will perform the
 * benchmark tests. 
 * Default: the value of the environment variable HOME.
 * 
 * @parameter
 * void
 *
 * @return
 * char *           (string with path to working directory)
 * *************************************************************************
 */
char *func_get_io_working_dir(void)
{  
  if (!io_working_dir) {

    func_set_io_working_dir(getenv("HOME"));
    /* io_working_dir = getenv("HOME"); */
  }

  return io_working_dir;
}


/* *************************************************************************
 * set_io_datarep
 * 
 * @description
 * Sets the data representation of a file. 
 * MPI defines therefore the strings "native" (no conversion between memory
 * and file), "internal" (implementation dependend conversion), and
 * "external32" (standardized conversion to satisfy interoperability).
 * 
 * For user-defined data representation you can choose an arbitrary identifier
 * (not yet supported by SKaMPI).
 *
 * This function is intended to be used in the 'SKaMPI input file' to set
 * a global data representation.
 *
 * @parameter
 * *datarep              data representation identifier
 *
 * @return
 * void
 * *************************************************************************
 */
void func_set_io_datarep(char *datarep)
{
  if (strcmp(datarep, "native") &&
      strcmp(datarep, "internal") &&
      strcmp(datarep, "external32")) {
    
    if (get_my_global_rank() == get_output_rank()) {
      error_with_abort(MPI_ERR_ARG, 
		       "\nUser-defined data representation ('%s') is not "
		       "supported."
		       "\nUse 'native', 'internal', or 'external32'.\n", 
		       datarep);
    }
  }

  io_data_representation = datarep;
}


/* *************************************************************************
 * get_io_datarep
 *
 * @description
 * Returns the global data representation identifier.
 * 
 * @parameter
 * void
 *
 * @return
 * char *
 * *************************************************************************
 */
char *func_get_io_datarep(void)
{
  if (!io_data_representation || 
      (strcmp(io_data_representation, "internal") && 
       strcmp(io_data_representation, "external32"))) {
    io_data_representation = "native";
  }

  return io_data_representation;
}


/* *************************************************************************
 * set_io_atomicity
 *
 * @description
 * Directs a measurement function to turn on/off atomic mode, 
 * if it supports both modes.
 * 
 * @parameter
 * flag                  turn on/off atomic mode, if true/false
 * 
 * @return
 * void
 * *************************************************************************
 */
void func_set_io_atomicity(int flag)
{
  io_atomicity = flag;
}

/* *************************************************************************
 * get_io_atomicity
 *
 * @parameter
 * void
 * 
 * @return
 * int
 * *************************************************************************
 */
int func_get_io_atomicity(void)
{
  return io_atomicity;
}


/* *************************************************************************
 * set_io_preallocation
 *
 * @description
 * If flag is TRUE, i.e. not 0, then the measurement function tries to 
 * preallocate storage space before it performs any write operation.
 * 
 * @parameter
 * flag                  Forces preallocation of file space, if true
 *
 * @return
 * void
 * *************************************************************************
 */
void func_set_io_preallocation(int flag)
{
  io_preallocate = flag;
}

/* *************************************************************************
 * get_io_preallocation
 *
 * @parameter
 * void
 *
 * @return
 * int
 * *************************************************************************
 */
int func_get_io_preallocation(void)
{
  return io_preallocate;
}

/* *************************************************************************
 * set_io_unique_open
 *
 * @description
 * If flag is TRUE, i.e. not 0, then the files are opened with the access
 * mode MPI_MODE_UNIQUE_OPEN.
 * 
 * @parameter
 * flag                  Open a file in access mode MPI_MODE_UNIQUE_OPEN
 *
 * @return
 * void
 * *************************************************************************
 */
void func_set_io_unique_open(int flag)
{
  io_unique_open = flag;
}

/* *************************************************************************
 * get_io_unique_open
 *
 * @parameter
 * void
 *
 * @return
 * int
 * *************************************************************************
 */
int func_get_io_unique_open(void)
{
  return io_unique_open;
}

/* *************************************************************************
 * set_io_file_sync
 *
 * @description
 * If flag is TRUE, i.e. not 0, then MPI_File_sync is called before
 * closing the file. Also the time for performing MPI_File_sync is included
 * in the measurement.
 * 
 * @parameter
 * flag                  Perform a MPI_File_sync before closing file.
 *
 * @return
 * void
 * *************************************************************************
 */
void func_set_io_file_sync(int flag)
{
  io_file_sync = flag;
}
/* *************************************************************************
 * get_io_file_sync
 *
 * @parameter
 * void
 *
 * @return
 * int
 * *************************************************************************
 */
int func_get_io_file_sync(void)
{
  return io_file_sync;
}

/* *************************************************************************
 * print_data_access_settings
 * 
 * @description
 * Print out current settings for data access 
 * (see MPI-2, 9.4.1 Data Access Routines)
 *
 * @parameter
 * daccess               data access routine class
 * flag                  print data access routine
 *                       1 (read routine), 
 *                       2 (write routine)
 *                       3 (read & write routine)
 *
 * @return
 * void
 *
 * *************************************************************************
 */
void func_print_data_access_settings(int daccess, int flag)
{
  if (get_my_global_rank() == get_output_rank()) {
    
    print_output("#\n# data access routine (0x%x, %d):\n", daccess, daccess);
    
    /* determine positioning */
    switch (daccess & 0x007) {
      
    case IO_EXPLICIT_OFFSETS :
      print_output("#    positioning:  explicit offsets\n");
      break;
    case IO_INDIVIDUAL_FILE_POINTERS :
      print_output("#    positioning:  individual file pointers\n");
      break;
    case IO_SHARED_FILE_POINTER :
      print_output("#    positioning:  shared file pointer\n");
      break;
    case 0 :
      print_output("#    positioning:  no preset found\n");
      break;
    default :
      print_output("#    positioning:  ambiguous preset (0x00%x)\n", 
		   (daccess & 0x007));
      break;
    }
    
    /* determine synchronism */
    switch (daccess & 0x030) {
    case IO_BLOCKING :
      print_output("#    synchronism:  blocking\n");
      break;
    case IO_NONBLOCKING_SPLIT :
      print_output("#    synchronism:  nonblocking or split collective\n");
      break;
    case 0 :
      print_output("#    synchronism:  no preset found\n");
      break;
    default :
      print_output("#    synchronism:  ambiguous preset (0x0%x)\n", 
		   (daccess & 0x030));
      break;
    }
    
    /* determine coordination */
    switch (daccess & 0x300) {
    case IO_NONCOLLECTIVE :
      print_output("#    coordination: noncollective\n");
      break;
    case IO_COLLECTIVE :
      print_output("#    coordination: collective\n");
      break;
    case 0 :
      print_output("#    coordination: no preset found\n");
      break;
    default :
      print_output("#    coordination: ambiguous preset (0x%x)\n", 
		   (daccess & 0x300));
      break;
    }

    if (daccess == (IO_EXPLICIT_OFFSETS |
		    IO_BLOCKING |
		    IO_NONCOLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_READ_AT\n");
      if (flag & 2) print_output("# => MPI_FILE_WRITE_AT\n");
    } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			   IO_BLOCKING |
			   IO_NONCOLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_READ\n");
      if (flag & 2) print_output("# => MPI_FILE_WRITE\n");
    } else if (daccess == (IO_SHARED_FILE_POINTER |
			   IO_BLOCKING |
			   IO_NONCOLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_READ_SHARED\n");
      if (flag & 2) print_output("# => MPI_FILE_WRITE_SHARED\n");
    } else if (daccess == (IO_EXPLICIT_OFFSETS |
			   IO_NONBLOCKING_SPLIT |
			   IO_NONCOLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_IREAD_AT\n");
      if (flag & 2) print_output("# => MPI_FILE_IWRITE_AT\n");
    } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			   IO_NONBLOCKING_SPLIT |
			   IO_NONCOLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_IREAD\n");
      if (flag & 2) print_output("# => MPI_FILE_IWRITE\n");
    } else if (daccess == (IO_SHARED_FILE_POINTER |
			   IO_NONBLOCKING_SPLIT |
			   IO_NONCOLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_IREAD_SHARED\n");
      if (flag & 2) print_output("# => MPI_FILE_IWRITE_SHARED\n");
    } else if (daccess == (IO_EXPLICIT_OFFSETS |
			   IO_BLOCKING |
			   IO_COLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_READ_AT_ALL\n");
      if (flag & 2) print_output("# => MPI_FILE_WRITE_AT_ALL\n");
    } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			   IO_BLOCKING |
			   IO_COLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_READ_ALL\n");
      if (flag & 2) print_output("# => MPI_FILE_WRITE_ALL\n");
    } else if (daccess == (IO_SHARED_FILE_POINTER |
			   IO_BLOCKING |
			   IO_COLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_READ_ORDERED\n");
      if (flag & 2) print_output("# => MPI_FILE_WRITE_ORDERED\n");
    } else if (daccess == (IO_EXPLICIT_OFFSETS |
			   IO_NONBLOCKING_SPLIT |
			   IO_COLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_READ_AT_ALL_BEGIN, "
				 "MPI_FILE_READ_AT_ALL_END\n");
      if (flag & 2) print_output("# => MPI_FILE_WRITE_AT_ALL_BEGIN, "
				 "MPI_FILE_WRITE_AT_ALL_END\n");
    } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			   IO_NONBLOCKING_SPLIT |
			   IO_COLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_READ_ALL_BEGIN, "
				 "MPI_FILE_READ_ALL_END\n");
      if (flag & 2) print_output("# => MPI_FILE_WRITE_ALL_BEGIN, "
				 "MPI_FILE_WRITE_ALL_END\n");
    } else if (daccess == (IO_SHARED_FILE_POINTER |
			   IO_NONBLOCKING_SPLIT |
			   IO_COLLECTIVE)) {
      if (flag & 1) print_output("# => MPI_FILE_READ_ORDERED_BEGIN, "
				 "MPI_FILE_READ_ORDERED_END\n");
      if (flag & 2) print_output("# => MPI_FILE_WRITE_ORDERED_BEGIN, "
				 "MPI_FILE_WRITE_ORDERED_END\n");
    }

    print_output("#\n");
  }
}


/* *************************************************************************
 * data_access_routines (iterator)
 *
 * @description
 * Iterates through all data access routine classes.
 *
 * @parameter
 * **self                runtime environment variable
 * pattern               limit iteration to data access routine classes
 *                       that are embedded in `pattern'
 *
 * @return
 * int                   data access routine identifier
 * *************************************************************************
 */
int iterator_data_access_routines(void **self, int pattern)
{
  int daccess[12] = {0x111, 0x112, 0x114, 0x121, 0x122, 0x124,
		     0x211, 0x212, 0x214, 0x221, 0x222, 0x224};
  int *counter;
  
  if (*self == NULL) {
    counter = mpi_malloc_ints(2);
    *self = counter;
    *counter = -1;
    *(counter+1) = MPI_Wtime();
  } else {
    counter = *self;
    if (get_my_global_rank() == get_output_rank()) {
      print_output("# duration %.2f sec\n", MPI_Wtime()-*(counter+1));
    }
    *(counter+1) = MPI_Wtime();
  }

  (*counter)++;

  while ((*counter < 12) && ((pattern & daccess[*counter]) != daccess[*counter]))
  {
    (*counter)++;
  }

  if (*counter == 12) {
    *self = NULL;
    mpi_free(counter);
    return 42;
  } 

  return daccess[*counter];
}


/* *************************************************************************
 * print_file_info
 *
 * @description
 * Print the (key,value) pairs associated with the current file system.
 *
 * @parameter
 * void
 *
 * @return
 * void
 * *************************************************************************
 */
void func_print_file_info(void)
{
  MPI_File fh;
  MPI_Info finfo;
  int      i, nkeys;
  char     key[MPI_MAX_INFO_KEY];
  int      valuelen, flag;
  char     *value;
  char     *filename;
  char     *cwd;

  cwd = func_get_io_working_dir();
  filename = mpi_malloc_chars(strlen(cwd)+11);
  sprintf(filename, "%s/skampi_io", cwd);
  
  MPI_File_open(MPI_COMM_WORLD, filename, 
		MPI_MODE_RDWR | MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE, 
		io_info, &fh);
  MPI_File_get_info(fh, &finfo);
  MPI_Info_get_nkeys(finfo, &nkeys);
  if (get_my_global_rank() == get_output_rank()) {
    print_output("#P[%d] File hints:\n", get_output_rank());
  }
  for (i=0; i<nkeys; ++i) {
    MPI_Info_get_nthkey(finfo, i, key);
    MPI_Info_get_valuelen(finfo, key, &valuelen, &flag);
    if (flag) {
      value = mpi_malloc_chars(valuelen+1);
      MPI_Info_get(finfo, key, valuelen, value, &flag);
      if (flag) {	  
	value[valuelen] = '\0';
	if (get_my_global_rank() == get_output_rank()) {
	  print_output("#P[%d]    %s = %s\n", get_output_rank(), key, value);
	}
      }
      mpi_free(value);
    }
  }
  MPI_File_close(&fh);
  
  mpi_free(filename);
}

/* *************************************************************************
 * set_file_info
 *
 * @description
 * Adds a new (key,value) pair to `io_info', that is used to update
 * the values for the hints of the current file system. Keys that are not
 * known to the file system handler will be ignored.
 * 
 * *************************************************************************
 */
void func_set_file_info(char *key, char *value)
{
  if ( io_info == MPI_INFO_NULL ) {
    MPI_Info_create(&io_info);
  }
  MPI_Info_set(io_info, key, value);
}

void func_reset_file_info(void)
{
  if ( io_info != MPI_INFO_NULL ) {
    MPI_Info_free(&io_info);
  }
  assert( io_info == MPI_INFO_NULL );
}

/*---------------------------------------------------------------------------*/

/* *************************************************************************
 * int_array_allgather
 *
 * @description
 * Useful when using measurement function MPI_IO_write_random/
 * MPI_IO_read_random. Must be called by all processes in communicator c.
 * Returns a list of all values by using MPI_Allgather.
 * 
 * @parameter
 * c                     Communicator
 * v                     integer value
 *
 * @return
 * IArray
 * *************************************************************************
 */
IArray func_int_array_allgather(MPI_Comm c, int v)
{
  int c_size;
  IArray a;
  
  MPI_Comm_size(c, &c_size);
  
  a.n = c_size;
  a.v = calloc(c_size, sizeof(int));
  
  MPI_Allgather(&v, 1, MPI_INT, a.v, 1, MPI_INT, c);

  return a;
}

/* ------------------------------------------------------------------------*/
/* Utility functions                                                       */
/* ------------------------------------------------------------------------*/

/* *************************************************************************
 * get_error_message [intern]
 *
 * @description
 * Returns an error message.
 * (The user has to free the memory occupied by the returned string.)
 *
 * @parameter
 * errorcode             error code return be MPI function
 *
 * @return
 * char *                error message
 * *************************************************************************
 */
char *get_error_message(int errorcode)
{  
  char *errorstring;
  int  resultlen;
  int  errorclass;

  errorstring = mpi_malloc_chars(2*MPI_MAX_ERROR_STRING);
  MPI_Error_string(errorcode, errorstring, &resultlen);
  MPI_Error_class(errorcode, &errorclass);

  sprintf(errorstring, "%s (%d, %d)", errorstring, errorclass, errorcode);

  return errorstring;
}


/* *************************************************************************
 * get_io_filename
 *
 * @description
 * Creates a unique name of the file, that will be accessed by the processes. 
 * The filename has the form '<name>_<group>-<id>', where <id> is a random
 * value.
 *
 * This function is collective over the processes in get_measurement_comm().
 * 
 * @parameter
 * name                  name of file to open (without path)
 * group                 integer value to differentiate filenames
 *                       e.g. `group' could be the global rank of the process
 *
 * @return
 * char *                filename with full path
 * *************************************************************************
 */
char *get_io_filename(char *name, int group)
{
  char   *fn;
  char   *cwd;
  char   _name[]="skampi_io";
  int    flen;
  int    clen;
  double time;
  int    id = 0;
  struct stat status;

  if (get_measurement_rank() == 0) {
    time = MPI_Wtime();
    time = time - (int) time;
    srand( ((unsigned int) (time*100000.0)) % UINT_MAX );
    id = ((int) ((INT_MAX-1)*((double) rand()/((double) RAND_MAX+1.0))));
  }

  MPI_Bcast(&id, 1, MPI_INT, 0, get_measurement_comm());  

  cwd = func_get_io_working_dir();
  if (name != NULL) {
    flen = strlen(name);
  } else {
    flen = strlen(_name);
  }

  clen = (int) ceil(log((double) INT_MAX)/log(10.0));
  
  fn = mpi_malloc_chars(strlen(cwd)+flen+2*clen+8);

  if (name != NULL) {
    sprintf(fn, "%s/%s_%d-%d", cwd, name, group, id);
  } else {
    sprintf(fn, "%s/%s_%d-%d", cwd, _name, group, id);
  }
  
  if (stat(fn, &status) == 0) { /* File found and is accessible */
    mpi_free(fn);
    fprintf(stderr, "# Proc %d: file %s exists, generating new filename\n", 
	    get_measurement_rank(), fn);
    fflush(stderr);
    fn = get_io_filename(name, group); /* get new `id' */
  } else {
    if (errno != ENOENT) {
      error_with_abort( MPI_ERR_BAD_FILE,
	     "\nget_io_filename: get_io_filename(name=%s, group=%d) failed."
	     "\nget_io_filename: %s (%d)\n",
	     name, group, strerror(errno), errno );
    }
  }
  
  MPI_Barrier(get_measurement_comm());
  return fn;
}


/* *************************************************************************
 * get_datatype_elements [intern]
 *
 * @description
 * Determines the number of elements in a datatype, i.e. the size of the
 * type signature, typesig, or type ap (see MPI-1 Derived datatypes).
 *
 * @parameter
 * datatype              basic/derived datatype
 *
 * @return
 * int
 * *************************************************************************
 */
int get_datatype_elements(MPI_Datatype datatype)
{
  void *sbuf, *rbuf;
  int elements;
  MPI_Status status;

  sbuf = mpi_malloc(get_extent(1, datatype));
  rbuf = mpi_malloc(get_extent(1, datatype));

  MPI_Sendrecv(sbuf, 1, datatype, 0, 23, 
	       rbuf, 1, datatype, 0, 23, MPI_COMM_SELF, &status);
  MPI_Get_elements(&status, datatype, &elements);
  
  mpi_free(sbuf);
  mpi_free(rbuf);
  return elements;
}


/* *************************************************************************
 * datatype_param_init
 * 
 * @description
 * Initial calculation of datatype parameter.
 *
 * @parameter
 * *dp                   
 * memtype               datatype of memory buffer
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 *
 * @return 
 * void
 * *************************************************************************
 */
void datatype_param_init(struct datatype_param *dp, MPI_Datatype memtype, 
			 MPI_Datatype etype, MPI_Datatype filetype)
{
  if (dp != NULL) {
    dp->memtype         = memtype;
    dp->etype           = etype;
    dp->filetype        = filetype;
    dp->memtype_elem    = get_datatype_elements(memtype);
    dp->etype_elem      = get_datatype_elements(etype);
    dp->filetype_elem   = get_datatype_elements(filetype);
    dp->elem_ratio = ((double) dp->memtype_elem) / ((double) dp->etype_elem);
  } else {
    error_with_abort(MPI_ERR_ARG,
		     "\nInvalid argument in offset_init_calculation()\n");
  }
}


/* *************************************************************************
 * get_offset
 * 
 * @description
 * Determines the number of offsets.
 *
 * @parameter
 * dp
 * size                  number of memory data
 *
 * @return
 * MPI_Offset            number of offsets
 * *************************************************************************
 */
MPI_Offset get_offset(struct datatype_param dp, int size)
{
  return (MPI_Offset) ceil(dp.elem_ratio * size);
  /* FIXME: external32 is not working as expected under MPICH2, version 1.0.3,
            see also MPI-2, 9.5.2, p. 250*/
}


/* *************************************************************************
 * preallocate_file_space
 *
 * @description
 * Forces allocation of file space to store memory data.
 * Before using this function you have to set the file view 
 * in the caller function, otherwise the result is erroneous.
 * 
 * @parameter
 * fh                    file handle
 * dp
 * size                  number of memory data
 * *************************************************************************
 */
void preallocate_file_space(MPI_File fh, struct datatype_param dp,
			    MPI_Offset size)
{
  MPI_Offset num_offset;
  MPI_Offset new_fsz;
  MPI_Offset cur_fsz;
  MPI_Aint   extent;   
  int        n;
  
  num_offset = get_offset(dp, size);
  MPI_File_get_type_extent(fh, dp.filetype, &extent);  

  /* MPI-2, 9.3 File Views: filetype_elem = n * etype_elem,
     i.e. filetype is a multiple of etype */
  n = dp.filetype_elem/dp.etype_elem; 
  
  new_fsz = extent * ceil((double) num_offset/(double) n);

  MPI_File_get_size(fh, &cur_fsz);
  
 /* Preallocation doesn't work properly on the HP XC6000 Cluster.
  * If the file exists, as it does when IO_CHECK is defined, the
  * preallocate routine adds the specified file space to the file.
  * But it should only increase it to the given file size or leave it 
  * unchanged. Therefor we check, if the new size of the file is
  * greater than the current size.
  */
  if (new_fsz > cur_fsz) { 
    /* The Barrier is necessary because the MPICH2 implementation 
       (version 1.0.3) has a bug, when using MPI_File_preallocate:
       
       The MPI-routine is collective and only process 0 changes the
       file size. But the changing is done immediately. And because of
       that behaviour and the if-statement above, you get a deadlock.
    */
    MPI_Barrier(get_measurement_comm());
    MPI_File_preallocate(fh, new_fsz);
    MPI_File_sync(fh);
  }
}


/* *************************************************************************
 * send_buffer_init
 * 
 * @description
 * Sets the size of the send buffer and the reported messages size.
 *
 * @parameter
 * count                 size of memory buffer
 * datatype              type of data in memory buffer
 *
 * @return
 * void
 * *************************************************************************
 */
void send_buffer_init(int count, MPI_Datatype datatype)
{
  int      buffer_usage;
  int      type_size;
  MPI_Aint sum;

  buffer_usage = get_extent( count, datatype );
  set_send_buffer_usage( buffer_usage );

  MPI_Type_size( datatype, &type_size );
  MPI_Allreduce( &count, &sum, 1, MPI_INT, MPI_SUM, get_measurement_comm() );

  set_reported_message_size( type_size * sum );
}


/* *************************************************************************
 * recv_buffer_init
 * 
 * @description
 * Sets the size of the receive buffer and the reported messages size.
 *
 * @parameter
 * count                 size of memory buffer
 * datatype              type of data in memory buffer
 *
 * @return
 * void
 * *************************************************************************
 */
void recv_buffer_init(int count, MPI_Datatype datatype)
{
  int      buffer_usage;
  int      type_size;
  MPI_Aint sum;

  buffer_usage = get_extent( count, datatype );
  set_recv_buffer_usage( buffer_usage );

  MPI_Type_size( datatype, &type_size );
  MPI_Allreduce( &count, &sum, 1, MPI_INT, MPI_SUM, get_measurement_comm() );

  set_reported_message_size( type_size * sum );
}

 

/* *************************************************************************
 * calc_offsets [intern]
 *
 * @description
 * Calculates the offsets where a process accesses to the file.
 * This function is collective, i.e. it must be called by all processes.
 * 
 * (Used in MPI_IO_write_random and MPI_IO_read_random)
 * 
 * @parameter
 * dp             
 * memsize               size of memory buffer
 * blocklen              chunk of memory buffer transfered in one call
 * numblocks             length of return value (OUT)
 * 
 * @return
 * MPI_Offset *          offsets 
 * *************************************************************************
 */
MPI_Offset *calc_offsets(struct datatype_param dp, 
			 IArray memsize, IArray blocklen, 
			 int *numblocks)
{
  MPI_Offset *offsets;
  MPI_Offset begin;
  int        myrank, numprocs;
  int        *nb;
  int        i, j;
  int        r;

  assert(numblocks != NULL);

  myrank = get_measurement_rank();
  numprocs = get_measurement_size();

  nb = mpi_malloc_ints(numprocs);
  *numblocks = 0;  /* maximum number of blocks */
  for (i=0; i<numprocs; ++i) {
    nb[i] = (int) ceil((double) memsize.v[i]/(double) blocklen.v[i]);
    *numblocks = MAX_INT(*numblocks, nb[i]);
  }

  offsets = mpi_malloc_offsets(*numblocks);

  /* calculate offsets of blocks */
  begin = (MPI_Offset)0;

  for (i=0; i<*numblocks; ++i) {      

    offsets[i] = begin;
    for (j=0; j<myrank; ++j) {
      if (i < nb[j]-1) {
	/* add offset of blocklen */
	offsets[i] += get_offset(dp, blocklen.v[j]); 
      } else if (i == nb[j]-1) {
	r = memsize.v[j] % blocklen.v[j];
	offsets[i] += get_offset(dp, (r == 0) ? blocklen.v[j] : r);
      }
      /* else don't change offsets */
    }
    
    begin = offsets[i];
    for (j=myrank; j<numprocs; ++j) {
      if (i < nb[j]-1) {
	begin += get_offset(dp, blocklen.v[j]);
      } else if (i == nb[j]-1) {
	r = memsize.v[j] % blocklen.v[j];
	begin += get_offset(dp, (r == 0) ? blocklen.v[j] : r);
      }
    }
  }
  
  mpi_free(nb);
  return offsets;
}


/* *************************************************************************
 * is_equal
 * 
 * @description
 * Checks, if `val' has the same value for all processes invoking this
 * function.
 *
 * @parameter
 * val                   value that is compared
 * comm                  communicator
 *
 * @return
 * int                   1 (all values a the same)
 *                       0 (at least one value differs from others)
 * *************************************************************************
 */

typedef struct {
  int v, r;
} OpType;

void equalOP(void *invec, void *inoutvec, int *len, MPI_Datatype *datatype)
{
  int i;
  OpType *in    = (OpType*)invec;
  OpType *inout = (OpType*)inoutvec;

  
  for (i=0; i<*len; ++i) {
    inout->r = (inout->r) && (in->v == inout->v);
    ++in;
    ++inout;
  }
}

int is_equal(int val)
{
  MPI_Datatype odt;
  OpType ot, res;
  MPI_Op myOP;

  ot.v = val;
  ot.r = 1;

  MPI_Type_contiguous( 2, MPI_INT, &odt );
  MPI_Type_commit( &odt );

  MPI_Op_create( equalOP, 1, &myOP );
  MPI_Reduce( &ot, &res, 1, odt, myOP, 0, get_measurement_comm() );
  MPI_Bcast( &(res.r), 1, MPI_INT, 0, get_measurement_comm() );
  MPI_Op_free( &myOP );

  MPI_Type_free( &odt );

  return res.r;
}





/* ------------------------------------------------------------------------*/
/* Measurement functions                                                   */
/* ------------------------------------------------------------------------*/

 /* *************************************************************************
 * MPI_IO_open
 *
 * @description
 * Measures the time to open a file
 * 
 * If `create_file' is set then this function creates a new file, everytime
 * measure_MPI_IO_open is called. This is done by appending MPI_MODE_CREATE 
 * and MPI_MODE_DELETE_ON_CLOSE to the access mode.
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            -
 * memory size:          -
 * block size:           -
 * access modes:         MPI_MODE_RDWR
 * memory datatype:      -
 * etype:                -
 * filetype:             -
 * positioning:          -
 * synchronism:          -
 * coordination:         -
 *
 * @parameter
 * create_file           create a new file for measurement
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_open(int create_file)
{
  MPI_File fh;

  io_filename = get_io_filename(FILENAME, 0);
  init_synchronization();

  if (!create_file) {
    MPI_File_open(get_measurement_comm(), io_filename, 
		  MPI_MODE_RDWR | MPI_MODE_CREATE, io_info, &fh);
    MPI_File_close(&fh);
  }
}

double measure_MPI_IO_open(int create_file)
{
  int    i;
  double start_time, end_time;
  int    amode;

  amode = MPI_MODE_RDWR;
  if (create_file) amode = amode | MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization(); 
  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);
  end_time = stop_synchronization();
  MPI_File_close(&io_fh);

  return (end_time - start_time);
}

void finalize_MPI_IO_open(int create_file) 
{
  if (!create_file) {
    FILE_DELETE(io_filename);
  }
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_close
 *
 * @description
 * Measures the time to close a file. 
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            -
 * memory size:          -
 * block size:           -
 * access modes:         MPI_MODE_RDWR
 * memory datatype:      MPI_BYTE
 * etype:                MPI_BYTE
 * filetype:             MPI_BYTE
 * positioning:          -
 * synchronism:          -
 * coordination:         -
 *
 * @parameter
 * count                 size of memory buffer
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_close(int count)
{  
  MPI_Status status;
  char       *send_buffer;

  assert(count >= 0);

  set_send_buffer_usage( get_extent( count, MPI_BYTE ) );
  io_filename = get_io_filename(FILENAME, 0);

  if (get_measurement_rank() == 0) {
    send_buffer = mpi_malloc_chars( get_extent(count, MPI_BYTE) );

    MPI_File_open(MPI_COMM_SELF, io_filename, 
		  MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN, 
		  io_info, &io_fh);
    MPI_File_write(io_fh, send_buffer, count, MPI_BYTE, &status);
    MPI_File_close(&io_fh);
    mpi_free(send_buffer);
  }

  MPI_Barrier(get_measurement_comm());
  init_synchronization();
}

double measure_MPI_IO_close(int count)
{
  double start_time, end_time;
  int    amode;

  amode = MPI_MODE_RDWR;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);

  start_time = start_synchronization(); 
  MPI_File_close(&io_fh);
  end_time = stop_synchronization();

  return (end_time - start_time);
}

void finalize_MPI_IO_close(int count)
{
  FILE_DELETE(io_filename);
  mpi_free(io_filename);
}

/* *************************************************************************
 * MPI_IO_open_close
 *
 * @description
 * Measures the time to open, write a byte and close a file.
 * (analoguous to Intels MPI Benchmark, version 2.3)
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            -
 * memory size:          -
 * block size:           -
 * access modes:         MPI_MODE_RDWR | MPI_MODE_CREATE  or
 *                       MPI_MODE_RDWR
 * memory datatype:      -
 * etype:                MPI_BYTE
 * filetype:             MPI_BYTE
 * positioning:          -
 * synchronism:          -
 * coordination:         -
 *
 * @parameter
 * create_flag           1 (when open a file, create a new one)
 *                       0 (open existing file)
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_open_close(int create_flag)
{  
  MPI_Status status;
  char       buf = 42;

  set_send_buffer_usage( get_extent( 1, MPI_BYTE ) );
  io_filename = get_io_filename(FILENAME, 0);

  if (!create_flag) {
    MPI_File_open(get_measurement_comm(), io_filename, 
		  MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN, 
		  io_info, &io_fh);
    
    MPI_File_write(io_fh, &buf, 1, MPI_BYTE, &status);
    MPI_File_close(&io_fh);
  }

  MPI_Barrier(get_measurement_comm());
  init_synchronization();
}

double measure_MPI_IO_open_close(int create_flag)
{
  double     start_time, end_time;
  int        amode;
  MPI_Status status;

  if (create_flag) {
    amode = MPI_MODE_RDWR | MPI_MODE_CREATE;
  } else {
    amode = MPI_MODE_RDWR;
  }
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  start_time = start_synchronization(); 
  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, MPI_BYTE, MPI_BYTE,
		    func_get_io_datarep(), io_info);  
  MPI_File_set_atomicity(io_fh, io_atomicity);

  MPI_File_write(io_fh, get_send_buffer(), 1, MPI_BYTE, &status);
  MPI_File_close(&io_fh);
  end_time = stop_synchronization();

  if (create_flag) {
    FILE_DELETE(io_filename);
  }

  return (end_time - start_time);
}

void finalize_MPI_IO_open_close(int create_flag) 
{
  if (!create_flag) {
    FILE_DELETE(io_filename);
  }
  mpi_free(io_filename);
}

 
/* *************************************************************************
 * MPI_IO_delete
 *
 * @description
 * Measures the time to delete a file of specified size.
 *
 * FIXME: This measurement function doesn't work for small files,
 * because the file may be still in the cache, when the delete function is
 * invoked. Possible solution: create the files, that are going to be 
 * deleted, in the init-function.
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            -
 * memory size:          -
 * block size:           -
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE | 
 *                       MPI_MODE_UNIQUE_OPEN
 * memory datatype:      MPI_BYTE
 * etype:                MPI_BYTE
 * filetype:             MPI_BYTE
 * positioning:          -
 * synchronism:          -
 * coordination:         -
 *
 * @parameter
 * count                 size of memory buffer
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_delete(int count)
{  
  assert(count >= 0);
  set_send_buffer_usage( get_extent( count, MPI_BYTE ) );
  io_filename = get_io_filename(FILENAME, 0);
}

double measure_MPI_IO_delete(int count)
{
  MPI_Status status;
  double     start_time, end_time;

  if (get_measurement_rank() == 0) {
    MPI_File_open(MPI_COMM_SELF, io_filename, 
		  MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN, 
		  io_info, &io_fh);
    MPI_File_write(io_fh, get_send_buffer(), count, MPI_BYTE, &status);
    MPI_File_close(&io_fh);
  } 

  if (get_measurement_rank() == 0) {
    start_time = MPI_Wtime();
    MPI_File_delete(io_filename, io_info);
    end_time = MPI_Wtime();
  } else {
    start_time = 1.0;
    end_time   = 0.0;
  }

  return (end_time - start_time);
}

void finalize_MPI_IO_delete(int count)
{
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_delete_on_close
 *
 * @description
 * Measures the time to delete a file of specified size.
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            -
 * memory size:          -
 * block size:           -
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE
 *                       MPI_DELETE_ON_CLOSE
 * memory datatype:      MPI_BYTE
 * etype:                MPI_BYTE
 * filetype:             MPI_BYTE
 * positioning:          -
 * synchronism:          -
 * coordination:         -
 *
 * @parameter
 * count                 size of memory buffer
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_delete_on_close(int count)
{  
  assert(count >= 0);
  set_send_buffer_usage( get_extent( count, MPI_BYTE ) );
  io_filename = get_io_filename(FILENAME, 0);
  init_synchronization();
}

double measure_MPI_IO_delete_on_close(int count)
{
  MPI_Status status;
  double     start_time, end_time;

  MPI_File_open(get_measurement_comm(), io_filename, 
		MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE,
		io_info, &io_fh);

  if (get_measurement_rank() == 0) {
    MPI_File_write(io_fh, get_send_buffer(), count, MPI_BYTE, &status);
  }
  MPI_File_sync(io_fh);

  start_time = start_synchronization(); 
  MPI_File_close(&io_fh);
  end_time = stop_synchronization();

  return (end_time - start_time);
}

void finalize_MPI_IO_delete_on_close(int count)
{
  mpi_free(io_filename);
}

/* *************************************************************************
 * MPI_IO_file_seek
 *
 * @description
 * Measures the time to perform a MPI_File_seek operation in which the 
 * 'whence' parameter is set to MPI_SEEK_SET.
 * 
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            -
 * memory size:          -
 * block size:           -
 * access modes:         MPI_MODE_RDWR | MPI_MODE_CREATE | 
 *                       MPI_DELETE_ON_CLOSE
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous or noncontiguous
 * positioning:          -
 * synchronism:          -
 * coordination:         -
 *
 * @parameter
 * memsize               size of memory buffer
 * blocklen              number of elements of type memtype that will
 *                       be overjumped
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_file_seek(int memsize, int blocklen, 
			   MPI_Datatype memtype, MPI_Datatype etype, 
			   MPI_Datatype filetype)
{
  int amode;

  assert(memsize > 0);
  assert(blocklen > 0);

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  set_reported_message_size(0);

  amode = MPI_MODE_RDWR | MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);  
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		    func_get_io_datarep(), io_info);

  preallocate_file_space(io_fh, io_dtype, get_measurement_size()*memsize);
  
  n_seek = 0;
  r_seek = 0;

  MPI_Barrier(get_measurement_comm());
  init_synchronization();
}

double measure_MPI_IO_file_seek(int memsize, int blocklen, 
				MPI_Datatype memtype, MPI_Datatype etype, 
				MPI_Datatype filetype)
{
  double     start_time, end_time;
  int        myrank, numprocs;
  MPI_Offset offset;
  int        position;

  myrank   = get_measurement_rank();
  numprocs = get_measurement_size();
  position = ((myrank+r_seek)%numprocs)*memsize+n_seek*blocklen;

  n_seek += 1;
  if (n_seek > (memsize/blocklen)) {
    n_seek = 0;            
    r_seek = (r_seek+1)%numprocs;
  }

  offset = get_offset(io_dtype, position);

  start_time = start_synchronization();
  MPI_File_seek(io_fh, offset, MPI_SEEK_SET);
  end_time = stop_synchronization();
		
  return (end_time - start_time);
}

void finalize_MPI_IO_file_seek(int memsize, int blocklen, 
			       MPI_Datatype memtype, MPI_Datatype etype, 
			       MPI_Datatype filetype)
{
  MPI_File_close(&io_fh);
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_file_seek_shared
 *
 * @description
 * Measures the time to perform a MPI_File_seek_shared operation in which
 * the 'whence' parameter is set to MPI_SEEK_SET.
 * 
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            -
 * memory size:          -
 * block size:           -
 * access modes:         MPI_MODE_RDWR | MPI_MODE_CREATE | 
 *                       MPI_DELETE_ON_CLOSE
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous or noncontiguous
 * positioning:          -
 * synchronism:          -
 * coordination:         -
 *
 * @parameter
 * memsize               size of memory buffer                  
 * blocklen              number of elements of type memtype that will
 *                       be overjumped
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_file_seek_shared(int memsize, int blocklen, 
				  MPI_Datatype memtype, MPI_Datatype etype, 
				  MPI_Datatype filetype)
{
  int amode;

  assert(memsize > 0);
  assert(blocklen > 0);
#ifdef IO_CHECK
  assert(is_equal(memsize));
  assert(is_equal(blocklen));
#endif

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  set_reported_message_size(0);

  amode = MPI_MODE_RDWR | MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		    func_get_io_datarep(), io_info);

  preallocate_file_space(io_fh, io_dtype, memsize);

  n_seek = 0;

  MPI_Barrier(get_measurement_comm());
  init_synchronization();
}

double measure_MPI_IO_file_seek_shared(int memsize, int blocklen, 
				       MPI_Datatype memtype, MPI_Datatype etype,
				       MPI_Datatype filetype)
{
  double     start_time, end_time;
  MPI_Offset offset;
  int        position;

  position = n_seek*blocklen;
  n_seek = (n_seek+1)%(memsize/blocklen);

  offset = get_offset(io_dtype, position);

  start_time = start_synchronization();
  MPI_File_seek_shared(io_fh, offset, MPI_SEEK_SET);
  end_time = stop_synchronization();
		
  return (end_time - start_time);
}

void finalize_MPI_IO_file_seek_shared(int memsize, int blocklen, 
				      MPI_Datatype memtype, MPI_Datatype etype,
				      MPI_Datatype filetype)
{
  MPI_File_close(&io_fh);
  mpi_free(io_filename);
}

/* *************************************************************************
 * MPI_IO_preallocate
 *
 * @description
 * Measures the time to preallocate file space.
 * 
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            count
 * memory size:          count
 * block size:           count
 * access modes:         MPI_MODE_RDWR | MPI_MODE_CREATE | 
 *                       MPI_DELETE_ON_CLOSE
 * memory datatype:      MPI_BYTE
 * etype:                MPI_BYTE
 * filetype:             MPI_BYTE
 * positioning:          -
 * synchronism:          -
 * coordination:         -
 *
 * @parameter
 * count                 size of memory buffer                  
 * with_sync             if true then measure a MPI_File_sync, too
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_preallocate(int count, int with_sync)
{
  assert(count > 0);

  io_filename = get_io_filename(FILENAME, 0);

  init_synchronization();
}

double measure_MPI_IO_preallocate(int count, int with_sync)
{
  double start_time, end_time;
  int    amode;

  amode = MPI_MODE_RDWR | MPI_MODE_CREATE | MPI_MODE_DELETE_ON_CLOSE;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);

  start_time = start_synchronization();
  MPI_File_preallocate(io_fh, count);
  if (with_sync) MPI_File_sync(io_fh);
  end_time = stop_synchronization();

  MPI_File_close(&io_fh);
  
  return (end_time - start_time);
}

void finalize_MPI_IO_preallocate(int count, int with_sync)
{
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_write_segmented
 *
 * @description
 * Write to a file using a segmentation pattern, i.e. each process 
 * writes the data in his own segment of the file.
 *
 * For data access routines that use explicit offsets or individual file
 * pointers no other communication between the processes is needed to
 * schedule them. Data access routines using shared file pointer produce 
 * extra communication in calling the collective MPI_FILE_SEEK_SHARED 
 * routine and MPI_BARRIER. 
 *
 * For data access routines with shared file pointer and collective 
 * coordination the performance is poor in this pattern, due to the fact 
 * that all processes have to call the I/O operations, even if only one 
 * process has something to write. DON'T USE THESE ROUTINES.
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            n*<memsize>
 * memory size:          <memsize>
 * block size:           <blocklen>
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE | 
 *                       MPI_MODE_DELETE_ON_CLOSE
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of memory buffer, that is written to the file
 * stride                number of elements between start of segments
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_write_segmented(int memsize, int blocklen, int stride, 
				 MPI_Datatype memtype, MPI_Datatype etype, 
				 MPI_Datatype filetype, int daccess)
{    
  assert(memsize > 0);
  assert(blocklen > 0);
  assert(stride >= memsize);  
#ifdef IO_CHECK
  assert(is_equal(memsize));
  assert(is_equal(blocklen));
  assert(is_equal(stride));
#endif

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  send_buffer_init(memsize, memtype);

  init_synchronization();
}

double measure_MPI_IO_write_segmented(int memsize, int blocklen, int stride, 
				      MPI_Datatype memtype, MPI_Datatype etype,
				      MPI_Datatype filetype, int daccess)
{  
  double      start_time, end_time;
  int         myrank, numprocs;
  MPI_Status  status;
  MPI_Offset  block_offset; 
  MPI_Offset  stride_offset; 
  int         cnt;
  int         i=0, j=0;
  MPI_Request request;
  char        *send_buffer;
  MPI_Offset  pos;
  int         msize;
  int         amode;

  myrank        = get_measurement_rank();
  numprocs      = get_measurement_size();

  block_offset  = get_offset(io_dtype, blocklen);
  stride_offset = get_offset(io_dtype, stride);  

#ifdef IO_CHECK
  memset(get_send_buffer(), myrank%255+1, get_send_buffer_usage());
#endif

  amode = MPI_MODE_WRONLY | MPI_MODE_CREATE;
#ifndef IO_CHECK
  amode = amode | MPI_MODE_DELETE_ON_CLOSE;
#endif
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		    func_get_io_datarep(), io_info);  
  MPI_File_set_atomicity(io_fh, io_atomicity);

  if (io_preallocate) {    
    preallocate_file_space(io_fh, io_dtype, (int)(stride*numprocs));
  }

  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization(); 
  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      pos   = myrank*stride_offset+i*block_offset;
      MPI_File_write_at(io_fh, pos, get_send_buffer(), cnt, memtype, &status);
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write(io_fh, get_send_buffer(), cnt, memtype, &status);
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    if (stride == memsize) {

      if (myrank != 0) {
	MPI_Recv(NULL, 0, MPI_BYTE, myrank-1, 23, get_measurement_comm(), &status);
      }
      msize = memsize;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	msize -= blocklen;

	MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype, &status);
      }
      if (myrank < numprocs-1) {
	MPI_Send(NULL, 0, MPI_BYTE, myrank+1, 23, get_measurement_comm());
      }

    } else { /* stride != memsize */

      for (j=0; j<numprocs; ++j) {
	/* synchronization is not guaranteed in MPI_File_seek_shared */
	MPI_Barrier(get_measurement_comm());
	MPI_File_seek_shared(io_fh, j*stride_offset, MPI_SEEK_SET);
	if (j == myrank) {
	  msize = memsize;
	  while (msize > 0) {
	    cnt = MIN_INT(msize, blocklen);
	    msize -= blocklen;

	    MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype, 
				  &status);
	  }
	}
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      pos   = myrank*stride_offset+i*block_offset;
      MPI_File_iwrite_at(io_fh, pos, get_send_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
      ++i;
    }
    

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iwrite(io_fh, get_send_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    if (stride == memsize) {

      if (myrank != 0) {
	MPI_Recv(NULL, 0, MPI_BYTE, myrank-1, 23, get_measurement_comm(), &status);
      }
      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	msize -= blocklen;

	MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype, 
			      &request);	
	++i;
      }
      if (myrank < numprocs-1) {
	MPI_Send(NULL, 0, MPI_BYTE, myrank+1, 23, get_measurement_comm());
      }

    } else { /* stide != memsize */

      for (j=0; j<numprocs; ++j) {
	/* synchronization is not guaranteed in MPI_File_seek_shared */
	MPI_Barrier(get_measurement_comm());
	MPI_File_seek_shared(io_fh, j*stride_offset, MPI_SEEK_SET);
	if (j == myrank) {
	  msize = memsize;
	  while (msize > 0) {
	    cnt = MIN_INT(msize, blocklen);
	    msize -= blocklen;
	    
	    MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype, 
				   &request);
	    MPI_Wait(&request, &status);
	  }
	}
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    i = 0;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      pos   = myrank*stride_offset+i*block_offset;
      MPI_File_write_at_all(io_fh, pos, get_send_buffer(), cnt, memtype, &status);    
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS | 
			 IO_BLOCKING | 
			 IO_COLLECTIVE)) {

    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_all(io_fh, get_send_buffer(), cnt, memtype, &status);
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    if ((memsize <= blocklen) && (memsize == stride)) {
      
      MPI_File_write_ordered(io_fh, get_send_buffer(), memsize, memtype, &status);

    } else { /* ((memsize > blocklen) || (memsize != stride)) */

      for (j=0; j<numprocs; ++j) {
	
	if (memsize < stride) {
	  /* MPI_Barrier(get_measurement_comm()); */
	  MPI_File_seek_shared(io_fh, j*stride_offset, MPI_SEEK_SET);
	}
	
	msize = memsize; 
	while (msize > 0) {
	  if (j == myrank) {
	    cnt = MIN_INT(msize, blocklen);
	  } else {
	    cnt = 0;
	  }
	  msize -= blocklen;

	  MPI_File_write_ordered(io_fh, get_send_buffer(), cnt, memtype, 
				 &status);
	}
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();

    i = 0;
    pos = myrank*stride_offset;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_at_all_begin(io_fh, pos, send_buffer, cnt, memtype);
      pos = myrank*stride_offset+i*block_offset;
      MPI_File_write_at_all_end(io_fh, send_buffer, &status);
      ++i;
    }
    
  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();

    i = 0;
    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_all_begin(io_fh, send_buffer, cnt, memtype);
      MPI_File_write_all_end(io_fh, send_buffer, &status);
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();

   if ((memsize <= blocklen) && (memsize == stride)) {
      
     MPI_File_write_ordered_begin(io_fh, send_buffer, memsize, memtype);
     MPI_File_write_ordered_end(io_fh, send_buffer, &status);

    } else { /* ((memsize > blocklen) || (memsize != stride)) */

     for (j=0; j<numprocs; ++j) {
       
       if (memsize < stride) {
	 /* MPI_Barrier(get_measurement_comm()); */
	 MPI_File_seek_shared(io_fh, j*stride_offset, MPI_SEEK_SET);
       }
       
       msize = memsize; 
       while (msize > 0) {
	 if (j == myrank) {
	   cnt = MIN_INT(msize, blocklen);
	 } else {
	   cnt = 0;
	 }
	 msize -= blocklen;

	 MPI_File_write_ordered_begin(io_fh, send_buffer, cnt, memtype);
	 MPI_File_write_ordered_end(io_fh, send_buffer, &status);
       }
     }
   }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);  
    return -1;
  }
  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();  
  MPI_File_close(&io_fh);  

  return (end_time - start_time);
}

void finalize_MPI_IO_write_segmented(int memsize, int blocklen, int stride, 
				     MPI_Datatype memtype, MPI_Datatype etype, 
				     MPI_Datatype filetype, int daccess)
{
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_read_segmented
 *
 * @description
 * Measures the time to read the file using a segmented pattern.
 *
 * see comments to MPI_IO_write_segmented
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            n*<memsize>
 * memory size:          <memsize>
 * block size:           <blocklen>
 * access modes:         MPI_MODE_RDONLY
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of file read to memory
 * stride                number of elements between start of segments
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */

void init_MPI_IO_read_segmented(int memsize, int blocklen, int stride, 
				MPI_Datatype memtype, MPI_Datatype etype, 
				MPI_Datatype filetype, int daccess)
{
  MPI_Status status;
  char       *send_buffer;

  assert(memsize > 0);
  assert(blocklen > 0);
  assert(stride >= memsize);
#ifdef IO_CHECK
  assert(is_equal(memsize));
  assert(is_equal(blocklen));
  assert(is_equal(stride));
#endif

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  recv_buffer_init(memsize, memtype);

  /* Create file and preallocate file space */
  send_buffer = mpi_malloc_chars(get_extent(stride, memtype));
#ifdef IO_CHECK
  memset(send_buffer, get_measurement_rank()%255+1, get_extent(stride, memtype));
  io_chk_ref = buffer_fill((unsigned char *)send_buffer, memsize, memtype);
  io_chk_msg = 0;
#endif

  MPI_File_open(get_measurement_comm(), io_filename, 
		MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN, 
		io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		    func_get_io_datarep(), io_info);  
  MPI_File_write_ordered(io_fh, send_buffer, stride, memtype, &status);
  MPI_File_close(&io_fh);    
  mpi_free(send_buffer);

  init_synchronization();  
}


double measure_MPI_IO_read_segmented(int memsize, int blocklen, int stride, 
				     MPI_Datatype memtype, MPI_Datatype etype,
				     MPI_Datatype filetype, int daccess)
{
  int         i=0, j=0;
  double      start_time, end_time;
  MPI_Offset  block_offset;
  MPI_Offset  stride_offset;
  int         myrank;
  int         numprocs;
  int         cnt=0;
  MPI_Status  status;
  MPI_Request request;
  char        *recv_buffer;
  int         msize;
  int         amode;

#ifdef IO_CHECK
  long cs = 0l;
#endif

  myrank   = get_measurement_rank();
  numprocs = get_measurement_size();
  block_offset  = get_offset(io_dtype, blocklen);
  stride_offset = get_offset(io_dtype, stride);

  amode = MPI_MODE_RDONLY;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);  
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);
  
  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization(); 
  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at(io_fh, myrank*stride_offset+i*block_offset, 
		       get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    if (stride == memsize) {

      if (myrank != 0) {
	MPI_Recv(NULL, 0, MPI_BYTE, myrank-1, 23, get_measurement_comm(), &status);
      }
      msize = memsize;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	msize -= blocklen;

	MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
	io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      }
      if (myrank < numprocs-1) {
	MPI_Send(NULL, 0, MPI_BYTE, myrank+1, 23, get_measurement_comm());
      }

    } else { /* stide != memsize */
      
      for (j=0; j<numprocs; ++j) {
	/* synchronization is not guaranteed in MPI_File_seek_shared */
	MPI_Barrier(get_measurement_comm());
	MPI_File_seek_shared(io_fh, j*stride_offset, MPI_SEEK_SET);
	if (j == myrank) {
	  msize = memsize;
	  while (msize > 0) {
	    cnt = MIN_INT(msize, blocklen);
	    msize -= blocklen;

	    MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
	    io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
	  }
	}

      } /* for */
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iread_at(io_fh, myrank*stride_offset+i*block_offset, 
			get_recv_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
	    io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }


  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iread(io_fh, get_recv_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
	    io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
    }
    

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    if (stride == memsize) {

      if (myrank != 0) {
	MPI_Recv(NULL, 0, MPI_BYTE, myrank-1, 23, get_measurement_comm(), &status);
      }
      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	msize -= blocklen;

	MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt, memtype, 
			      &request);	
	++i;
      }
      if (myrank < numprocs-1) {
	MPI_Send(NULL, 0, MPI_BYTE, myrank+1, 23, get_measurement_comm());
      }

    } else { /* stide != memsize */

      for (j=0; j<numprocs; ++j) {
	/* synchronization is not guaranteed in MPI_File_seek_shared */
	MPI_Barrier(get_measurement_comm());
	MPI_File_seek_shared(io_fh, j*stride_offset, MPI_SEEK_SET);
	if (j == myrank) {
	  msize = memsize;
	  while (msize > 0) {
	    cnt = MIN_INT(msize, blocklen);
	    msize -= blocklen;

	    MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt, memtype,
				  &request);
	    MPI_Wait(&request, &status);
#ifdef IO_CHECK
	    io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
	  }
	}
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    i = 0;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at_all(io_fh, myrank*stride_offset+i*block_offset, 
			   get_recv_buffer(), cnt, memtype, &status);

#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_all(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

   if ((memsize <= blocklen) && (memsize == stride)) {
      
      MPI_File_read_ordered(io_fh, get_recv_buffer(), memsize, memtype, &status);

    } else { /* ((memsize > blocklen) || (memsize != stride)) */

     for (j=0; j<numprocs; ++j) {
       if (memsize < stride) {
	 /* MPI_Barrier(get_measurement_comm()); */
	 MPI_File_seek_shared(io_fh, j*stride_offset, MPI_SEEK_SET);
       }
       msize = memsize;
       while (msize > 0) {
	 if (j == myrank) {
	   cnt = MIN_INT(msize, blocklen);
	 } else {
	   cnt = 0;
	 }	
	 msize -= blocklen;

	 MPI_File_read_ordered(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
	 io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
       }
     }
   }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();

    i = 0;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at_all_begin(io_fh, myrank*stride_offset+i*block_offset,
				 recv_buffer, cnt, memtype);
      MPI_File_read_at_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)recv_buffer, cnt, memtype, &cs);
#endif      
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();

    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_all_begin(io_fh, recv_buffer, cnt, memtype);
      MPI_File_read_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)recv_buffer, cnt, memtype, &cs);
#endif      
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();
    if ((memsize <= blocklen) && (memsize == stride)) {
      
      MPI_File_read_ordered_begin(io_fh, recv_buffer, memsize, memtype);
      MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
      
    } else { /* ((memsize > blocklen) || (memsize != stride)) */

      for (j=0; j<numprocs; ++j) {
	if (memsize < stride) {
	  /* MPI_Barrier(get_measurement_comm()); */
	  MPI_File_seek_shared(io_fh, j*stride_offset, MPI_SEEK_SET);
	}
	msize = memsize;
	while (msize > 0) {
	  if (j == myrank) {
	    cnt = MIN_INT(msize, blocklen);
	  } else {
	    cnt = 0;
	  }
	  msize -= blocklen;

	  MPI_File_read_ordered_begin(io_fh, recv_buffer, cnt, memtype);
	  MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
	  io_chk_add_sum((unsigned char *)recv_buffer, cnt, memtype, &cs);
#endif      
	}
      }
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }
  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();  
  MPI_File_close(&io_fh);

#ifdef IO_CHECK
  io_chk_compare(cs, "MPI_IO_read_segmented", daccess);
#endif /* IO_CHECK */

  return (end_time - start_time);
}

void finalize_MPI_IO_read_segmented(int memsize, int blocklen, int stride, 
				    MPI_Datatype memtype, MPI_Datatype etype, 
				    MPI_Datatype filetype, int daccess)
{
#ifndef IO_CHECK
  FILE_DELETE(io_filename);
#endif    
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_write_strided
 *
 * @description
 * Write to a file using a strided pattern, i.e. the first block is from
 * process 0, then second from process 1, the next from process 2 and so
 * on.
 * 
 * For data access routines that use explicit offsets or individual file
 * pointers no other communication between the processes is needed to
 * schedule them. Data access routines using shared file pointers and
 * noncollective coordination use MPI_SEND/MPI_RECV to schedule the
 * processes, but only if the stride is equal blocklen. 
 * 
 * In general, data access routing that use shared file pointers loose
 * performace in this pattern when stride is greater than blocklen, because
 * a collective MPI_FILE_SEEK_SHARED routine and MPI_BARRIER operation must 
 * precede each collective I/O operation. In the case of collective 
 * coordination the I/O operations must be called by all processes, even
 * when only one of them has something to write. DON'T USE THESE ROUTINES.
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            n*<memsize>
 * memory size:          <memsize>
 * block size:           <blocklen>
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE |
 *                       MPI_MODE_DELETE_ON_CLOSE
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer (in memtype)
 * blocklen              chunk of memory buffer, that is written to the file
 * stride                number of elements between start of blocks
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double
 * *************************************************************************
 */
void init_MPI_IO_write_strided(int memsize, int blocklen, int stride, 
			       MPI_Datatype memtype, MPI_Datatype etype, 
			       MPI_Datatype filetype, int daccess)
{    
  assert(memsize > 0);
  assert(blocklen > 0);
  assert(stride >= blocklen);
#ifdef IO_CHECK
  assert(is_equal(memsize));
  assert(is_equal(blocklen));
  assert(is_equal(stride));
#endif

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  send_buffer_init(memsize, memtype);

  init_synchronization();
}

double measure_MPI_IO_write_strided(int memsize, int blocklen, int stride, 
				    MPI_Datatype memtype, MPI_Datatype etype,
				    MPI_Datatype filetype, int daccess)
{  
  double      start_time, end_time;
  int         myrank, numprocs;
  MPI_Offset  stride_offset;
  int         cnt;
  int         filesize;
  int         i, j;
  MPI_Status  status;
  MPI_Request request;
  char        *send_buffer;
  MPI_Offset  pos;
  int         msize;
  int         nblocks;
  int         amode;

  myrank = get_measurement_rank();
  numprocs = get_measurement_size();
  stride_offset = get_offset(io_dtype, stride);

#ifdef IO_CHECK
  memset(get_send_buffer(), myrank%255+1, get_send_buffer_usage());
#endif
  
  amode = MPI_MODE_WRONLY | MPI_MODE_CREATE;
#ifndef IO_CHECK
  amode = amode | MPI_MODE_DELETE_ON_CLOSE;
#endif
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;
  
  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);

  /* 
   * MPI-2, 9.4.4 Data Access with Shared File Pointers:
   * "... the use of shared file pointer routines is erroneous unless 
   * all processes use the same file view."
   * => use the same displacement value for all data access routines 
   *    in this function
   */
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  if (io_preallocate)  {
    filesize = (int) (stride*ceil(((double)memsize)/((double)blocklen))*numprocs);
    preallocate_file_space(io_fh, io_dtype, filesize);
  }

  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization(); 
  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_at(io_fh, (i*numprocs+myrank)*stride_offset, 
			get_send_buffer(), cnt, memtype, &status);
      ++i;
    }      

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_seek(io_fh, (i*numprocs+myrank)*stride_offset, MPI_SEEK_SET); 
      MPI_File_write(io_fh, get_send_buffer(), cnt, memtype, &status);
      ++i;
    }      

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    /* 
     * MPI-2, 9.4.4 Data Access with Shared File Pointers: 
     * "* the effect of multiple calls to shared file pointer routines is
     * defined to behave as if the calls were serialized, ..."
     * 
     * "For the noncollective shared file pointer routines, the serialization 
     * ordering is not deterministic. The user needs to use other synchronization
     * means to enforce a specific order."
     * 
     * "After a shared file pointer operation is initiated, the shared file
     * pointer is updated to point to the next etype after the last one that
     * will be accessed." 
     *
     * "MPI_FILE_SEEK_SHARED is collective; all the processes in the communicator
     * group with the file handle fh must call MPI_FILE_SEEK_SHARED with the same
     * values for offset and whence."
     */      

    if (stride == blocklen) {
      nblocks = (int) ceil((double)memsize/(double)blocklen);
      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	if (msize >= blocklen) {	  
	  if ((numprocs > 1) && ((i != 0) || (myrank != 0))) {
	    MPI_Recv(NULL, 0, MPI_BYTE, ((myrank-1)+numprocs)%numprocs, 42,
		     get_measurement_comm(), &status);
	  } 
	  MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype, &status);
	  if ((numprocs > 1) && ((i < nblocks-1) || (myrank != numprocs-1))) {
	    MPI_Send(NULL, 0, MPI_BYTE, (myrank+1)%numprocs, 42,
		     get_measurement_comm());
	  }

	} else { /* msize < blocklen */
       
	  for (j=0; j<numprocs; j++) {
	    /* synchronization is not guaranteed in MPI_File_seek_shared */
	    MPI_Barrier(get_measurement_comm()); 
	    MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset, MPI_SEEK_SET);
	    if (j == myrank) {
	      MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype,
				    &status);
	    }
	  }
	}
	msize -= blocklen;
	++i;
      }      

    } else { /* stride != blocklen */

      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	msize -= blocklen;

	for (j=0; j<numprocs; j++) {
	  /* synchronization is not guaranteed in MPI_File_seek_shared */
	  MPI_Barrier(get_measurement_comm());	  
	  MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset, MPI_SEEK_SET);
	  if (j == myrank) {
	    MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype,
				  &status);
	  }
	}
	
	++i;
      }      
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    /* 
     * MPI-2, 2.4.3 Data Access with Individual File Pointers:
     * "After an individual file pointer operation is initiated, 
     * the individual file pointer is updated to point to the next etype 
     * after the last one that will be accessed." 
     */

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iwrite_at(io_fh, (i*numprocs+myrank)*stride_offset, 
			 get_send_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
      ++i;
    }

    
  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    /* MPI-2, 9.4.3 Data Access with Individual File Pointers:
     * "After an individual file pointer operation is initiated, the individual
     * file pointer is updated to point to the next etype after the last one 
     * that will be accessed."
     */

    msize = memsize;
    i=0; 
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_seek(io_fh, (i*numprocs+myrank)*stride_offset, MPI_SEEK_SET); 
      MPI_File_iwrite(io_fh, get_send_buffer(), cnt, memtype,
		      &request);
      MPI_Wait(&request, &status);
      ++i;
    }          
      
  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    if (stride == blocklen) {

      nblocks = (int) ceil((double)memsize/(double)blocklen);
      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	if (msize >= blocklen) {	  
	  if ((numprocs > 1) && ((i != 0) || (myrank != 0))) {
	    MPI_Recv(NULL, 0, MPI_BYTE, ((myrank-1)+numprocs)%numprocs, 42,
		     get_measurement_comm(), &status);
	  } 
	  MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype,
				 &request);
	  if ((numprocs > 1) && ((i < nblocks-1) || (myrank != numprocs-1))) {
	    MPI_Send(NULL, 0, MPI_BYTE, (myrank+1)%numprocs, 42,
		     get_measurement_comm());
	  }

	} else { /* msize < blocklen */
       
	  for (j=0; j<numprocs; j++) {
	    /* synchronization is not guaranteed in MPI_File_seek_shared */
	    MPI_Barrier(get_measurement_comm());       
	    MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,
				 MPI_SEEK_SET);
	    if (j == myrank) {
	      MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype,
				     &request);
	      MPI_Wait(&request, &status);
	    }
	  }
	}
	msize -= blocklen;
	++i;
      }      

    } else { /* stride != blocklen */

      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	msize -= blocklen;

	for (j=0; j<numprocs; j++) {
	  /* synchronization is not guaranteed in MPI_File_seek_shared */
	  MPI_Barrier(get_measurement_comm());	  
	  MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,MPI_SEEK_SET);
	  if (j == myrank) {
	    MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype,
				   &request);
	    MPI_Wait(&request, &status);
	  }
	}	
	++i;
      }      
    }
    
  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    /* 
     * MPI-2, 9.4.1 Data Access Routines:
     * "... the completion of a collective call may depend on the activity of
     * the other processes participating in the collective call. ..."
     *
     * "Collective operations may perform much better than their noncollective
     * counterparts, as global data access have significant potential for
     * automatic optimization."
     */

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_at_all(io_fh, (i*numprocs+myrank)*stride_offset, 
			    get_send_buffer(), cnt, memtype, &status);
      ++i;
    }      

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_seek(io_fh, (i*numprocs+myrank)*stride_offset, MPI_SEEK_SET); 
      MPI_File_write_all(io_fh, get_send_buffer(), cnt, memtype, &status);
      ++i;
    }      

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    /* 
     * MPI-2, 9.4.4 Data Access with Shared File Pointers, Collective Operations:
     * "... the accesses to the file will be in the order determined by the
     * ranks of the processes within the group. ..."
     *
     * "In addition, in order to prevent subsequent shared offset accesses
     * by the same processes from interfering with this collective access,
     * the call might return only after all the processes within the group
     * have initiated their access."
     *
     * "When the call returns, the shared file pointer points to the next 
     * etype accessible, according to the file view used by all processes,
     * after the last etype requested."
     */

    if (stride == blocklen) {
      
      msize = memsize;
      i = 0;
      while (msize > 0) {
	if (msize >= blocklen) {
	  cnt = blocklen;
	  MPI_File_write_ordered(io_fh, get_send_buffer(), cnt, memtype,
				 &status);
	} else {
	  for (j=0; j<numprocs; j++) {
	    cnt = (j == myrank) ? msize : 0;
	    /* MPI_Barrier(get_measurement_comm()); */
	    MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,
				 MPI_SEEK_SET);
 	    MPI_File_write_ordered(io_fh, get_send_buffer(), cnt, memtype,
				   &status);
	  }
	}	
	msize -= blocklen;
	++i;
      }

    } else { /* stride != blocklen */

      msize = memsize;
      i = 0;
      while (msize > 0) {
	for (j=0; j<numprocs; j++) {
	  if (j == myrank) {
	    cnt = MIN_INT(msize, blocklen);
	  } else {
	    cnt = 0;
	  }
	  /* MPI_Barrier(get_measurement_comm()); */
	  MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,MPI_SEEK_SET);
	  MPI_File_write_ordered(io_fh, get_send_buffer(), cnt, memtype,
				 &status);
	}	
	msize -= blocklen;
	++i;
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    /*
     * MPI-2, 9.4.5 Split Collective Data Access Routine:
     * "... the user must not use the buffer passed to a begin routine while
     * the routine is outstanding; the operation must be completed with an 
     * end routine before it is safe to free buffers, etc."
     *
     * "* Begin calls are collective ..."
     * "* End calls are collective ..."
     * "* ... buffer in both the begin and end routines ...
     *    avoid many of the problems decribed in `A Problem with Register
     *    Optimization', Section 10.2.2"
     * "* No collective I/O operations are permitted on a file handle 
     *    concurrently with a split collective access on that file handle. 
     *    ..."
     */
    send_buffer = get_send_buffer();

    msize = memsize;
    i = 0;
    pos = myrank*stride_offset;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_at_all_begin(io_fh, pos, send_buffer, cnt, memtype);
      pos = ((i+1)*numprocs+myrank)*stride_offset;
      MPI_File_write_at_all_end(io_fh, send_buffer, &status);

      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    /* MPI-2, 9.4.5 Split Collective Data Access Routines:
     * "The begin routine begins the operation, much like a nonblocking 
     * data access (e.g. MPI_FILE_IREAD). The end routine completes the
     * operation, much like the matching test or wait (e.g. MPI_WAIT)."
     *
     * "An implementation is free to implement any split collective data 
     * access routine using the corresponding blocking collective routine 
     * when either the begin call (e.g. MPI_FILE_READ_ALL_BEGIN) the end 
     * call (e.g. MPI_FILE_READ_ALL_END) is issued."
     *
     * MPI-2, 9.4.3 Data Access with Individual File Pointers:
     * "After an individual file pointer operation is initiated, the individual
     * file pointer is updated to point to the next etype after the last one 
     * that will be accessed."
     * 
     * MPI-2, 9.4.4 Data Access with Shared File Pointers:
     * "After an shared file pointer operation is initiated, the shared
     * file pointer is updated to point to the next etype after the last
     * one that will be accessed."
     */

    /* MPI-2, 9.4.5 Split Collective Data Access Routines:
     * ".. When an 'end' call is made, exactly one unmatched 'begin' call
     * for the same operation must precede it."
     *
     * "No collective I/O operation are permitted on a file handle concurrently
     * with a split collective access on that file handle (..)."
     */
    send_buffer = get_send_buffer();

    msize = memsize;
    i = 0;
    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_all_begin(io_fh, send_buffer, cnt, memtype);
      MPI_File_seek(io_fh, ((i+1)*numprocs+myrank)*stride_offset, MPI_SEEK_SET);
      MPI_File_write_all_end(io_fh, send_buffer, &status);
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    /* 
     * MPI-2, 9.4.4 Data Access with Shared File Pointers, Collective Operations:
     * "Advice to implementors: ...
     * Once all processes have issued their requests, locations within the file 
     * for all accesses can be computed, and accesses can proceed independently
     * from each other ..."
     */
    send_buffer = get_send_buffer();

    if (stride == blocklen) {
      msize = memsize;
      i = 0;
      while (msize > 0) {	
	if (msize >= blocklen) {
	  MPI_File_write_ordered_begin(io_fh, send_buffer, blocklen, memtype);
	  MPI_File_write_ordered_end(io_fh, send_buffer, &status);	  
	} else {
	  for (j=0; j<numprocs; j++) {
	    cnt = (j == myrank) ? msize : 0;
	    /* MPI_Barrier(get_measurement_comm()); */
	    MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,
				 MPI_SEEK_SET);
	    MPI_File_write_ordered_begin(io_fh, send_buffer, cnt, memtype);
	    MPI_File_write_ordered_end(io_fh, send_buffer, &status);
	  }
	}	
      msize -= blocklen;
      ++i;
      }      
      
    } else { /* stride != blocklen */
      
      msize = memsize;
      i = 0;
      while (msize > 0) {
	for (j=0; j<numprocs; j++) {
	  if (j == myrank) {
	    cnt = MIN_INT(msize, blocklen);
	  } else {
	    cnt = 0;
	  }
	  /* MPI_Barrier(get_measurement_comm()); */
	  MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,MPI_SEEK_SET);
	  MPI_File_write_ordered_begin(io_fh, send_buffer, cnt, memtype);
	  MPI_File_write_ordered_end(io_fh, send_buffer, &status);
	}
	msize -= blocklen;
	++i;
      }
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();  
  MPI_File_close(&io_fh);  

  return (end_time-start_time);
}

void finalize_MPI_IO_write_strided(int memsize, int blocklen, int stride, 
				   MPI_Datatype memtype, MPI_Datatype etype,
				   MPI_Datatype filetype, int daccess)
{
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_read_strided
 *
 * @description
 * Measures the time to read the file using a strided pattern.
 *
 * see comments to MPI_IO_write_strided 
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            n*<memsize>
 * memory size:          <memsize>
 * block size:           <blocklen>
 * access modes:         MPI_MODE_RDONLY
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of file read to memory
 * stride                number of elements between start of blocks
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_read_strided(int memsize, int blocklen, int stride, 
			      MPI_Datatype memtype, MPI_Datatype etype,
			      MPI_Datatype filetype, int daccess)
{
  int        i=0;
  MPI_Aint   block_extent;
  MPI_Status status;
  char       *send_buffer;
  int        cnt;
  int        myrank, numprocs;
  int        msize=0;
  MPI_Offset stride_offset;

  assert(memsize > 0);
  assert(blocklen > 0);
  assert(stride >= blocklen);
#ifdef IO_CHECK
  assert(is_equal(memsize));
  assert(is_equal(blocklen));
  assert(is_equal(stride));
#endif

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  recv_buffer_init(memsize, memtype);

  myrank = get_measurement_rank();
  numprocs = get_measurement_size();
  block_extent = get_extent(blocklen, memtype);
  stride_offset = get_offset(io_dtype, stride);

  send_buffer = mpi_malloc_chars(get_extent(memsize, memtype));
#ifdef IO_CHECK
  io_chk_ref  = buffer_fill((unsigned char *)send_buffer, memsize, memtype);
  io_chk_msg = 0;
#endif

  MPI_File_open(get_measurement_comm(), io_filename, 
		MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN,
		io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		    func_get_io_datarep(), io_info);  
  i = 0;
  msize = memsize;
  while (msize > 0) {
    cnt = MIN_INT(msize, blocklen);
    msize -= blocklen;

    MPI_File_seek(io_fh, (i*numprocs+myrank)*stride_offset, MPI_SEEK_SET); 
    MPI_File_write(io_fh, send_buffer+i*block_extent, cnt, memtype, &status);
    ++i;
  }  
  MPI_File_close(&io_fh);
  mpi_free(send_buffer);
  
  init_synchronization();  
} 

double measure_MPI_IO_read_strided(int memsize, int blocklen, int stride, 
				   MPI_Datatype memtype, MPI_Datatype etype,
				   MPI_Datatype filetype, int daccess)
{
  double     start_time, end_time;
  MPI_Offset stride_offset;
  int        myrank, numprocs;
  int        cnt;
  char       *recv_buffer;
  int        i=0, j=0;
  MPI_Request request;
  MPI_Status status;
  int        msize;
  int        nblocks;
  int        amode;
  
#ifdef IO_CHECK
  long cs = 0l;
#endif /* IO_CHECK */

  myrank   = get_measurement_rank();
  numprocs = get_measurement_size();
  stride_offset = get_offset(io_dtype, stride);

  amode = MPI_MODE_RDONLY;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);  
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);


  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization(); 
  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at(io_fh, (i*numprocs+myrank)*stride_offset,
		       get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_seek(io_fh, (i*numprocs+myrank)*stride_offset, MPI_SEEK_SET);
      MPI_File_read(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    if (stride == blocklen) {

      nblocks = (int) ceil((double)memsize/(double)blocklen);
      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	if (msize >= blocklen) {	  	  
	  if ((numprocs > 1) && ((i != 0) || (myrank != 0))) {
	    MPI_Recv(NULL, 0, MPI_BYTE, ((myrank-1)+numprocs)%numprocs, 42, 
		     get_measurement_comm(), &status);
	  } 
	  MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype,&status);
#ifdef IO_CHECK
	  io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */
	  if ((numprocs > 1) && ((i < nblocks-1) || (myrank != numprocs-1))) {
	    MPI_Send(NULL, 0, MPI_BYTE, (myrank+1)%numprocs, 42, 
		     get_measurement_comm());
	  }
	  
	} else { /* msize < blocklen */
       
	  for (j=0; j<numprocs; ++j) {
	    /* synchronization is not guaranteed in MPI_File_seek_shared */
	    MPI_Barrier(get_measurement_comm());       
	    MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,
				 MPI_SEEK_SET);
	    if (j == myrank) {
	      MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype,
				   &status);
#ifdef IO_CHECK
	      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */
	    }
	  }
	}
	msize -= blocklen;
	++i;
      }      

    } else { /* stride != blocklen */

      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);	
	msize -= blocklen;

	for (j=0; j<numprocs; ++j) {
	  /* synchronization is not guaranteed in MPI_File_seek_shared */
	  MPI_Barrier(get_measurement_comm());	  
	  MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,MPI_SEEK_SET);
	  if (j == myrank) {
	    MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype,
				 &status);
#ifdef IO_CHECK
	    io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */
	  }
	}
	++i;
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iread_at(io_fh, (i*numprocs+myrank)*stride_offset,
			get_recv_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_seek(io_fh, (i*numprocs+myrank)*stride_offset, MPI_SEEK_SET);
      MPI_File_iread(io_fh, get_recv_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }


  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    if (stride == blocklen) {

      nblocks = (int) ceil((double)memsize/(double)blocklen);
      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	if (msize >= blocklen) {	  
	  if ((numprocs > 1) && ((i != 0) || (myrank != 0))) {
	    MPI_Recv(NULL, 0, MPI_BYTE, ((myrank-1)+numprocs)%numprocs, 42,
		     get_measurement_comm(), &status);
	  } 
	  MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt, memtype,
				&request);
	  if ((numprocs > 1) && ((i < nblocks-1) || (myrank != numprocs-1))) {
	    MPI_Send(NULL, 0, MPI_BYTE, (myrank+1)%numprocs, 42,
		     get_measurement_comm());
	  }

	} else { /* msize < blocklen */
       
	  for (j=0; j<numprocs; ++j) {
	    /* synchronization is not guaranteed in MPI_File_seek_shared */
	    MPI_Barrier(get_measurement_comm());       
	    MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,
				 MPI_SEEK_SET);
	    if (j == myrank) {
	      MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt, memtype,
				    &request);
	      MPI_Wait(&request, &status);
#ifdef IO_CHECK
	      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
	    }
	  }
	}
	msize -= blocklen;
	++i;
      }      

    } else { /* stride != blocklen */

      msize = memsize;
      i = 0;
      while (msize > 0) {
	cnt = MIN_INT(msize, blocklen);
	msize -= blocklen;

	for (j=0; j<numprocs; ++j) {
	  /* synchronization is not guaranteed in MPI_File_seek_shared */
	  MPI_Barrier(get_measurement_comm());	  
	  MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset, MPI_SEEK_SET);
	  if (j == myrank) {
	    MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt,
				  memtype, &request);
	    MPI_Wait(&request, &status);
#ifdef IO_CHECK
	    io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
	  }
	}	
	++i;
      }            
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at_all(io_fh, (i*numprocs+myrank)*stride_offset, 
			   get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_seek(io_fh, (i*numprocs+myrank)*stride_offset, MPI_SEEK_SET);
      MPI_File_read_all(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    if (stride == blocklen) {      
      msize = memsize;
      i = 0;
      while (msize > 0) {
	if (msize >= blocklen) {
	  MPI_File_read_ordered(io_fh, get_recv_buffer(), blocklen, memtype,
				&status);
#ifdef IO_CHECK
	  io_chk_add_sum((unsigned char*)get_recv_buffer(), blocklen, memtype, &cs);
#endif /* IO_CHECK */
	} else {
	  for (j=0; j<numprocs; ++j) {
	    cnt = (j == myrank) ? msize : 0;
	    /* MPI_Barrier(get_measurement_comm()); */
	    MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,
				 MPI_SEEK_SET);
	    MPI_File_read_ordered(io_fh, get_recv_buffer(), cnt, memtype,
				  &status);
#ifdef IO_CHECK
	    io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */
	  }
	}
	
	msize -= blocklen;
	++i;
      }

    } else { /* stride != blocklen */

      msize = memsize;
      i = 0;
      while (msize > 0) {
	for (j=0; j<numprocs; ++j) {
	  if (j == myrank) {
	    cnt = MIN_INT(msize, blocklen);
	  } else {
	    cnt = 0;
	  }
	  /* MPI_Barrier(get_measurement_comm()); */
	  MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset, MPI_SEEK_SET);
	  MPI_File_read_ordered(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
	  io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */
	}	
	msize -= blocklen;
	++i;
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at_all_begin(io_fh, (i*numprocs+myrank)*stride_offset,
				 recv_buffer, cnt, memtype);
      MPI_File_read_at_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)recv_buffer, cnt, memtype, &cs);
#endif /* IO_CHECK */
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();

    msize = memsize;
    i = 0;
    MPI_File_seek(io_fh, myrank*stride_offset, MPI_SEEK_SET);
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_all_begin(io_fh, recv_buffer, cnt, memtype);
      MPI_File_seek(io_fh, ((i+1)*numprocs+myrank)*stride_offset, MPI_SEEK_SET);
      MPI_File_read_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)recv_buffer, cnt, memtype, &cs);
#endif /* IO_CHECK */
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();	

    if (stride == blocklen) {
      msize = memsize;
      i = 0;
      while (msize > 0) {
	if (msize >= blocklen) {
	  MPI_File_read_ordered_begin(io_fh, recv_buffer, blocklen, memtype);
	  MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
	  io_chk_add_sum((unsigned char*)recv_buffer, blocklen, memtype, &cs);
#endif /* IO_CHECK */
	  
	} else {
	  for (j=0; j<numprocs; ++j) {
	    cnt = (j == myrank) ? msize : 0;
	    /* MPI_Barrier(get_measurement_comm()); */
	    MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset,
				 MPI_SEEK_SET);
	    MPI_File_read_ordered_begin(io_fh, recv_buffer, cnt, memtype);
	    MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
	    io_chk_add_sum((unsigned char*)recv_buffer, cnt, memtype, &cs);
#endif /* IO_CHECK */
	  }
	}	
      msize -= blocklen;
      ++i;
      }      
      
    } else { /* stride != blocklen */
      
      msize = memsize;
      i = 0;
      while (msize > 0) {
	for (j=0; j<numprocs; ++j) {
	  if (j == myrank) {
	    cnt = MIN_INT(msize, blocklen);
	  } else {
	    cnt = 0;
	  }
	  /* MPI_Barrier(get_measurement_comm()); */
	  MPI_File_seek_shared(io_fh, (i*numprocs+j)*stride_offset, MPI_SEEK_SET);
	  MPI_File_read_ordered_begin(io_fh, recv_buffer, cnt, memtype);
	  MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
	  io_chk_add_sum((unsigned char*)recv_buffer, cnt, memtype, &cs);
#endif /* IO_CHECK */
	}
	msize -= blocklen;
	++i;
      }
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();  
  MPI_File_close(&io_fh);

#ifdef IO_CHECK
  io_chk_compare(cs, "MPI_IO_read_strided", daccess);
#endif /* IO_CHECK */

  return (end_time - start_time);
}

void finalize_MPI_IO_read_strided(int memsize, int blocklen, int stride, 
				  MPI_Datatype memtype, MPI_Datatype etype, 
				  MPI_Datatype filetype, int daccess)
{
#ifndef IO_CHECK
  FILE_DELETE(io_filename);
#endif    
  mpi_free(io_filename);
}



/* *************************************************************************
 * MPI_IO_read_file
 *
 * @description
 * Measures the time to read the whole file by all processes.
 *
 * All prozesses executing this measurement must use the same 
 * datatypes for memtype, etype, and filetype. The datatype
 * must be all contiguous.
 *
 * The function doesn't support data access routines using 
 * shared file pointer
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            <count>
 * memory size:          <count>
 * block size:           <count>
 * access modes:         MPI_MODE_RDONLY
 * memory datatype:      contiguous
 * etype:                contiguous
 * filetype:             contiguous 
 * positioning:          individual fp, explicit offsets
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * count                 global size of memory buffer
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_read_file(int count,
			   MPI_Datatype memtype, MPI_Datatype etype, 
			   MPI_Datatype filetype, int daccess)
{
  MPI_Status status;
  char       *send_buffer;
  int        type_size;

  assert(count > 0);

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  set_recv_buffer_usage( get_extent( count, memtype ) );

  MPI_Type_size( memtype, &type_size );
  set_reported_message_size( type_size * count * get_measurement_size() );

#ifdef IO_CHECK
  io_chk_msg = 0;
#endif
  if (get_measurement_rank() == 0) {
    send_buffer = mpi_malloc_chars(get_extent(count, memtype));
#ifdef IO_CHECK
  io_chk_ref  = buffer_fill((unsigned char *)send_buffer, count, memtype);
#endif
   
    MPI_File_open(MPI_COMM_SELF, io_filename, 
		  MPI_MODE_WRONLY | MPI_MODE_CREATE, 
		  io_info, &io_fh);
    MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		      func_get_io_datarep(), io_info);  
    MPI_File_write(io_fh, send_buffer, count, memtype, &status);
    MPI_File_close(&io_fh);
    mpi_free(send_buffer);
  }
#ifdef IO_CHECK
  MPI_Bcast(&io_chk_ref, 1, MPI_LONG, 0, get_measurement_comm());
#endif

  MPI_Barrier(get_measurement_comm());
  init_synchronization();
} 


double measure_MPI_IO_read_file(int count,
				MPI_Datatype memtype, MPI_Datatype etype,
				MPI_Datatype filetype, int daccess)
{
  int         i=0;
  double      start_time, end_time;
  char        *recv_buffer;
  MPI_Status  status;
  MPI_Request request;
  int         amode;
#ifdef IO_CHECK
  long cs = 0l;
#endif

  amode = MPI_MODE_RDONLY;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);  
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    MPI_File_read_at(io_fh, 0, get_recv_buffer(), count, memtype, &status);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    MPI_File_read(io_fh, get_recv_buffer(), count, memtype, &status);
 
  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {


    MPI_File_iread_at(io_fh, 0, get_recv_buffer(), count, memtype, &request);
    MPI_Wait(&request, &status);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    MPI_File_iread(io_fh, get_recv_buffer(), count, memtype, &request);
    MPI_Wait(&request, &status);

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    MPI_File_read_at_all(io_fh, 0, get_recv_buffer(), count, memtype, &status);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    MPI_File_read_all(io_fh, get_recv_buffer(), count, memtype, &status);

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {


    recv_buffer = get_recv_buffer();
    MPI_File_read_at_all_begin(io_fh, 0, recv_buffer, count, memtype);
    MPI_File_read_at_all_end(io_fh, recv_buffer, &status);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();
    MPI_File_read_all_begin(io_fh, recv_buffer, count, memtype);
    MPI_File_read_all_end(io_fh, recv_buffer, &status);

  } else {
      stop_synchronization();
      MPI_File_close(&io_fh);
      return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  MPI_File_close(&io_fh);
  end_time = stop_synchronization();

#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), count, memtype, &cs);
      io_chk_compare(cs, "MPI_IO_read_file", daccess);
#endif /* IO_CHECK */  


  return (end_time - start_time);
}

void finalize_MPI_IO_read_file(int count,
			       MPI_Datatype memtype, MPI_Datatype etype,
			       MPI_Datatype filetype, int daccess)
{
#ifndef IO_CHECK
  FILE_DELETE(io_filename);
#endif
  mpi_free(io_filename);
}

/* *************************************************************************
 * MPI_IO_read_and_distribute
 *
 * @description
 * Measures the time to read the whole file. The file is splited in disjoint
 * equal sized parts. Each part is read by a process and then distributed
 * to the other processes.
 *
 * All prozesses executing this measurement must use the same 
 * datatypes for memtype, etype, and filetype. The datatype
 * must be all contiguous.
 *
 * process group:        <rcomm>
 * group-files ratio:    np:1 (np processes operate on one file)
 * file size:            <count>
 * memory size:          <count>
 * block size:           <count>/np
 * access modes:         MPI_MODE_RDONLY
 * memory datatype:      contiguous
 * etype:                contiguous
 * filetype:             contiguous 
 * positioning:          individual fp, explicit offsets
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * count                 global size of memory buffer
 * np                    number of processes that read the file
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_read_and_distribute(int count, int np,
				     MPI_Datatype memtype, 
				     MPI_Datatype etype,
				     MPI_Datatype filetype,
				     int daccess)
{
  MPI_Status status;
  char       *send_buffer;
  int        type_size;

  assert(count > 0);
  assert(np > 0);

  if (np > get_measurement_size()) {
    np = get_measurement_size();
  }

  /* Small adjustment to count, if count can't be divided by np
   * with no remainder.
   */
  count = (int) ceil((double) count / (double) np) * np;

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  set_recv_buffer_usage( get_extent( count, memtype ) );

  MPI_Type_size( memtype, &type_size );
  set_reported_message_size( type_size * count * get_measurement_size() );


  /* Needed for the assertion in FILE_DELETE:
   * It is needed to test, if a file is closed by all processes.
   */
  io_fh = MPI_FILE_NULL;

#ifdef IO_CHECK
  io_chk_msg = 0;
#endif
  
  if (get_measurement_rank() == 0) {
    send_buffer = mpi_malloc_chars(get_extent(count, memtype));
#ifdef IO_CHECK
    io_chk_ref  = buffer_fill((unsigned char *)send_buffer, count, memtype);
#endif
   
    MPI_File_open(MPI_COMM_SELF, io_filename, 
		  MPI_MODE_WRONLY | MPI_MODE_CREATE, 
		  io_info, &io_fh);
    MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		      func_get_io_datarep(), io_info);  
    MPI_File_write(io_fh, send_buffer, count, memtype, &status);
    MPI_File_close(&io_fh);
    mpi_free(send_buffer);
  }
   
#ifdef IO_CHECK
  MPI_Bcast(&io_chk_ref, 1, MPI_LONG, 0, get_measurement_comm());
#endif

  init_synchronization();
}

double measure_MPI_IO_read_and_distribute(int count, int np, 
					  MPI_Datatype memtype,
					  MPI_Datatype etype, 
					  MPI_Datatype filetype,
					  int daccess)
{
  int         i=0;
  double      start_time, end_time;
  int         cnt;
  char        *recv_buffer;
  MPI_Offset  offset;
  MPI_Status  status;
  MPI_Request request;
  int         amode;
  MPI_Comm    rcomm;
  int         rcomm_rank;
  int         *recv_counts;
  int         *recv_displs;
#ifdef IO_CHECK
  long cs = 0l;
#endif


  if (daccess & IO_SHARED_FILE_POINTER) {
    return -1;
  }
  
  if (np > get_measurement_size()) {
    np = get_measurement_size();
  }

  /* Small adjustment to count, if count can't be divided by np
   * with no remainder.
   */
  count = (int) ceil((double) count / (double) np) * np;

  cnt = count / np;
  recv_counts = mpi_malloc_ints(get_measurement_size());
  recv_displs = mpi_malloc_ints(get_measurement_size());
  
  for (i=0; i<np; ++i) {
    recv_counts[i] = cnt;
    recv_displs[i] = i * cnt;
  }

  for (i=np; i<get_measurement_size(); ++i) {
    recv_counts[i] = 0;
    recv_displs[i] = 0;
  }

  /* Create new communicator, that contains only processes
   * that perform the reading. 
   */
  MPI_Comm_split(get_measurement_comm(), 
		 (get_measurement_rank() < np) ? 0 : MPI_UNDEFINED,
		 get_measurement_rank(), &rcomm);

  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();
  
  if (rcomm != MPI_COMM_NULL) {

    
    MPI_Comm_rank(rcomm, &rcomm_rank);
    offset = rcomm_rank * get_offset(io_dtype, cnt);
    recv_buffer = mpi_malloc_chars(get_extent(cnt, memtype));

    amode = MPI_MODE_RDONLY;
    if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;
    
    MPI_File_open(rcomm, io_filename, amode, io_info, &io_fh);  
    MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype, 
		      func_get_io_datarep(), io_info);
    MPI_File_set_atomicity(io_fh, io_atomicity);
    
    if (daccess == (IO_EXPLICIT_OFFSETS |
		    IO_BLOCKING |
		    IO_NONCOLLECTIVE)) {
      
      MPI_File_read_at(io_fh, offset, recv_buffer, cnt, memtype, &status);
      
    } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			   IO_BLOCKING |
			   IO_NONCOLLECTIVE)) {

      MPI_File_seek(io_fh, offset, MPI_SEEK_SET);
      MPI_File_read(io_fh, recv_buffer, cnt, memtype, &status);
      
    } else if (daccess == (IO_EXPLICIT_OFFSETS |
			   IO_NONBLOCKING_SPLIT |
			   IO_NONCOLLECTIVE)) {
      
      MPI_File_iread_at(io_fh, offset, recv_buffer, cnt,
			memtype, &request);
      MPI_Wait(&request, &status);
      
    } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			   IO_NONBLOCKING_SPLIT |
			   IO_NONCOLLECTIVE)) {
      
      MPI_File_seek(io_fh, offset, MPI_SEEK_SET);
      MPI_File_iread(io_fh, recv_buffer, cnt, memtype, &request);
      MPI_Wait(&request, &status);
      
    } else if (daccess == (IO_EXPLICIT_OFFSETS |
			   IO_BLOCKING |
			   IO_COLLECTIVE)) {
      
      MPI_File_read_at_all(io_fh, offset, recv_buffer, cnt,
			   memtype, &status);
      
    } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			   IO_BLOCKING |
			   IO_COLLECTIVE)) {
      
      MPI_File_seek(io_fh, offset, MPI_SEEK_SET);
      MPI_File_read_all(io_fh, recv_buffer, cnt, memtype, &status);
      
    } else if (daccess == (IO_EXPLICIT_OFFSETS |
			   IO_NONBLOCKING_SPLIT |
			   IO_COLLECTIVE)) {
      
      MPI_File_read_at_all_begin(io_fh, offset, recv_buffer, cnt, memtype);
      MPI_File_read_at_all_end(io_fh, recv_buffer, &status);
      
    } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			   IO_NONBLOCKING_SPLIT |
			   IO_COLLECTIVE)) {
      
      MPI_File_seek(io_fh, offset, MPI_SEEK_SET);
      MPI_File_read_all_begin(io_fh, recv_buffer, cnt, memtype);
      MPI_File_read_all_end(io_fh, recv_buffer, &status);
      
    } else {
      MPI_File_close(&io_fh);
      error_with_abort(MPI_ERR_ARG,
		       "\nError in MPI_IO_read_and_distribute: "
		       "Invalid 'daccess' argument (daccess = %d).\n", 
		       daccess);
    }
    
    MPI_File_close(&io_fh);

    /* distribute data */
    MPI_Allgatherv(recv_buffer, cnt, memtype, get_recv_buffer(), recv_counts,
		   recv_displs, memtype, get_measurement_comm());

    mpi_free(recv_buffer);

  } else { /* rcomm == MPI_COMM_NULL */
    
    MPI_Allgatherv(NULL, 0, memtype, get_recv_buffer(), recv_counts,
		   recv_displs, memtype, get_measurement_comm());
    
  }

  end_time = stop_synchronization();

  if (rcomm != MPI_COMM_NULL) {
        MPI_Comm_free(&rcomm);
  }

#ifdef IO_CHECK
  io_chk_add_sum((unsigned char*)get_recv_buffer(), count, memtype, &cs);
  io_chk_compare(cs, "MPI_IO_read_file", daccess);
#endif /* IO_CHECK */  

  mpi_free(recv_counts);
  mpi_free(recv_displs);

  return (end_time - start_time);
}

void finalize_MPI_IO_read_and_distribute(int count, int np,
				     MPI_Datatype memtype, 
				     MPI_Datatype etype,
				     MPI_Datatype filetype,
				     int daccess)
{
#ifndef IO_CHECK
  FILE_DELETE(io_filename);
#endif
  mpi_free(io_filename);
}

/* *************************************************************************
 * MPI_IO_write_random
 *
 * @description
 * Measures the time to write in a file in a round-robin manner.
 * The values of `memsize' and `blocklen' may differ between the processes.
 *
 * Unlike MPI_IO_write_strided the blocks, that are written to the file,
 * are aligned, i.e. there is no stride between the end of a block and
 * the begin of the next block.
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            SUM( <memsize>_i ), i=0..n-1
 * memory size:          <memsize>_i, i=0..n-1
 * block size:           <blocklen>_i, i=0..n-1
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE
 *                       MPI_MODE_DELETE_ON_CLOSE
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer (all processes)
 * blocklen              chunk of memory that is written to file (all processes)
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_write_random(IArray memsize, IArray blocklen, 
			      MPI_Datatype memtype, MPI_Datatype etype, 
			      MPI_Datatype filetype,
			      int daccess)
{
  assert(memsize.n == blocklen.n);
  assert(memsize.n == get_measurement_size());

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  send_buffer_init(memsize.v[get_measurement_rank()], memtype);

  init_synchronization();
}

double measure_MPI_IO_write_random(IArray memsize, IArray blocklen, 
				   MPI_Datatype memtype, MPI_Datatype etype,
				   MPI_Datatype filetype,
				   int daccess)
{
  double      start_time, end_time;
  int         i;
  int         myrank, numprocs;
  int         filesize; 
  int         cnt;
  MPI_Status  status; 
  MPI_Request request;
  MPI_Offset  *offsets = NULL;
  int         msize;
  int         nb;
  char        *send_buffer;
  int         maxnumblocks;
  int         amode;

  myrank = get_measurement_rank();
  numprocs = get_measurement_size();

#ifdef IO_CHECK
  memset(get_send_buffer(), myrank%255+1, get_send_buffer_usage());
#endif

  if (daccess & IO_SHARED_FILE_POINTER) {
    maxnumblocks = 0;
    for (i=0; i<numprocs; ++i) {
      maxnumblocks = MAX_INT(maxnumblocks,
		     (int)ceil((double)memsize.v[i]/(double)blocklen.v[i]));
    }

  } else {
    offsets = calc_offsets(io_dtype, memsize, blocklen, &maxnumblocks);
  }    


  amode = MPI_MODE_WRONLY | MPI_MODE_CREATE;

#ifndef IO_CHECK
  amode = MPI_MODE_DELETE_ON_CLOSE;
#endif
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);  
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  if (io_preallocate) {
    filesize = 0;
    for (i=0; i<memsize.n; ++i) {
      filesize += memsize.v[i];
    }
    preallocate_file_space(io_fh, io_dtype, filesize);
  }

  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization(); 

  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize.v[myrank];
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen.v[myrank]);
      msize -= blocklen.v[myrank];

      MPI_File_write_at(io_fh, offsets[i], get_send_buffer(), cnt, memtype,
			&status);
      ++i;
    }
    mpi_free(offsets);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize.v[myrank];
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen.v[myrank]);
      msize -= blocklen.v[myrank];

      MPI_File_seek(io_fh, offsets[i], MPI_SEEK_SET);
      MPI_File_write(io_fh, get_send_buffer(), cnt, memtype, &status);
      ++i;
    }
    mpi_free(offsets);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    msize = memsize.v[myrank];
    for (i=0; i<maxnumblocks; ++i) {
      if ((numprocs > 1) && ((i > 0) || (myrank > 0))) {
	MPI_Recv(NULL, 0, MPI_BYTE, (myrank-1+numprocs)%numprocs, 23,
		 get_measurement_comm(), &status);
      }
      if (msize > 0) {
	cnt = MIN_INT(msize, blocklen.v[myrank]);
	msize -= blocklen.v[myrank];

	MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype, &status);
      }
      if ((numprocs > 1) && ((i < maxnumblocks-1) || (myrank < numprocs-1))) {
	MPI_Send(NULL, 0, MPI_BYTE, (myrank+1)%numprocs, 23,
		 get_measurement_comm());
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize.v[myrank];
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen.v[myrank]);
      msize -= blocklen.v[myrank];

      MPI_File_iwrite_at(io_fh, offsets[i], get_send_buffer(), cnt, memtype, 
			 &request);
      MPI_Wait(&request, &status);
      ++i;
    }
    mpi_free(offsets);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {
 
    i = 0;
    msize = memsize.v[myrank];
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen.v[myrank]);
      msize -= blocklen.v[myrank];

      MPI_File_seek(io_fh, offsets[i], MPI_SEEK_SET);
      MPI_File_iwrite(io_fh, get_send_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
      ++i;
    }
    mpi_free(offsets);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {
    
    msize = memsize.v[myrank];
    for (i=0; i<maxnumblocks; ++i) {
      if ((numprocs > 1) && ((i > 0) || (myrank > 0))) {
	MPI_Recv(NULL, 0, MPI_BYTE, (myrank-1+numprocs)%numprocs, 23,
		 get_measurement_comm(), &status);
      }      
      if (msize > 0) {
	cnt = MIN_INT(msize, blocklen.v[myrank]);
	msize -= blocklen.v[myrank];

	MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype, &request); 
	MPI_Wait(&request, &status);
      }
      if ((numprocs > 1) && ((i < maxnumblocks-1) || (myrank < numprocs-1))) {
	MPI_Send(NULL, 0, MPI_BYTE, (myrank+1)%numprocs, 23,
		 get_measurement_comm());
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    nb = (memsize.v[myrank]/blocklen.v[myrank])+1;
    for (i=0; i<maxnumblocks; ++i) {
      if (i < nb-1) {
	cnt = blocklen.v[myrank];
      } else if (i == nb-1) {
	cnt = (memsize.v[myrank] % blocklen.v[myrank]);
      } else {
	cnt = 0;
      }
      MPI_File_write_at_all(io_fh, offsets[i], get_send_buffer(), cnt, memtype, 
			    &status);
    }
    mpi_free(offsets);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    nb = (memsize.v[myrank]/blocklen.v[myrank])+1;
    for (i=0; i<maxnumblocks; ++i) {
      if (i < nb-1) {
	cnt = blocklen.v[myrank];
      } else if (i == nb-1) {
	cnt = (memsize.v[myrank] % blocklen.v[myrank]);
      } else {
	cnt = 0;
      }
      MPI_File_seek(io_fh, offsets[i], MPI_SEEK_SET);
      MPI_File_write_all(io_fh, get_send_buffer(), cnt, memtype, &status);
    }
    mpi_free(offsets);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize.v[myrank];
    for (i=0; i<maxnumblocks; ++i) {
      if (msize > 0) {
	cnt = MIN_INT(msize, blocklen.v[myrank]);
	msize -= blocklen.v[myrank];
      } else {
	cnt = 0;
      }

      /* MPI_Barrier(get_measurement_comm()); */
      MPI_File_write_ordered(io_fh, get_send_buffer(), cnt, memtype, &status);
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    nb = (memsize.v[myrank]/blocklen.v[myrank])+1;
    for (i=0; i<maxnumblocks; ++i) {
      if (i < nb-1) {
	cnt = blocklen.v[myrank];
      } else if (i == nb-1) {
	cnt = (memsize.v[myrank] % blocklen.v[myrank]);
      } else {
	cnt = 0;
      }
      MPI_File_write_at_all_begin(io_fh, offsets[i], send_buffer, cnt, memtype);
      MPI_File_write_at_all_end(io_fh, send_buffer, &status);
    }
    mpi_free(offsets);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    nb = (memsize.v[myrank]/blocklen.v[myrank])+1;
    for (i=0; i<maxnumblocks; ++i) {
      if (i < nb-1) {
	cnt = blocklen.v[myrank];
      } else if (i == nb-1) {
	cnt = (memsize.v[myrank] % blocklen.v[myrank]);
      } else {
	cnt = 0;
      }
      MPI_File_seek(io_fh, offsets[i], MPI_SEEK_SET);
      MPI_File_write_all_begin(io_fh, send_buffer, cnt, memtype);
      MPI_File_write_all_end(io_fh, send_buffer, &status);
    }
    mpi_free(offsets);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    msize = memsize.v[myrank];
    for (i=0; i<maxnumblocks; ++i) {
      if (msize > 0) {
	cnt = MIN_INT(msize, blocklen.v[myrank]);
	msize -= blocklen.v[myrank];
      } else {
	cnt = 0;
      }
      /* MPI_Barrier(get_measurement_comm()); */
      MPI_File_write_ordered_begin(io_fh, send_buffer, cnt, memtype);
      MPI_File_write_ordered_end(io_fh, send_buffer, &status);
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();  
  MPI_File_close(&io_fh);

  return (end_time - start_time);
}

void finalize_MPI_IO_write_random(IArray memsize, IArray blocklen, 
				  MPI_Datatype memtype, MPI_Datatype etype,
				  MPI_Datatype filetype,
				  int daccess)
{
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_read_random
 *
 * @description
 * Measures the time to read from a file in a round-robin manner.
 * The values of `memsize' and `blocklen' may differ between the processes.
 *
 * Unlike MPI_IO_read_strided the blocks, that are read from the file,
 * are aligned, i.e. there is no stride between the end of a block and
 * the begin of the next block.
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            SUM( <memsize>_i ), i=0..n-1
 * memory size:          <memsize>_i, i=0..n-1
 * block size:           <blocklen>_i, i=0..n-1
 * access modes:         MPI_MODE_RDONLY
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer (all processes)
 * blocklen              chunk of memory that is read from file (all processes)
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_read_random(IArray memsize, IArray blocklen, 
			     MPI_Datatype memtype, MPI_Datatype etype, 
			     MPI_Datatype filetype,
			     int daccess)
{
  char       *send_buffer;
  MPI_Status status;
  int        myrank;
  int        msize;
  int        i;
  int        cnt;
  int        maxnumblocks;
  int        block_extent;
  int        displ;

  assert(memsize.n == blocklen.n);
  assert(memsize.n == get_measurement_size());

  myrank = get_measurement_rank();
  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  recv_buffer_init(memsize.v[myrank], memtype);

  block_extent = get_extent(blocklen.v[myrank], memtype);

  send_buffer = mpi_malloc_chars(get_extent(memsize.v[myrank], memtype));
#ifdef IO_CHECK
  io_chk_ref = buffer_fill((unsigned char *)send_buffer, memsize.v[myrank], 
			   memtype);
  io_chk_msg = 0;
#endif
  maxnumblocks = 0;
  for (i=0; i<memsize.n; ++i) {
    maxnumblocks = MAX_INT(maxnumblocks,
			   (int) ceil((double)memsize.v[i]/(double)blocklen.v[i]));
  }
  
  MPI_File_open(get_measurement_comm(), io_filename, 
		MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN,
		io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);  
  msize = memsize.v[myrank];
  for (i=0; i<maxnumblocks; ++i) {
    if (msize > 0) {
      cnt = MIN_INT(msize, blocklen.v[myrank]);
      msize -= blocklen.v[myrank];
      displ = i*block_extent;
    } else {
      cnt = 0;
      displ = 0;
    }
    MPI_File_write_ordered(io_fh, send_buffer+displ, cnt, memtype, &status);
  }
  MPI_File_close(&io_fh);
  mpi_free(send_buffer);      

  init_synchronization();
}

double measure_MPI_IO_read_random(IArray memsize, IArray blocklen, 
				  MPI_Datatype memtype, MPI_Datatype etype,
				  MPI_Datatype filetype,
				  int daccess)
{
  int         i;
  int         cnt;
  double      start_time, end_time;
  int         myrank, numprocs;
  MPI_Status  status; 
  MPI_Request request;
  MPI_Offset  *offsets = NULL;
  int         msize;
  char        *recv_buffer;
  int         maxnumblocks; 
  int         nb;
  int         amode;
#ifdef IO_CHECK
  long cs = 0l;
#endif

  myrank = get_measurement_rank();
  numprocs = get_measurement_size();

  if (daccess & IO_SHARED_FILE_POINTER) {
    maxnumblocks = 0;
    for (i=0; i<memsize.n; ++i) {
      maxnumblocks = MAX_INT(maxnumblocks,
		     (int) ceil((double)memsize.v[i]/(double)blocklen.v[i]));
    }
  } else {
    offsets = calc_offsets(io_dtype, memsize, blocklen, &maxnumblocks);
  }    
  
  amode = MPI_MODE_RDONLY;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();

  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize.v[myrank];
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen.v[myrank]);
      msize -= blocklen.v[myrank];

      MPI_File_read_at(io_fh, offsets[i], get_recv_buffer(), cnt, memtype,
		       &status);

#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }
    mpi_free(offsets);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize.v[myrank];
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen.v[myrank]);
      msize -= blocklen.v[myrank];

      MPI_File_seek(io_fh, offsets[i], MPI_SEEK_SET);
      MPI_File_read(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }
    mpi_free(offsets);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    msize = memsize.v[myrank];
    for (i=0; i<maxnumblocks; ++i) {
      if ((numprocs > 1) && ((i > 0) || (myrank > 0))) {
	MPI_Recv(NULL, 0, MPI_BYTE, (myrank-1+numprocs)%numprocs, 23,
		 get_measurement_comm(), &status);
      }
      if (msize > 0) {
	cnt = MIN_INT(msize, blocklen.v[myrank]);
	msize -= blocklen.v[myrank];

	MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype, &status); 
#ifdef IO_CHECK
	io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif
      }
      if ((numprocs > 1) && ((i < maxnumblocks-1) || (myrank < numprocs-1))) {
	MPI_Send(NULL, 0, MPI_BYTE, (myrank+1)%numprocs, 23,
		 get_measurement_comm());
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize.v[myrank];
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen.v[myrank]);
      msize -= blocklen.v[myrank];

      MPI_File_iread_at(io_fh, offsets[i], get_recv_buffer(), cnt,
			memtype, &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }
    mpi_free(offsets);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    i = 0;
    msize = memsize.v[myrank];
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen.v[myrank]);
      msize -= blocklen.v[myrank];

      MPI_File_seek(io_fh, offsets[i], MPI_SEEK_SET);
      MPI_File_iread(io_fh, get_recv_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }
    mpi_free(offsets);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize.v[myrank];
    for (i=0; i<maxnumblocks; ++i) {
      if ((numprocs > 1) && ((i > 0) || (myrank > 0))) {
	MPI_Recv(NULL, 0, MPI_BYTE, (myrank-1+numprocs)%numprocs, 23,
		 get_measurement_comm(), &status);
      }      
      if (msize > 0) {
	cnt = MIN_INT(msize, blocklen.v[myrank]);
	msize -= blocklen.v[myrank];

	MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt, memtype,
			      &request); 
	MPI_Wait(&request, &status);
#ifdef IO_CHECK
	io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      }
      if ((numprocs > 1) && ((i < maxnumblocks-1) || (myrank < numprocs-1))) {
	MPI_Send(NULL, 0, MPI_BYTE, (myrank+1)%numprocs, 23,
		 get_measurement_comm());
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    nb = (memsize.v[myrank]/blocklen.v[myrank])+1;
    for (i=0; i<maxnumblocks; ++i) {
      if (i < nb-1) {
	cnt = blocklen.v[myrank];
      } else if (i == nb-1) {
	cnt = (memsize.v[myrank] % blocklen.v[myrank]);
      } else {
	cnt = 0;
      }      
      MPI_File_read_at_all(io_fh, offsets[i], get_recv_buffer(), cnt, memtype,
			   &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }
    mpi_free(offsets);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    nb = (memsize.v[myrank]/blocklen.v[myrank])+1;
    for (i=0; i<maxnumblocks; ++i) {
      if (i < nb-1) {
	cnt = blocklen.v[myrank];
      } else if (i == nb-1) {
	cnt = (memsize.v[myrank] % blocklen.v[myrank]);
      } else {
	cnt = 0;
      }
      MPI_File_seek(io_fh, offsets[i], MPI_SEEK_SET);
      MPI_File_read_all(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }
    mpi_free(offsets);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize.v[myrank];
    for (i=0; i<maxnumblocks; ++i) {
      if (msize > 0) {
	cnt = MIN_INT(msize, blocklen.v[myrank]);
	msize -= blocklen.v[myrank];
      } else {
	cnt = 0;
      }

      /* MPI_Barrier(get_measurement_comm()); */
      MPI_File_read_ordered(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();
    nb = (memsize.v[myrank]/blocklen.v[myrank])+1;
    for (i=0; i<maxnumblocks; ++i) {
      if (i < nb-1) {
	cnt = blocklen.v[myrank];
      } else if (i == nb-1) {
	cnt = (memsize.v[myrank] % blocklen.v[myrank]);
      } else {
	cnt = 0;
      }
      MPI_File_read_at_all_begin(io_fh, offsets[i], recv_buffer, cnt, memtype);
      MPI_File_read_at_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }
    mpi_free(offsets);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();
    nb = (memsize.v[myrank]/blocklen.v[myrank])+1;
    for (i=0; i<maxnumblocks; ++i) {
      if (i < nb-1) {
	cnt = blocklen.v[myrank];
      } else if (i == nb-1) {
	cnt = (memsize.v[myrank] % blocklen.v[myrank]);
      } else {
	cnt = 0;
      }
      MPI_File_seek(io_fh, offsets[i], MPI_SEEK_SET);
      MPI_File_read_all_begin(io_fh, recv_buffer, cnt, memtype);
      MPI_File_read_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }
    mpi_free(offsets);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();
    msize = memsize.v[myrank];
    for (i=0; i<maxnumblocks; ++i) {
      if (msize > 0) {
	cnt = MIN_INT(msize, blocklen.v[myrank]);
	msize -= blocklen.v[myrank];
      } else {
	cnt = 0;
      }
      /* MPI_Barrier(get_measurement_comm()); */
      MPI_File_read_ordered_begin(io_fh, recv_buffer, cnt, memtype);
      MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif 
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();  
  MPI_File_close(&io_fh);

#ifdef IO_CHECK
  io_chk_compare(cs, "MPI_IO_read_random", daccess);
#endif /* IO_CHECK */
  
  return (end_time - start_time);
}

void finalize_MPI_IO_read_random(IArray memsize, IArray blocklen, 
				 MPI_Datatype memtype, MPI_Datatype etype,
				 MPI_Datatype filetype,
				 int daccess)
{
#ifndef IO_CHECK
  FILE_DELETE(io_filename);
#endif
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_write_noncontiguous
 *
 * @description
 * Measures the time to write data from memory buffer to a file using
 * noncontiguous filetypes of same extent.
 *
 * For the collective routines additional communication is necessary.
 *
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            n*<memsize>
 * memory size:          <memsize>
 * block size:           <blocklen>
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE | 
 *                       MPI_MODE_DELETE_ON_CLOSE
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             noncontiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of memory written to file
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                

 * *************************************************************************
 */
void init_MPI_IO_write_noncontiguous(int memsize, int blocklen, 
				     MPI_Datatype memtype, MPI_Datatype etype, 
				     MPI_Datatype filetype,
				     int daccess)
{
  assert(memsize > 0);
  assert(blocklen > 0);

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  send_buffer_init(memsize, memtype);

  init_synchronization();
}

double measure_MPI_IO_write_noncontiguous(int memsize, int blocklen, 
					  MPI_Datatype memtype, 
					  MPI_Datatype etype, 
					  MPI_Datatype filetype,
					  int daccess)
{
  double      start_time, end_time;
  int         myrank, numprocs;
  MPI_Request request;
  MPI_Status  status;
  int         msize;
  int         cnt;
  int         i;
  char        *send_buffer;
  int         n, nb, p;
  MPI_Offset  block_offset;
  MPI_Offset  disp, filesize;
  int         amode;

  myrank = get_measurement_rank();
  numprocs = get_measurement_size();

#ifdef IO_CHECK
  memset(get_send_buffer(), myrank%255+1, get_send_buffer_usage());
#endif  

  if (io_preallocate) {
#ifdef IO_CHECK
    FILE_DELETE(io_filename);
#endif   
      
    MPI_File_open(get_measurement_comm(), io_filename, 
		  MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN, 
		  io_info, &io_fh);
    MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		      func_get_io_datarep(), io_info);
    MPI_File_get_byte_offset(io_fh, get_offset(io_dtype, memsize), &disp);
    MPI_Allreduce(&disp, &filesize, 1, MPI_LONG_LONG, MPI_MAX,
		  get_measurement_comm());
    MPI_File_preallocate(io_fh, filesize);
    MPI_File_close(&io_fh);
  }

  block_offset = get_offset(io_dtype, blocklen);

  amode = MPI_MODE_WRONLY | MPI_MODE_CREATE;
#ifndef IO_CHECK
  amode = amode | MPI_MODE_DELETE_ON_CLOSE;
#endif
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();

  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_at(io_fh, i*block_offset, get_send_buffer(), cnt,
			memtype, &status);     
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write(io_fh, get_send_buffer(), cnt, memtype, &status);
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    for (i=0; i<numprocs; ++i) {
      /* synchronization is not guaranteed in MPI_File_seek_shared */
      MPI_Barrier(get_measurement_comm());
      MPI_File_seek_shared(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
      if (i == myrank) {
	msize = memsize;
	while (msize > 0) {
	  cnt = MIN_INT(msize, blocklen);
	  msize -= blocklen;

	  MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype, &status);
	}
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iwrite_at(io_fh, i*block_offset, get_send_buffer(), cnt,
			 memtype, &request);
      MPI_Wait(&request, &status);
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iwrite(io_fh, get_send_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    for (i=0; i<numprocs; ++i) {
      /* synchronization is not guaranteed in MPI_File_seek_shared */
      MPI_Barrier(get_measurement_comm());
      MPI_File_seek_shared(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
      if (i == myrank) {
	msize = memsize;
	while (msize > 0) {
	  cnt = MIN_INT(msize, blocklen);
	  msize -= blocklen;

	  MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype,
				 &request);
	  MPI_Wait(&request, &status);
	}
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;
      
      MPI_File_write_at_all(io_fh, i*block_offset, get_send_buffer(), cnt,
			    memtype, &status);
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_all(io_fh, get_send_buffer(), cnt, memtype, &status);
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    nb = (int) ceil((double)memsize/(double)blocklen); /* number of blocks */
    n = (memsize/blocklen)+1;
    p = myrank+1;                                      /* block position 1..n */

    for (i=0; i<numprocs; ++i) {
      while (p < nb+myrank+1) {
	if ((p <= 0) || (p > n)) {
	  cnt = 0;
	} else {	  
	  cnt = ((p > 0) && (p < n)) ? blocklen : (memsize % blocklen);
	}
	MPI_File_write_ordered(io_fh, get_send_buffer(), cnt, memtype, &status);
	p += numprocs;
      }

      /* MPI_Barrier(get_measurement_comm()); */
      MPI_File_seek_shared(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
      p = myrank-i;
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_at_all_begin(io_fh, i*block_offset, send_buffer, cnt,
				  memtype);
      ++i;
      MPI_File_write_at_all_end(io_fh, send_buffer, &status);
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_all_begin(io_fh, send_buffer, cnt, memtype);
      ++i;
      MPI_File_write_all_end(io_fh, send_buffer, &status);
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();    
    nb = (int) ceil((double)memsize/(double)blocklen); /* number of blocks */
    n = (memsize/blocklen)+1;
    p = myrank+1;                                      /* block position 1..n */
        
    for (i=0; i<numprocs; ++i) {
      while (p < nb+myrank+1) {
	if ((p <= 0) || (p > n)) {
	  cnt = 0;
	} else {	  
	  cnt = ((p > 0) && (p < n)) ? blocklen : (memsize % blocklen);
	}
	MPI_File_write_ordered_begin(io_fh, send_buffer, cnt, memtype);
	p += numprocs;
	MPI_File_write_ordered_end(io_fh, send_buffer, &status);
      }

      /* MPI_Barrier(get_measurement_comm()); */
      MPI_File_seek_shared(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
      p = myrank-i;
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();  
  MPI_File_close(&io_fh);

  return (end_time - start_time);
}

void finalize_MPI_IO_write_noncontiguous(int memsize, int blocklen, 
					 MPI_Datatype memtype,
					 MPI_Datatype etype,
					 MPI_Datatype filetype,
					 int daccess)
{
  mpi_free(io_filename);
}

/* *************************************************************************
 * MPI_IO_read_noncontiguous
 *
 * @description
 * Measures the time to read data from a file to the memory buffer using
 * noncontiguous filetypes of same extent.
 *
 * For the collective routines additional communication is necessary.
 * 
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on one file)
 * file size:            n*<memsize>
 * memory size:          <memsize>
 * block size:           <blocklen>
 * access modes:         MPI_MODE_RDONLY
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             noncontiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of file read to memory
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_read_noncontiguous(int memsize, int blocklen, 
				    MPI_Datatype memtype, MPI_Datatype etype,
				    MPI_Datatype filetype,
				    int daccess)
{
  MPI_Aint   block_extent;
  char       *send_buffer;
  MPI_Status status;
  int        i, cnt;

  assert(memsize > 0);
  assert(blocklen > 0);

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  recv_buffer_init(memsize, memtype);

  block_extent = get_extent(blocklen, memtype);
  send_buffer = mpi_malloc_chars(get_extent(memsize, memtype));
#ifdef IO_CHECK
  io_chk_ref = buffer_fill((unsigned char *)send_buffer, memsize, memtype);
  io_chk_msg = 0;
#endif    
  
  MPI_File_open(get_measurement_comm(), io_filename, 
		MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN,
		io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);

  i = 0;
  while (memsize > 0) {
    cnt = MIN_INT(memsize, blocklen);
    memsize -= blocklen;

    MPI_File_write(io_fh, send_buffer+i*block_extent, cnt, memtype, &status);
    ++i;
  }
  MPI_File_close(&io_fh);
  mpi_free(send_buffer);      

  init_synchronization();
}

double measure_MPI_IO_read_noncontiguous(int memsize, int blocklen, 
					 MPI_Datatype memtype, 
					 MPI_Datatype etype, 
					 MPI_Datatype filetype,
					 int daccess)
{
  double      start_time, end_time;
  int         myrank, numprocs;
  MPI_Request request;
  MPI_Status  status;
  int         msize;
  int         cnt;
  int         i;
  MPI_Offset  block_offset;
  char        *recv_buffer;
  int         n, nb, p;
  int         amode;

#ifdef IO_CHECK
  long cs = 0l;
#endif

  myrank = get_measurement_rank();
  numprocs = get_measurement_size();

  block_offset = get_offset(io_dtype, blocklen);

  amode = MPI_MODE_RDONLY;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  /* start measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();

  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at(io_fh, i*block_offset, get_recv_buffer(), cnt, memtype,
		       &status);     
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    for (i=0; i<numprocs; ++i) {
      /* synchronization is not guaranteed in MPI_File_seek_shared */
      MPI_Barrier(get_measurement_comm());
      MPI_File_seek_shared(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
      if (i == myrank) {
	msize = memsize;
	while (msize > 0) {
	  cnt = MIN_INT(msize, blocklen);
	  msize -= blocklen;

	  MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
	  io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
	}
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iread_at(io_fh, i*block_offset, get_recv_buffer(), cnt,
			memtype, &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iread(io_fh, get_recv_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    for (i=0; i<numprocs; ++i) {
      /* synchronization is not guaranteed in MPI_File_seek_shared */
      MPI_Barrier(get_measurement_comm());
      MPI_File_seek_shared(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
      if (i == myrank) {
	msize = memsize;
	while (msize > 0) {
	  cnt = MIN_INT(msize, blocklen);
	  msize -= blocklen;

	  MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt, memtype,
				&request);
	  MPI_Wait(&request, &status);
#ifdef IO_CHECK
	  io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
	}
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at_all(io_fh, i*block_offset, get_recv_buffer(), cnt,
			   memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_all(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    nb = (int) ceil((double)memsize/(double)blocklen); /* number of blocks */
    n = (memsize/blocklen)+1;
    p = myrank+1;                                      /* block position 1..n */

    for (i=0; i<numprocs; ++i) {
      while (p < nb+myrank+1) {
	if ((p <= 0) || (p > n)) {
	  cnt = 0;
	} else {	  
	  cnt = ((p > 0) && (p < n)) ? blocklen : (memsize % blocklen);
	}
	MPI_File_read_ordered(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
	io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
	p += numprocs;
      }

      /* MPI_Barrier(get_measurement_comm()); */
      MPI_File_seek_shared(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
      p = myrank-i;
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {
    
    recv_buffer = get_recv_buffer();
    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at_all_begin(io_fh, i*block_offset, recv_buffer, cnt,
				 memtype);
      ++i;
      MPI_File_read_at_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();
    i = 0;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_all_begin(io_fh, recv_buffer, cnt, memtype);
      ++i;
      MPI_File_read_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();
    nb = (int) ceil((double)memsize/(double)blocklen); /* number of blocks */
    n = (memsize/blocklen)+1;
    p = myrank+1;                                      /* block position 1..n */
        
    for (i=0; i<numprocs; ++i) {
      while (p < nb+myrank+1) {
	if ((p <= 0) || (p > n)) {
	  cnt = 0;
	} else {	  
	  cnt = ((p > 0) && (p < n)) ? blocklen : (memsize % blocklen);
	}
	MPI_File_read_ordered_begin(io_fh, recv_buffer, cnt, memtype);
	p += numprocs;
	MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
	io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      }

      /* MPI_Barrier(get_measurement_comm()); */
      MPI_File_seek_shared(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
      p = myrank-i;
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();  
  MPI_File_close(&io_fh);

#ifdef IO_CHECK
  io_chk_compare(cs, "MPI_IO_read_noncontiguous", daccess);
#endif /* IO_CHECK */

  return (end_time - start_time);
}

void finalize_MPI_IO_read_noncontiguous(int memsize, int blocklen, 
					MPI_Datatype memtype, MPI_Datatype etype,
					MPI_Datatype filetype,
					int daccess)
{
#ifndef IO_CHECK
  FILE_DELETE(io_filename);
#endif
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_write_separate
 *
 * @description
 * Measures the time to write to different files.
 * 
 * All processes have to call this measurement with identical parameter
 * values.
 *
 * process group:        MPI_COMM_SELF
 * group-files ratio:    1:1 (each process operates on one file)
 * file size:            <memsize>
 * memory size:          <memsize>
 * block size:           <blocklen>
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE
 *                       MPI_MODE_DELETE_ON_CLOSE [| MPI_MODE_UNIQUE_OPEN]
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous 
 * filetype:             contiguous or noncontiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of memory that is written to file
 * stride                number of elements between start of blocks
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_write_separate(int memsize, int blocklen, int stride, 
				MPI_Datatype memtype, MPI_Datatype etype, 
				MPI_Datatype filetype, int daccess)
{
  assert(memsize > 0);
  assert(blocklen > 0);
  assert(stride >= blocklen);

  io_filename = get_io_filename(FILENAME, get_measurement_rank());
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  send_buffer_init(memsize, memtype);

  init_synchronization();
}

double measure_MPI_IO_write_separate(int memsize, int blocklen, int stride, 
				     MPI_Datatype memtype, MPI_Datatype etype,
				     MPI_Datatype filetype, int daccess)
{
  int         i;
  int         cnt;
  double      start_time, end_time;
  int         filesize; 
  MPI_Offset  stride_offset;
  MPI_Status  status; 
  MPI_Request request;
  char        *send_buffer;
  int         msize;
  int         amode;

  stride_offset = get_offset(io_dtype, stride);

  amode = MPI_MODE_WRONLY | MPI_MODE_CREATE;
#ifndef IO_CHECK
  amode = amode | MPI_MODE_DELETE_ON_CLOSE;
#endif
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(MPI_COMM_SELF, io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  if (io_preallocate)  {    
    filesize = (int) (stride*ceil(((double) memsize)/((double) blocklen)));
    preallocate_file_space(io_fh, io_dtype, filesize);
  }

  /* begin measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();
  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_at(io_fh, i*stride_offset, get_send_buffer(), cnt,
			memtype, &status);
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {
    
    MPI_File_seek(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_write(io_fh, get_send_buffer(), cnt, memtype, &status);
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek_shared(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype, &status);
      ++i;
    }
    
  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iwrite_at(io_fh, i*stride_offset, get_send_buffer(), cnt,
			 memtype, &request);
      MPI_Wait(&request, &status);
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    MPI_File_seek(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_iwrite(io_fh, get_send_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek_shared(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
      ++i;
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_at_all(io_fh, i*stride_offset, get_send_buffer(), cnt,
			    memtype, &status);
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    MPI_File_seek(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_write_all(io_fh, get_send_buffer(), cnt, memtype, &status);
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek_shared(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_write_ordered(io_fh, get_send_buffer(), cnt, memtype, &status);
      ++i;
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_at_all_begin(io_fh, i*stride_offset, send_buffer, cnt,
				  memtype);
      MPI_File_write_at_all_end(io_fh, send_buffer, &status);
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();

    MPI_File_seek(io_fh, (MPI_Offset)0, MPI_SEEK_SET);
    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_write_all_begin(io_fh, send_buffer, cnt, memtype);
      MPI_File_write_all_end(io_fh, send_buffer, &status);
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek_shared(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_write_ordered_begin(io_fh, send_buffer, cnt, memtype);
      MPI_File_write_ordered_end(io_fh, send_buffer, &status);
      ++i;
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();
  MPI_File_close(&io_fh);

  return (end_time - start_time);
}

void finalize_MPI_IO_write_separate(int memsize, int blocklen, int stride, 
				    MPI_Datatype memtype, MPI_Datatype etype,
				    MPI_Datatype filetype, int daccess)
{
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_read_separate
 *
 * @description
 * Measures the time to read from different files.
 *
 * All processes have to call this measurement with identical parameter
 * values.
 *
 * process group:        MPI_COMM_SELF
 * group-files ratio:    1:1 (each process operates on own file)
 * file size:            <memsize>
 * memory size:          <memsize>
 * block size:           <blocklen>
 * access modes:         MPI_MODE_RDONLY [| MPI_MODE_UNIQUE_OPEN]
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous 
 * filetype:             contiguous or noncontiguous 
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of file read to memory
 * stride                number of elements between start of blocks
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_read_separate(int memsize, int blocklen, int stride, 
			       MPI_Datatype memtype, MPI_Datatype etype,
			       MPI_Datatype filetype, int daccess)
{
  int        i;
  int        cnt;
  char       *send_buffer;
  MPI_Aint   block_extent;
  int        msize;
  MPI_Status status;

  assert(memsize > 0);
  assert(blocklen > 0);
  assert(stride >= blocklen);

  io_filename = get_io_filename(FILENAME, get_measurement_rank());
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  recv_buffer_init(memsize, memtype);

  block_extent = get_extent(blocklen, memtype);
  send_buffer = mpi_malloc_chars(get_extent(memsize, memtype));
#ifdef IO_CHECK
  io_chk_ref  = buffer_fill((unsigned char *)send_buffer, memsize, memtype);
  io_chk_msg = 0;
#endif

  MPI_File_open(MPI_COMM_SELF, io_filename, 
		MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN, 
		io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  if (stride > blocklen) {
    i = 0;
    msize = memsize;
    while (msize > 0) { 
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;
      
      MPI_File_write(io_fh, send_buffer+i*block_extent, cnt, memtype, &status);
      MPI_File_seek(io_fh, get_offset(io_dtype, stride-cnt), MPI_SEEK_CUR);

      ++i;
    }
  } else {
    MPI_File_write(io_fh, send_buffer, memsize, memtype, &status);
  }
  MPI_File_close(&io_fh);
  mpi_free(send_buffer);

  init_synchronization();
}

double measure_MPI_IO_read_separate(int memsize, int blocklen, int stride, 
				    MPI_Datatype memtype, MPI_Datatype etype,
				    MPI_Datatype filetype, int daccess)
{
  int         i;
  int         cnt;
  double      start_time, end_time;
  MPI_Offset  stride_offset;
  MPI_Status  status; 
  MPI_Request request;
  char        *recv_buffer;
  int         amode;
  int         msize;
#ifdef IO_CHECK
  long cs = 0l;
#endif
  stride_offset = get_offset(io_dtype, stride);

  amode = MPI_MODE_RDONLY;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(MPI_COMM_SELF, io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  /* begin measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();
  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at(io_fh, i*stride_offset, get_recv_buffer(), cnt,
		       memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_read(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek_shared(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }
    
  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iread_at(io_fh, i*stride_offset, get_recv_buffer(), cnt,
			memtype, &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_iread(io_fh, get_recv_buffer(), cnt, memtype,
		     &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek_shared(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt, memtype,
			    &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at_all(io_fh, i*stride_offset, get_recv_buffer(), cnt,
			   memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_read_all(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek_shared(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_read_ordered(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_at_all_begin(io_fh, i*stride_offset, recv_buffer, cnt,
				 memtype);
      MPI_File_read_at_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_read_all_begin(io_fh, recv_buffer, cnt, memtype);
      MPI_File_read_all_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (stride > blocklen) {
	MPI_File_seek_shared(io_fh, i*stride_offset, MPI_SEEK_SET);
      }

      MPI_File_read_ordered_begin(io_fh, recv_buffer, cnt, memtype);
      MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
      ++i;
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }
 
  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();
  MPI_File_close(&io_fh);

#ifdef IO_CHECK
  io_chk_compare(cs, "MPI_IO_read_separate", daccess);
#endif /* IO_CHECK */

  return (end_time - start_time);
}

void finalize_MPI_IO_read_separate(int memsize, int blocklen, int stride, 
				   MPI_Datatype memtype, MPI_Datatype etype,
				   MPI_Datatype filetype, int daccess)
{
#ifndef IO_CHECK
  FILE_DELETE(io_filename);
#endif
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_write_sequential
 *
 * @description
 * Measures the time to write to a file in sequential mode.
 *
 * From MPI-2, 9.6.2 Random Access vs. Sequential Mode:
 * "For these files, the only permitted data access operations are shared
 *  file pointer reads and writes. Filetypes and etypes with holes are 
 *  erroneous."
 * 
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on same file)
 * file size:            SUM ( <memsize>_i ), i=1..n-1
 * memory size:          <memsize>_i , i=1..n-1
 * block size:           <blocklen>_i, i=1..n-1
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE
 *                       MPI_MODE_DELETE_ON_CLOSE | MPI_MODE_SEQUENTIAL
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of memory that is written to file
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_write_sequential(int memsize, int blocklen,
				  MPI_Datatype memtype, MPI_Datatype etype,
				  MPI_Datatype filetype, 
				  int daccess)
{
  assert(memsize > 0);
  assert(blocklen > 0);

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  send_buffer_init(memsize, memtype);

  init_synchronization();
}

double measure_MPI_IO_write_sequential(int memsize, int blocklen,
				       MPI_Datatype memtype,
				       MPI_Datatype etype,
				       MPI_Datatype filetype, 
				       int daccess)
{
  int         i;
  double      start_time, end_time;
  int         filesize;
  int         amode;
  int         msize, cnt;
  MPI_Status  status;
  MPI_Request request;
  char        *send_buffer;

#ifdef IO_CHECK
  memset(get_send_buffer(), get_measurement_rank()%255+1, get_send_buffer_usage());
#endif
   
  amode = MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_SEQUENTIAL;
#ifndef IO_CHECK
  amode = amode | MPI_MODE_DELETE_ON_CLOSE;
#endif
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)MPI_DISPLACEMENT_CURRENT, etype, 
		    filetype, func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  if (io_preallocate) {
    filesize = memsize * get_measurement_size();
    preallocate_file_space(io_fh, io_dtype, filesize);
  }

 /* begin measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();

  if (daccess == (IO_SHARED_FILE_POINTER |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype, &status);
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype, &request);
      MPI_Wait(&request, &status);
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_ordered(io_fh, get_send_buffer(), cnt, memtype, &status);
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_write_ordered_begin(io_fh, send_buffer, cnt, memtype);
      MPI_File_write_ordered_end(io_fh, send_buffer, &status);
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();
  MPI_File_close(&io_fh);

  return (end_time - start_time);
}

void finalize_MPI_IO_write_sequential(int memsize, int blocklen,
				      MPI_Datatype memtype, MPI_Datatype etype,
				      MPI_Datatype filetype, 
				      int daccess)
{
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_read_sequential
 *
 * @description
 * Measures the time to read from a file in sequential mode.
 * 
 * From MPI-2, 9.6.2 Random Access vs. Sequential Mode:
 * "For these files, the only permitted data access operations are shared
 *  file pointer reads and writes. Filetypes and etypes with holes are 
 *  erroneous."
 * 
 * process group:        <get_measurement_comm()>
 * group-files ratio:    n:1 (all processes operate on same file)
 * file size:            SUM ( <memsize>_i ), i=1..n-1
 * memory size:          <memsize>_i , i=1..n-1
 * block size:           <blocklen>_i, i=1..n-1
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE
 *                       MPI_MODE_DELETE_ON_CLOSE | MPI_MODE_SEQUENTIAL
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of memory that is written to file
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_read_sequential(int memsize, int blocklen,
				 MPI_Datatype memtype, MPI_Datatype etype,
				 MPI_Datatype filetype, 
				 int daccess)
{
  char       *send_buffer;
  MPI_Status status;
#ifdef IO_CHECK
  long       sum;
#endif
  assert(memsize > 0);
  assert(blocklen > 0);

  io_filename = get_io_filename(FILENAME, 0);
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  recv_buffer_init(memsize, memtype);

  send_buffer = mpi_malloc_chars(get_extent(memsize, memtype));
#ifdef IO_CHECK
  sum = buffer_fill((unsigned char *)send_buffer, memsize, memtype);
  io_chk_ref = 0l;
  MPI_Reduce(&sum, &io_chk_ref, 1, MPI_LONG, MPI_SUM, 0, get_measurement_comm());
  io_chk_msg = 0;
#endif

  MPI_File_open(get_measurement_comm(), io_filename,
		MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN, 
		io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_write_ordered(io_fh, send_buffer, memsize, memtype, &status);
  MPI_File_close(&io_fh);
  mpi_free(send_buffer);

  init_synchronization();
}

double measure_MPI_IO_read_sequential(int memsize, int blocklen,
				      MPI_Datatype memtype, MPI_Datatype etype,
				      MPI_Datatype filetype,
				      int daccess)
{
  int         i;
  double      start_time, end_time;
  int         msize, cnt;
  MPI_Status  status;
  MPI_Request request;
  char        *recv_buffer;
  int         amode;

#ifdef IO_CHECK
  long cs   = 0l;
  long csum = 0l;
#endif

  amode = MPI_MODE_RDONLY | MPI_MODE_SEQUENTIAL;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(get_measurement_comm(), io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)MPI_DISPLACEMENT_CURRENT, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  /* begin measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();
  if (daccess == (IO_SHARED_FILE_POINTER |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt, memtype,
			    &request);
      MPI_Wait(&request, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char *)get_recv_buffer(), cnt, memtype, &cs);
#endif      
      ++i;
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_ordered(io_fh, get_recv_buffer(), cnt, memtype, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    recv_buffer = get_recv_buffer();
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_File_read_ordered_begin(io_fh, recv_buffer, cnt, memtype);
      MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
#ifdef IO_CHECK
      io_chk_add_sum((unsigned char*)get_recv_buffer(), cnt, memtype, &cs);
#endif /* IO_CHECK */    
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }

  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();
  MPI_File_close(&io_fh);

#ifdef IO_CHECK
  MPI_Reduce(&cs, &csum, 1, MPI_LONG, MPI_SUM, 0, get_measurement_comm());
  if (get_measurement_rank() != 0) {
    csum = io_chk_ref;
  }
  io_chk_compare(csum, "MPI_IO_read_sequential", daccess);
#endif /* IO_CHECK */

  return (end_time - start_time);
}

void finalize_MPI_IO_read_sequential(int memsize, int blocklen,
				     MPI_Datatype memtype, MPI_Datatype etype,
				     MPI_Datatype filetype,
				     int daccess)
{
#ifndef IO_CHECK
  FILE_DELETE(io_filename);
#endif
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_write_and_read
 *
 * @description
 * Measures the time to write and read a file, i.e. the time in which a pair
 * of processes accesses to a file, where one of them performs a write and 
 * the other a read operation.
 * 
 * process group:        io_comm2
 * number of processes:  2*g (g: number of process groups) 
 * group-files ratio:    1:1 (each group of processes operates on its own file)
 * file size:            <memsize> 
 * memory size:          <memsize>
 * block size:           <blocklen>
 * access modes:         MPI_MODE_RDWR | MPI_MODE_CREATE |
 *                       MPI_MODE_DELETE_ON_CLOSE
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous or noncontiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * memsize               global size of memory buffer
 * blocklen              chunk of memory that is written to/read from file
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_write_and_read(int memsize, int blocklen, 
				MPI_Datatype memtype, MPI_Datatype etype,
				MPI_Datatype filetype, int daccess)
{
  MPI_Aint   buffer_usage;
  int        type_size;

  assert(memsize > 0);
  assert(blocklen > 0);
  assert(get_measurement_size()%2 == 0);
#ifdef IO_CHECK
  assert(is_equal(memsize));
  assert(is_equal(blocklen));
#endif

  io_filename = get_io_filename(FILENAME, get_measurement_rank()/2);
  datatype_param_init(&io_dtype, memtype, etype, filetype);

  buffer_usage = get_extent( blocklen, memtype );
  if (get_measurement_rank() % 2) {
    set_send_buffer_usage( buffer_usage );
  } else {
    set_recv_buffer_usage( buffer_usage );
  }

  /* data transferred by all process */
  MPI_Type_size( memtype, &type_size );
  set_reported_message_size( type_size * memsize * get_measurement_size() ); 

  
  MPI_Comm_split(get_measurement_comm(), (get_measurement_rank()/2), 
		 get_measurement_rank(), &io_comm2);

  init_synchronization();
}

double measure_MPI_IO_write_and_read(int memsize, int blocklen, 
				     MPI_Datatype memtype, MPI_Datatype etype,
				     MPI_Datatype filetype, int daccess)
{
  double      start_time, end_time;
  int         i;
  int         myrank;
  int         msize, cnt;
  MPI_Offset  block_offset;
  MPI_Status  status;
  MPI_Request request;
  int         amode;
  char        *send_buffer, *recv_buffer;
  MPI_Offset  pos;

  if (get_measurement_size()%2 != 0) {
    return -1;
  }

  myrank   = get_measurement_rank();
  block_offset = get_offset(io_dtype, blocklen);
  
  amode = MPI_MODE_RDWR | MPI_MODE_CREATE;
#ifndef IO_CHECK
  amode = amode | MPI_MODE_DELETE_ON_CLOSE;
#endif
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(io_comm2, io_filename, amode, io_info, &io_fh);

  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  if (io_preallocate) {
    preallocate_file_space(io_fh, io_dtype, memsize);
  }

  /* begin measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();

  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {
    
    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (myrank % 2) { /* write */
	MPI_File_write_at(io_fh, i*block_offset, get_send_buffer(), cnt,
			  memtype, &status);

	if (!io_atomicity) MPI_File_sync(io_fh);	  
	/* alternatively use MPI_Barrier(io_comm2); */
	MPI_Send(NULL, 0, MPI_BYTE, 0, 17, io_comm2); 
	if (!io_atomicity) MPI_File_sync(io_fh);	  

      } else { /* read */

	if (!io_atomicity) MPI_File_sync(io_fh);	  
	/* alternatively use MPI_Barrier(io_comm2); */
	MPI_Recv(NULL, 0, MPI_BYTE, 1, 17, io_comm2, &status); 
	if (!io_atomicity) MPI_File_sync(io_fh);	    
	
	MPI_File_read_at(io_fh, i*block_offset, get_recv_buffer(), cnt,
			 memtype, &status);	  
      }
      ++i;	
    }
      
  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

      
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;
      
      if (myrank % 2) { /* write */	
	MPI_File_write(io_fh, get_send_buffer(), cnt, memtype, &status);
	
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Send(NULL, 0, MPI_BYTE, 0, 17, io_comm2);
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	
      } else { /* read */
	
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Recv(NULL, 0, MPI_BYTE, 1, 17, io_comm2, &status);
	if (!io_atomicity) MPI_File_sync(io_fh);	    
	
	MPI_File_read(io_fh, get_recv_buffer(), cnt, memtype, &status);
      }
    }


  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    pos = (MPI_Offset)0;

    i=0;
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_Barrier(io_comm2);
      MPI_File_get_position_shared(io_fh, &pos);
      MPI_Barrier(io_comm2);

      if (myrank % 2) { /* write */	
	MPI_File_write_shared(io_fh, get_send_buffer(), cnt, memtype, &status);
	
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Send(NULL, 0, MPI_BYTE, 0, 17, io_comm2);	
	if (!io_atomicity) MPI_File_sync(io_fh);	  

	MPI_File_seek_shared(io_fh, pos, MPI_SEEK_SET);
	
      } else { /* read */
	
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Recv(NULL, 0, MPI_BYTE, 1, 17, io_comm2, &status);
	if (!io_atomicity) MPI_File_sync(io_fh);	    

	MPI_File_seek_shared(io_fh, pos, MPI_SEEK_SET);
		
	MPI_File_read_shared(io_fh, get_recv_buffer(), cnt, memtype, &status); 
      }
      ++i;
    }


  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {
    

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      if (myrank % 2) { /* write */

	MPI_File_iwrite_at(io_fh, i*block_offset, get_send_buffer(), cnt,
			   memtype, &request);
	MPI_Wait(&request, &status);

	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Send(NULL, 0, MPI_BYTE, 0, 17, io_comm2);
	if (!io_atomicity) MPI_File_sync(io_fh);	  

      } else { /* read */

	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Recv(NULL, 0, MPI_BYTE, 1, 17, io_comm2, &status);
	if (!io_atomicity) MPI_File_sync(io_fh);	    

	MPI_File_iread_at(io_fh, i*block_offset, get_recv_buffer(), cnt,
			  memtype, &request);
	MPI_Wait(&request, &status);
      }
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;
      
      if (myrank % 2) { /* write */	
	MPI_File_iwrite(io_fh, get_send_buffer(), cnt, memtype, &request);
	MPI_Wait(&request, &status);
	
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Send(NULL, 0, MPI_BYTE, 0, 17, io_comm2);
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	
      } else { /* read */
	
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Recv(NULL, 0, MPI_BYTE, 1, 17, io_comm2, &status);
	if (!io_atomicity) MPI_File_sync(io_fh);	    
	
	MPI_File_iread(io_fh, get_recv_buffer(), cnt, memtype, &request);
	MPI_Wait(&request, &status);
      }
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;
      
      MPI_Barrier(io_comm2);
      MPI_File_get_position_shared(io_fh, &pos);
      MPI_Barrier(io_comm2);

      if (myrank % 2) { /* write */	
	MPI_File_iwrite_shared(io_fh, get_send_buffer(), cnt, memtype,
			       &request);
	MPI_Wait(&request, &status);
	
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Send(NULL, 0, MPI_BYTE, 0, 17, io_comm2);	
	if (!io_atomicity) MPI_File_sync(io_fh);

	MPI_File_seek_shared(io_fh, pos, MPI_SEEK_SET);
	
	MPI_Recv(NULL, 0, MPI_BYTE, 0, 19, io_comm2, &status);
      } else { /* read */
	
	if (!io_atomicity) MPI_File_sync(io_fh);	  
	MPI_Recv(NULL, 0, MPI_BYTE, 1, 17, io_comm2, &status);
	if (!io_atomicity) MPI_File_sync(io_fh);	    

	MPI_File_seek_shared(io_fh, pos, MPI_SEEK_SET);
	
	MPI_File_iread_shared(io_fh, get_recv_buffer(), cnt, memtype, &request);	  
	MPI_Wait(&request, &status);

	MPI_Send(NULL, 0, MPI_BYTE, 1, 19, io_comm2);
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;
      if (myrank % 2) {
	MPI_File_write_at_all(io_fh, i*block_offset, get_send_buffer(), cnt,
			      memtype, &status);
      } else {
	MPI_File_write_at_all(io_fh, i*block_offset, get_send_buffer(), 0,
			      memtype, &status);
      }
      
      if (!io_atomicity) MPI_File_sync(io_fh);
      MPI_Barrier(io_comm2);
      if (!io_atomicity) MPI_File_sync(io_fh);	  
      
      if (myrank % 2) {
	MPI_File_read_at_all(io_fh, i*block_offset, get_recv_buffer(), 0,
			     memtype, &status);
      } else {
	MPI_File_read_at_all(io_fh, i*block_offset, get_recv_buffer(), cnt,
			     memtype, &status);
      }
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;
      
      if (myrank % 2) {
	MPI_File_write_all(io_fh, get_send_buffer(), cnt, memtype, &status);
      } else {
	MPI_File_write_all(io_fh, get_send_buffer(), 0, memtype, &status);
      }
	
      if (!io_atomicity) MPI_File_sync(io_fh);	  
      MPI_Barrier(io_comm2);
      if (!io_atomicity) MPI_File_sync(io_fh);	  
	
      if (myrank % 2) {
	MPI_File_read_all(io_fh, get_recv_buffer(), 0, memtype, &status);
      } else {
	MPI_File_read_all(io_fh, get_recv_buffer(), cnt, memtype, &status);
      }
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_Barrier(io_comm2);
      MPI_File_get_position_shared(io_fh, &pos);
      MPI_Barrier(io_comm2);
      
      if (myrank % 2) {
	MPI_File_write_ordered(io_fh, get_send_buffer(), cnt, memtype, &status);
      } else {
	MPI_File_write_ordered(io_fh, get_send_buffer(), 0, memtype, &status);
      }
	
      if (!io_atomicity) MPI_File_sync(io_fh);	  
      MPI_Barrier(io_comm2);
      if (!io_atomicity) MPI_File_sync(io_fh);	  

      MPI_File_seek_shared(io_fh, pos, MPI_SEEK_SET);
	
      if (myrank % 2) {
	MPI_File_read_ordered(io_fh, get_recv_buffer(), 0, memtype, &status);
      } else {
	MPI_File_read_ordered(io_fh, get_recv_buffer(), cnt, memtype, &status);
      }
    }

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    recv_buffer = get_recv_buffer();

    msize = memsize;
    i = 0;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;
      if (myrank % 2) {
	MPI_File_write_at_all_begin(io_fh, i*block_offset, send_buffer, cnt,
				    memtype);
      } else {
	MPI_File_write_at_all_begin(io_fh, i*block_offset, send_buffer, 0,
				    memtype);
      }
      MPI_File_write_at_all_end(io_fh, send_buffer, &status);

      if (!io_atomicity) MPI_File_sync(io_fh);
      MPI_Barrier(io_comm2);
      if (!io_atomicity) MPI_File_sync(io_fh);	  
      
      if (myrank % 2) {
	MPI_File_read_at_all_begin(io_fh, i*block_offset, recv_buffer, 0,
				   memtype);
      } else {
	MPI_File_read_at_all_begin(io_fh, i*block_offset, recv_buffer, cnt,
				   memtype);
      }
      MPI_File_read_at_all_end(io_fh, recv_buffer, &status);
      ++i;
    }

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    recv_buffer = get_recv_buffer();
    
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;
      
      if (myrank % 2) {
	MPI_File_write_all_begin(io_fh, send_buffer, cnt, memtype);
      } else {
	MPI_File_write_all_begin(io_fh, send_buffer, 0, memtype);
      }
      MPI_File_write_all_end(io_fh, send_buffer, &status);

      if (!io_atomicity) MPI_File_sync(io_fh);	  
      MPI_Barrier(io_comm2);
      if (!io_atomicity) MPI_File_sync(io_fh);	  
	
      if (myrank % 2) {
	MPI_File_read_all_begin(io_fh, recv_buffer, 0, memtype);
      } else {
	MPI_File_read_all_begin(io_fh, recv_buffer, cnt, memtype);
      }
      MPI_File_read_all_end(io_fh, recv_buffer, &status);
    }

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    recv_buffer = get_recv_buffer();
    
    msize = memsize;
    while (msize > 0) {
      cnt = MIN_INT(msize, blocklen);
      msize -= blocklen;

      MPI_Barrier(io_comm2);
      MPI_File_get_position_shared(io_fh, &pos);
      MPI_Barrier(io_comm2);
      
      if (myrank % 2) {
	MPI_File_write_ordered_begin(io_fh, send_buffer, cnt, memtype);
      } else {
	MPI_File_write_ordered_begin(io_fh, send_buffer, 0, memtype);
      }
      MPI_File_write_ordered_end(io_fh, send_buffer, &status);

      if (!io_atomicity) MPI_File_sync(io_fh);	  
      MPI_Barrier(io_comm2);
      if (!io_atomicity) MPI_File_sync(io_fh);

      MPI_File_seek_shared(io_fh, pos, MPI_SEEK_SET);
	
      if (myrank % 2) {
	MPI_File_read_ordered_begin(io_fh, recv_buffer, 0, memtype);
      } else {
	MPI_File_read_ordered_begin(io_fh, recv_buffer, cnt, memtype);
      }
      MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
    }

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  } 
   
  if (io_file_sync) {
    MPI_File_sync(io_fh); /* assure completion of all transfers */
  }
  end_time = stop_synchronization();
  MPI_File_close(&io_fh);

  return (end_time - start_time);
}

void finalize_MPI_IO_write_and_read(int memsize, int blocklen, 
				    MPI_Datatype memtype, MPI_Datatype etype,
				    MPI_Datatype filetype, int daccess)
{
  MPI_Comm_free(&io_comm2);
  mpi_free(io_filename);
}


/* *************************************************************************
 * MPI_IO_write_once
 *
 * @description
 * Measures the time to write once into a file, irrespective of the process 
 * order. Each process which participates in the measurement writes to his 
 * own section of a shared file or to his own (private) file. 
 *
 * process group:        <get_measurement_comm()>
 * number of processes:  n
 * group-files ratio:    n:1 (all processes operate on same file)
 * file size:            <count>*n  or  <count>
 * memory size:          <count>
 * block size:           <count>
 * access modes:         MPI_MODE_WRONLY | MPI_MODE_CREATE |
 *                       MPI_MODE_DELETE_ON_CLOSE
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous or noncontiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * count                 size of memory buffer
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * private               each process accesses its own file
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_write_once(int count, MPI_Datatype memtype, MPI_Datatype etype,
			    MPI_Datatype filetype, int private, int daccess)
{
  assert(count > 0);

  io_filename = get_io_filename(FILENAME, get_measurement_rank()*(private != 0));
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  send_buffer_init(count, memtype);

  init_synchronization();
}

double measure_MPI_IO_write_once(int count, MPI_Datatype memtype, 
				 MPI_Datatype etype, MPI_Datatype filetype, 
				 int private, int daccess)
{
  MPI_Comm    comm;
  int         amode;
  int         i;
  double      start_time, end_time;
  MPI_Offset  offset;
  MPI_Offset  position;
  MPI_Status  status;
  MPI_Request request;
  char        *send_buffer;
  MPI_Offset  filesize;

#ifdef IO_CHECK
  memset(get_send_buffer(), get_measurement_rank()%255+1, get_send_buffer_usage());
#endif

  offset = (private) ? (MPI_Offset)0 : get_offset(io_dtype, count);
  comm   = (private) ? MPI_COMM_SELF : get_measurement_comm();

  amode  = MPI_MODE_WRONLY | MPI_MODE_CREATE;
#ifndef IO_CHECK
  amode  = amode | MPI_MODE_DELETE_ON_CLOSE;
#endif
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(comm, io_filename, amode, io_info, &io_fh);

  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  if (io_preallocate)  {    
    filesize = (private) ? count : count*get_measurement_size();
    preallocate_file_space(io_fh, io_dtype, filesize);
  }

  /* begin measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();

  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    position = (MPI_Offset) 0;
    if (!private) {
      position = get_measurement_rank()*offset;
    }
    MPI_File_write_at(io_fh, position, get_send_buffer(), 
		      count, memtype, &status);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {
    if (!private) {
      MPI_File_seek(io_fh, get_measurement_rank()*offset, MPI_SEEK_SET);
    }
    MPI_File_write(io_fh, get_send_buffer(), count, memtype, &status);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

    MPI_File_write_shared(io_fh, get_send_buffer(), count, memtype, &status);

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    position = (MPI_Offset) 0;
    if (!private) {
      position = get_measurement_rank()*offset;
    }

    MPI_File_iwrite_at(io_fh, position, get_send_buffer(), 
		       count, memtype, &request);
    MPI_Wait(&request, &status);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    if (!private) {
      MPI_File_seek(io_fh, get_measurement_rank()*offset, MPI_SEEK_SET);
    }
    MPI_File_iwrite(io_fh, get_send_buffer(), count, memtype, &request);
    MPI_Wait(&request, &status);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    MPI_File_iwrite_shared(io_fh, get_send_buffer(), count, memtype, &request);
    MPI_Wait(&request, &status);

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    position = (MPI_Offset) 0;
    if (!private) {
      position = get_measurement_rank()*offset;
    }
    MPI_File_write_at_all(io_fh, position, get_send_buffer(), 
			  count, memtype, &status);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    if (!private) {
      MPI_File_seek(io_fh, get_measurement_rank()*offset, MPI_SEEK_SET);
    }
    MPI_File_write_all(io_fh, get_send_buffer(), count, memtype, &status);

 } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {
    
    MPI_File_write_ordered(io_fh, get_send_buffer(), count, memtype, &status);

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    position = (MPI_Offset) 0;
    if (!private) {
      position = get_measurement_rank()*offset;
    }
    MPI_File_write_at_all_begin(io_fh, position, send_buffer, count, memtype);
    MPI_File_write_at_all_end(io_fh, send_buffer, &status);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    if (!private) {
      MPI_File_seek(io_fh, get_measurement_rank()*offset, MPI_SEEK_SET);
    }
    send_buffer = get_send_buffer();
    MPI_File_write_all_begin(io_fh, send_buffer, count, memtype);
    MPI_File_write_all_end(io_fh, send_buffer, &status);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

    send_buffer = get_send_buffer();
    MPI_File_write_ordered_begin(io_fh, send_buffer, count, memtype);
    MPI_File_write_ordered_end(io_fh, send_buffer, &status);

  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }
  if (io_file_sync) {
    MPI_File_sync(io_fh);
  }
  end_time = stop_synchronization();
  MPI_File_close(&io_fh);

  return (end_time - start_time);
}

void finalize_MPI_IO_write_once(int count, MPI_Datatype memtype, 
				MPI_Datatype etype, MPI_Datatype filetype, 
				int private, int daccess)
{
  mpi_free(io_filename);
}



/* *************************************************************************
 * MPI_IO_read_once
 *
 * @description
 * Measures the time to read once from a file, irrespective of the process 
 * order. Each process which participates in the measurement reads from his 
 * own section of a shared file or from his own (private) file. 
 *
 * process group:        <get_measurement_comm()>
 * number of processes:  n
 * group-files ratio:    n:1 (all processes operate on same file)
 * file size:            <count>*n  or  <count>
 * memory size:          <count>
 * block size:           <count>
 * access modes:         MPI_MODE_RDONLY
 * memory datatype:      contiguous or noncontiguous
 * etype:                contiguous
 * filetype:             contiguous or noncontiguous
 * positioning:          all
 * synchronism:          all
 * coordination:         all
 *
 * @parameter
 * count                 size of memory buffer
 * memtype               type of data in memory
 * etype                 unit of data access and positioning
 * filetype              datatype for partitioning a file among processes
 * private               each process access his own file
 * daccess               data access routine class
 *
 * @return
 * double                
 * *************************************************************************
 */
void init_MPI_IO_read_once(int count, MPI_Datatype memtype, MPI_Datatype etype,
			   MPI_Datatype filetype, int private, int daccess)
{
  MPI_Status status;
  int        i, itmax;
  char       *send_buffer;

  assert(count > 0);

  io_filename = get_io_filename(FILENAME, get_measurement_rank()*(private != 0));
  datatype_param_init(&io_dtype, memtype, etype, filetype);
  recv_buffer_init(count, memtype);

  if (private || (get_measurement_rank() == 0)) {
    send_buffer = mpi_malloc_chars(get_extent(count, memtype));
#ifdef IO_CHECK
    io_chk_ref = buffer_fill((unsigned char *)send_buffer, count, memtype);
#endif    

    MPI_File_open(MPI_COMM_SELF, io_filename, 
		  MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN,
		  io_info, &io_fh);
    MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		      func_get_io_datarep(), io_info);

    itmax = (private) ? 1 : get_measurement_size();

    for (i=0; i<itmax; ++i) {
      MPI_File_write(io_fh, send_buffer, count, memtype, &status);      
    }

    MPI_File_close(&io_fh);
    mpi_free(send_buffer);
  }

#ifdef IO_CHECK
  if (!private) {
    MPI_Bcast(&io_chk_ref, 1, MPI_LONG, 0, get_measurement_comm());
  }
  io_chk_msg = 0;
#endif

  MPI_Barrier(get_measurement_comm());
  init_synchronization();
}

double measure_MPI_IO_read_once(int count, MPI_Datatype memtype, 
				MPI_Datatype etype, MPI_Datatype filetype, 
				int private, int daccess)
{
  MPI_Comm    comm;
  int         i;
  double      start_time, end_time;
  MPI_Offset  offset;
  MPI_Offset  position;
  MPI_Status  status;
  MPI_Request request;
  char        *recv_buffer;
  int         amode;
#ifdef IO_CHECK
  long cs = 0l;
#endif

  offset = (private) ? (MPI_Offset)0 : get_offset(io_dtype, count);
  comm   = (private) ? MPI_COMM_SELF : get_measurement_comm();

  amode = MPI_MODE_RDONLY;
  if (io_unique_open) amode = amode | MPI_MODE_UNIQUE_OPEN;

  MPI_File_open(comm, io_filename, amode, io_info, &io_fh);
  MPI_File_set_view(io_fh, (MPI_Offset)0, etype, filetype,
		    func_get_io_datarep(), io_info);
  MPI_File_set_atomicity(io_fh, io_atomicity);

  /* begin measurement */
  for (i=0; i<N_BARRIER; ++i) {
    MPI_Barrier(get_measurement_comm());
  }
  start_time = start_synchronization();
  
  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

    position = (MPI_Offset) 0;
    if (!private) {
      position = get_measurement_rank()*offset;
    }
    MPI_File_read_at(io_fh, position, get_recv_buffer(), 
		     count, memtype, &status);

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {
    if (!private) {
      MPI_File_seek(io_fh, get_measurement_rank()*offset, MPI_SEEK_SET);
    }
    MPI_File_read(io_fh, get_recv_buffer(), count, memtype, &status);
    
  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {
    
    MPI_File_read_shared(io_fh, get_recv_buffer(), count, memtype, &status);
    
  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {
    
    position = (MPI_Offset) 0;
    if (!private) {
      position = get_measurement_rank()*offset;
    }
    MPI_File_iread_at(io_fh, position, get_recv_buffer(), 
		      count, memtype, &request);
    MPI_Wait(&request, &status);
    
  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {
    
    if (!private) {
      MPI_File_seek(io_fh, get_measurement_rank()*offset, MPI_SEEK_SET);
    }
    MPI_File_iread(io_fh, get_recv_buffer(), count, memtype, &request);
    MPI_Wait(&request, &status);

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

    MPI_File_iread_shared(io_fh, get_recv_buffer(), count, memtype, &request);
    MPI_Wait(&request, &status);

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

    position = (MPI_Offset) 0;
    if (!private) {
      position = get_measurement_rank()*offset;
    }
    MPI_File_read_at_all(io_fh, position, get_recv_buffer(), 
			 count, memtype, &status);
    
  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {
    
    if (!private) {
      MPI_File_seek(io_fh, get_measurement_rank()*offset, MPI_SEEK_SET);
    }
    MPI_File_read_all(io_fh, get_recv_buffer(), count, memtype, &status);
    
  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {
    
    MPI_File_read_ordered(io_fh, get_recv_buffer(), count, memtype, &status);
    
  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {
    
    recv_buffer = get_recv_buffer();
    position = (MPI_Offset) 0;
    if (!private) {
      position = get_measurement_rank()*offset;
    }
    MPI_File_read_at_all_begin(io_fh, position, recv_buffer, count, memtype);
    MPI_File_read_at_all_end(io_fh, recv_buffer, &status);
    
  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {
    
    if (!private) {
      MPI_File_seek(io_fh, get_measurement_rank()*offset, MPI_SEEK_SET);
    }
    recv_buffer = get_recv_buffer();
    MPI_File_read_all_begin(io_fh, recv_buffer, count, memtype);
    MPI_File_read_all_end(io_fh, recv_buffer, &status);
    
  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {
    
    recv_buffer = get_recv_buffer();
    MPI_File_read_ordered_begin(io_fh, recv_buffer, count, memtype);
    MPI_File_read_ordered_end(io_fh, recv_buffer, &status);
    
  } else {
    stop_synchronization();
    MPI_File_close(&io_fh);
    return -1;
  }
  if (io_file_sync) {
    MPI_File_sync(io_fh);
  }
  end_time = stop_synchronization();
  MPI_File_close(&io_fh);

#ifdef IO_CHECK
  io_chk_add_sum((unsigned char*)get_recv_buffer(), count, memtype, &cs);
  io_chk_compare(cs, "MPI_IO_read", daccess);
#endif /* IO_CHECK */

  return (end_time - start_time);
}

void finalize_MPI_IO_read_once(int count, MPI_Datatype memtype, 
			       MPI_Datatype etype, MPI_Datatype filetype, 
			       int private, int daccess)
{
#ifndef IO_CHECK
  if (private || (get_measurement_rank() == 0)) {
    MPI_File_delete(io_filename, io_info);
  }
#endif /* IO_CHECK */
  mpi_free(io_filename);
}


/* ************************************************************************* */


#pragma weak end_skampi_extensions 












/* ************************************************************************* */


#ifdef IO_CHECK

int MPI_File_read_at(MPI_File mpi_fh, MPI_Offset offset, void *buf, 
		     int count, MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_read_at(mpi_fh, offset, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_at(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_iread_at(MPI_File mpi_fh, MPI_Offset offset, void *buf,
		      int count, MPI_Datatype datatype, MPI_Request *request)
{
  int errcode;
  errcode = PMPI_File_iread_at(mpi_fh, offset, buf, count, datatype, request);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_iread_at(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_read(MPI_File mpi_fh, void *buf, int count,
		  MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_read(mpi_fh, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_iread(MPI_File mpi_fh, void *buf, int count,
		   MPI_Datatype datatype, MPI_Request *request)
{
  int errcode;
  errcode = PMPI_File_iread(mpi_fh, buf, count, datatype, request);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_iread(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_read_shared(MPI_File mpi_fh, void *buf, int count,
			 MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_read_shared(mpi_fh, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_shared(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_iread_shared(MPI_File mpi_fh, void *buf, int count,
			  MPI_Datatype datatype, MPI_Request *request)
{
  int errcode;
  errcode = PMPI_File_iread_shared(mpi_fh, buf, count, datatype, request);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_iread_shared(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}


int MPI_File_read_at_all(MPI_File mpi_fh, MPI_Offset offset, void *buf,
			 int count, MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_read_at_all(mpi_fh, offset, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_at_all(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_read_at_all_begin(MPI_File mpi_fh, MPI_Offset offset, void *buf,
			       int count, MPI_Datatype datatype)
{
  int errcode;
  errcode = PMPI_File_read_at_all_begin(mpi_fh, offset, buf, count, datatype);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_at_all_begin(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_read_at_all_end(MPI_File mpi_fh, void *buf, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_read_at_all_end(mpi_fh, buf, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_at_all_end(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_read_all(MPI_File mpi_fh, void *buf, int count,
		      MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_read_all(mpi_fh, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_all(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_read_all_begin(MPI_File mpi_fh, void *buf, int count,
			    MPI_Datatype datatype)
{
  int errcode;
  errcode = PMPI_File_read_all_begin(mpi_fh, buf, count, datatype);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_all_begin(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_read_all_end(MPI_File mpi_fh, void *buf, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_read_all_end(mpi_fh, buf, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_all_end(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_read_ordered(MPI_File mpi_fh, void *buf, int count,
			  MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_read_ordered(mpi_fh, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_ordered(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }

  return errcode;
}

int MPI_File_read_ordered_begin(MPI_File mpi_fh, void *buf, int count,
				MPI_Datatype datatype)
{
  int errcode;
  errcode = PMPI_File_read_ordered_begin(mpi_fh, buf, count, datatype);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_ordered_begin(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_read_ordered_end(MPI_File mpi_fh, void *buf, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_read_ordered_end(mpi_fh, buf, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_read_ordered_end(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}


int MPI_File_write_at(MPI_File mpi_fh, MPI_Offset offset, void *buf, 
		      int count, MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_write_at(mpi_fh, offset, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_at(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_iwrite_at(MPI_File mpi_fh, MPI_Offset offset, void *buf,
		       int count, MPI_Datatype datatype, MPI_Request *request)
{
  int errcode;
  errcode = PMPI_File_iwrite_at(mpi_fh, offset, buf, count, datatype, request);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_iwrite_at(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}



int MPI_File_write(MPI_File mpi_fh, void *buf, int count,
		   MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_write(mpi_fh, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_iwrite(MPI_File mpi_fh, void *buf, int count,
		    MPI_Datatype datatype, MPI_Request *request)
{
  int errcode;
  errcode = PMPI_File_iwrite(mpi_fh, buf, count, datatype, request);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_iwrite(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_write_shared(MPI_File mpi_fh, void *buf, int count,
			  MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_write_shared(mpi_fh, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_shared(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_iwrite_shared(MPI_File mpi_fh, void *buf, int count,
			   MPI_Datatype datatype, MPI_Request *request)
{
  int errcode;
  errcode = PMPI_File_iwrite_shared(mpi_fh, buf, count, datatype, request);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_iwrite_shared(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}


int MPI_File_write_at_all(MPI_File mpi_fh, MPI_Offset offset, void *buf,
			  int count, MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_write_at_all(mpi_fh, offset, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_at_all(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_write_at_all_begin(MPI_File mpi_fh, MPI_Offset offset, void *buf,
			       int count, MPI_Datatype datatype)
{
  int errcode;
  errcode = PMPI_File_write_at_all_begin(mpi_fh, offset, buf, count, datatype);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_at_all_begin(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_write_at_all_end(MPI_File mpi_fh, void *buf, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_write_at_all_end(mpi_fh, buf, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_at_all_end(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_write_all(MPI_File mpi_fh, void *buf, int count,
		       MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_write_all(mpi_fh, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_all(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_write_all_begin(MPI_File mpi_fh, void *buf, int count,
			     MPI_Datatype datatype)
{
  int errcode;
  errcode = PMPI_File_write_all_begin(mpi_fh, buf, count, datatype);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_all_begin(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_write_all_end(MPI_File mpi_fh, void *buf, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_write_all_end(mpi_fh, buf, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_all_end(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_write_ordered(MPI_File mpi_fh, void *buf, int count,
			   MPI_Datatype datatype, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_write_ordered(mpi_fh, buf, count, datatype, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_ordered(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_write_ordered_begin(MPI_File mpi_fh, void *buf, int count,
				 MPI_Datatype datatype)
{
  int errcode;
  errcode = PMPI_File_write_ordered_begin(mpi_fh, buf, count, datatype);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_ordered_begin(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_write_ordered_end(MPI_File mpi_fh, void *buf, MPI_Status *status)
{
  int errcode;
  errcode = PMPI_File_write_ordered_end(mpi_fh, buf, status);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_write_ordered_end(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}



int MPI_File_open(MPI_Comm comm, char *filename, int amode,
		  MPI_Info info, MPI_File *fh)
{
  int errcode;
  errcode = PMPI_File_open(comm, filename, amode, info, fh);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_open(.., %s, ..) failed."
		     "\n%s\n", filename, get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_close(MPI_File *mpi_fh) 
{
  int errcode;
  errcode = PMPI_File_close(mpi_fh);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_close(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_set_view(MPI_File mpi_fh, MPI_Offset disp, MPI_Datatype etype,
		      MPI_Datatype filetype, char *datarep, MPI_Info info)
{
  int errcode;
  errcode = PMPI_File_set_view(mpi_fh, disp, etype, filetype, datarep, info);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_set_view(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_preallocate(MPI_File mpi_fh, MPI_Offset size)
{
  int errcode;
  errcode = PMPI_File_preallocate(mpi_fh, size);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_preallocate(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_set_size(MPI_File mpi_fh, MPI_Offset size)
{
  int errcode;
  errcode = PMPI_File_set_size(mpi_fh, size);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_set_size(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }

  return errcode;

}

int MPI_File_set_atomicity(MPI_File mpi_fh, int flag)
{
  int errcode;
  errcode = PMPI_File_set_atomicity(mpi_fh, flag);
  if (errcode != MPI_SUCCESS) {
    error_with_abort(errcode, "\nMPI_File_set_size(..) failed."
		     "\n%s\n", get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_seek(MPI_File mpi_fh, MPI_Offset offset, int whence)
{
  int errcode;
  char wstr[13];

  errcode = PMPI_File_seek(mpi_fh, offset, whence);
  if (errcode != MPI_SUCCESS) {
    switch (whence) {
    case MPI_SEEK_CUR: sprintf(wstr, "%s", "MPI_SEEK_CUR"); break;
    case MPI_SEEK_SET: sprintf(wstr, "%s", "MPI_SEEK_SET"); break;
    case MPI_SEEK_END: sprintf(wstr, "%s", "MPI_SEEK_END"); break;
    default:           sprintf(wstr, "%d", whence);         break;
    }
    error_with_abort(errcode,
		 "\nMPI_File_seek(mpi_fh=%d, offset=%d, whence=%s) failed\n%s\n",
		 (int) mpi_fh, (int) offset, wstr, get_error_message(errcode));
  }
  return errcode;
}

int MPI_File_seek_shared(MPI_File mpi_fh, MPI_Offset offset, int whence)
{
  int  errcode;
  char wstr[13];

  errcode = PMPI_File_seek_shared(mpi_fh, offset, whence);
  if (errcode != MPI_SUCCESS) {
    switch (whence) {
    case MPI_SEEK_CUR: sprintf(wstr, "%s", "MPI_SEEK_CUR"); break;
    case MPI_SEEK_SET: sprintf(wstr, "%s", "MPI_SEEK_SET"); break;
    case MPI_SEEK_END: sprintf(wstr, "%s", "MPI_SEEK_END"); break;
    default:           sprintf(wstr, "%d", whence);         break;
    }
    error_with_abort(errcode,
	  "\nMPI_File_seek_shared(mpi_fh=%d, offset=%d, whence=%s) failed\n%s\n",
	  (int) mpi_fh, (int) offset, wstr, get_error_message(errcode));
  }
  return errcode;
}


/* *************************************************************************
 * get_random_seed
 * *************************************************************************
 */
unsigned int get_random_seed(void)
{
     int myrank;
     double time;
     
     time = MPI_Wtime();
     time = time - (int) time;

     myrank = get_measurement_rank();
     return ((unsigned int) (time*(100000.0+myrank*237.0))) % UINT_MAX;
}

/* *************************************************************************
 * get_random_byte
 *
 * @parameter
 * void
 *
 * @return: 
 * unsigned char    value between 0 and 255
 * ***************************************************************************
 */
unsigned char get_random_byte(void)
{
  return (unsigned char) (256 * ((double) rand() / ((double) RAND_MAX + 1.0)));
}

/* ***************************************************************************
 * buffer_fill
 *
 * @description
 * Fill buffer with random bytes

 * @parameter
 * *buf                  pointer to buffer
 * size                  number of elements in buffer
 * type                  datatype of elements
 *
 * @return 
 * long                   
 * ***************************************************************************
 */
long buffer_fill(unsigned char *buf, int size, MPI_Datatype type)
{
  int           i;
  unsigned int  seed;
  long          sum;
  int           cnt;
  unsigned char *data_buf;
  unsigned char *packed_buf;
  int           psize;
  int           position;

  assert(buf != NULL);
  assert(size >= 0);

  /* erase buffer content */
  memset(buf, 0, get_extent(size, type));

  cnt = get_true_extent(size, type);
  data_buf  = (unsigned char*)mpi_malloc_chars(cnt);

  seed = get_random_seed();
  srand(seed);
  for (i=0; i<cnt; ++i) {
    *(data_buf+i) = get_random_byte();
  }

  /* Maybe the following 4 lines are not necessary, but with 
     respect to the "Rationale" section in MPI-1, chapter 3.13, that  
     states that a package could contain additional information, 
     such as a description of the sender architecture, we include
     them. */

  MPI_Pack_size(cnt, MPI_UNSIGNED_CHAR, MPI_COMM_SELF, &psize);
  packed_buf = (unsigned char*)mpi_malloc_chars(psize);
  position = 0;
  MPI_Pack(data_buf, cnt, MPI_UNSIGNED_CHAR, packed_buf, psize, &position, 
	   MPI_COMM_SELF);
  /* until here only architecture information could be added */

  position = 0;
  MPI_Unpack(packed_buf, psize, &position, buf, size, type, MPI_COMM_SELF);

  mpi_free(packed_buf);
  mpi_free(data_buf);

  sum = 0l;
  for (i=get_extent(size, type); i>0; --i) {
    sum += *buf;
    ++buf;
  }

  return sum;
}



/* ***************************************************************************
 * io_chk_add_sum
 * 
 * @description
 * Calculate the sum of the data in `buf' and add it to parameter `sum'.
 *
 * @parameter
 * *buf                  pointer to buffer
 * size                  size of buffer (in bytes)
 * type                  data type of buffer elements
 * sum                   value to which the buffer content is added
 *
 * @return
 * void                  
 * ***************************************************************************
 */
void io_chk_add_sum(unsigned char *buf, int size, MPI_Datatype type, long *sum)
{
  int           i;
  long          s;
  unsigned char *packed_buf;
  unsigned char *data_buf;
  int           psize;
  int           position;

  assert(buf != NULL);
  assert(size >= 0);

  MPI_Pack_size(size, type, MPI_COMM_SELF, &psize);
  packed_buf = (unsigned char*)mpi_malloc_chars(psize);
  position = 0;
  MPI_Pack(buf, size, type, packed_buf, psize, &position, MPI_COMM_SELF);

  data_buf = (unsigned char*)mpi_malloc_chars(psize);
  position = 0;
  MPI_Unpack(packed_buf, psize, &position, data_buf, psize, MPI_UNSIGNED_CHAR,
	     MPI_COMM_SELF);


  s = 0l;
  for (i=0; i<psize; ++i) {
    s += *(data_buf+i);
  }
  *sum = *sum + s;

  mpi_free(data_buf);
  mpi_free(packed_buf);
}


/* ***************************************************************************
 * io_chk_compare
 * 
 * @description
 * Check if `sum' is equal to `io_chk_ref'. 
 *
 *
 * @parameter
 * sum                   value which has to be compared with io_chk_ref
 * func                  name of the function (e.g. MPI_IO_read_random)
 * daccess               data access routine class (e.g. 0x111)
 *
 * @return
 * void
 * ***************************************************************************
 */
void io_chk_compare(long sum, const char *func, int daccess) 
{
  if ((sum != io_chk_ref) && (!io_chk_msg)) {
    fprintf(stderr, "# IO_CHECK error in %s(.., daccess=0x%x=%d): "
	    "sum (%ld) != io_chk_ref (%ld)\n", 
	    func, daccess, daccess, sum, io_chk_ref);
    fflush(stderr);
    io_chk_msg = 1;
  }
}

#endif /* IO_CHECK */

/* *************************************************************************** */


#endif /* USE_MPI_IO */


/* *************************************************************************** */

/*
  if (daccess == (IO_EXPLICIT_OFFSETS |
		  IO_BLOCKING |
		  IO_NONCOLLECTIVE)) {

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_NONCOLLECTIVE)) {

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_NONCOLLECTIVE)) {

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_BLOCKING |
			 IO_COLLECTIVE)) {

  } else if (daccess == (IO_EXPLICIT_OFFSETS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

  } else if (daccess == (IO_INDIVIDUAL_FILE_POINTERS |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

  } else if (daccess == (IO_SHARED_FILE_POINTER |
			 IO_NONBLOCKING_SPLIT |
			 IO_COLLECTIVE)) {

  } else {
  }
*/
