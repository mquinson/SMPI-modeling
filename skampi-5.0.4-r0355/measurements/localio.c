/********************************************************************************
 * SKaMPI  MPI-Benchmark 
 *
 * Copyright (C) 2007-2008  Joachim Mathes
 * Lehrstuhl Informatik fuer Ingenieure und Naturwissenschaftler
 * Institut fuer Algorithmen und Kognitive Systeme
 * Fakultaet fuer Informatik
 * Universitaet Karlsruhe (TH)
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *******************************************************************************/

/**
 * \file localio.c
 * 
 * \brief SKaMPI measurement functions for rudimentary local IO
 *
 * This file provides rudimentary IO measurement functions for SkaMPI. 
 *
 * \author Joachim Mathes
 * \date 2007-2008
 */

#include <mpi.h>
#include "../mpiversiontest.h"

#ifdef USE_MPI_IO

/**
 * The <tt>_GNU_SOURCE</tt> macro must be set to provide the <tt>O_DIRECT</tt>
 * parameter for open() routine.
 */
#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <sys/mman.h>

#include "../misc.h"
#include "../synchronize.h"
#include "../output.h"
#include "../mem.h"

#define IO_FILENAME   "skampi_io"
#define POSIX_API     "posix"
#define MPI_API       "mpi"
#define BUFFER_SIZE   4194304   /**< 4 MB */
#define MAXIMUM_POWER 62	/**< limited by MPI_Offset size (number of bits) */

#pragma weak begin_skampi_extensions

/**
 * \brief Returns a unique filename in a dedicated path
 *
 * This function is defined in <tt>&lt;SKaMPI path&gt;/measurements/io.c</tt>.
 */
extern char *get_io_filename (char *, int);

/********************************************************************************
 * Global variables
 *******************************************************************************/

static char*    io_filename;
static MPI_File io_fh;
static int      io_fd;

/********************************************************************************
 * Measure functions
 *******************************************************************************/

/*@{*/
/** \name Measurement-Functions
 * The following measurement function blocks (<tt>init_</tt>, <tt>measure_</tt>,
 * <tt>finalize_</tt>) are meant to work on <em>local</em> disks. Nevertheless
 * it depends on the user defined environment variable <tt>TMP</tt>, if a
 * distributed RFA-file will be stored on local or global (parallel) disks.
 */

/**
 * \brief Initialization function of measure function
 *        measure_MPI_IO_read_file_once().
 *
 * Only one process is active. It reads once from a file.
 *
 * Remark:<br>
 * With the <tt>O_DIRECT</tt> flag set, cache effects are minimized, because I/O
 * is done directly to/from user space buffers. The operation system's page
 * cache is bypassed. Under Linux 2.6 alignment to 512-byte boundaries is
 * required for buffer and file offset. Thus the following parameters should be
 * set in a SKaMPI input file:
 * - <tt>set_send_buffert_alignment (512)</tt>
 * - <tt>set_recv_buffert_alignment (512)</tt>
 * - <tt>switch_buffer_cycling_off ()</tt><br>
 *
 * <tt>O_DIRECT</tt> is only relevant if the POSIX-API is used for I/O.
 * 
 * For more information please refer to the <tt>open ()</tt> man pages.
 * 
 * \param[in] size          size of memory buffer, i.e. number of <tt>MPI_BYTE</tt>s
 * \param[in] api           POSIX-API or MPI-API for I/O accesses
 * \param[in] directio_flag open file with <tt>O_DIRECT</tt> flag to minimize
 *                          cache effects
 *
 * \return    void
 */
void init_MPI_IO_read_file_once (int size, char *api, int directio_flag) {
  char *send_buffer;

  assert (size > 0);

  io_filename = get_io_filename (IO_FILENAME, 0);

  if (get_measurement_rank () == 0){

    send_buffer = mpi_malloc_chars (get_extent (size, MPI_BYTE));

    MPI_File_open (MPI_COMM_SELF, io_filename,
		   MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN,
		   MPI_INFO_NULL, &io_fh);
    MPI_File_set_view (io_fh, (MPI_Offset)0, 
		       MPI_BYTE, MPI_BYTE,
		       "native", MPI_INFO_NULL);
    MPI_File_write (io_fh, send_buffer, size, MPI_BYTE, MPI_STATUS_IGNORE);
    MPI_File_close (&io_fh);
    mpi_free (send_buffer);

    set_recv_buffer_usage (size);
    set_reported_message_size (size);
  }

  MPI_Barrier (get_measurement_comm ());

  /* set synchronization type:
   SYNC_BARRIER if all SKaMPI processes run on one physical processor 
   SYNC_REAL if every SKaMPI process runs on its own physical processor */
  set_synchronization (SYNC_REAL);

  init_synchronization ();
}

/**
 * \brief Measures the time to read once from a file.
 *
 * Only one process is active. It reads once from a file.
 *
 * Remark:<br>
 * With the <tt>O_DIRECT</tt> flag set, cache effects are minimized, because I/O
 * is done directly to/from user space buffers. The operation system's page
 * cache is bypassed. Under Linux 2.6 alignment to 512-byte boundaries is
 * required for buffer and file offset. Thus the following parameters should be
 * set in a SKaMPI input file:
 * - <tt>set_send_buffert_alignment (512)</tt>
 * - <tt>set_recv_buffert_alignment (512)</tt>
 * - <tt>switch_buffer_cycling_off ()</tt><br>
 *
 * <tt>O_DIRECT</tt> is only relevant if the POSIX-API is used for I/O.
 * 
 * For more information please refer to the <tt>open ()</tt> man pages.
 *
 * \param[in] size  size of memory buffer, i.e. number of <tt>MPI_BYTE</tt>s
 * \param[in] api   POSIX-API or MPI-API for I/O accesses
 * \param[in] directio_flag open file with <tt>O_DIRECT</tt> flag to minimize
 *                          cache effects
 *
 * \return    measured time 
 */
double measure_MPI_IO_read_file_once (int size, char *api, int directio_flag){
  double     start_time = 1.0, end_time = 0.0;
  int        open_flags;
  char       *error_string;
  
  if (get_measurement_rank () == 0){
    if (strcmp (api, POSIX_API) == 0){ 

      if (directio_flag != 0)
	open_flags = O_RDONLY | O_DIRECT;
      else
	open_flags = O_RDONLY;

      printf ("flags %d,%d\n", open_flags, O_DIRECT);

      errno = 0;
      if ((io_fd = open (io_filename, open_flags)) < 0){
	error_string = strerror (errno);
	error_with_abort (errno,
			  "\nmeasure_MPI_IO_read_file_once (int %d, char * %s, int %d) failed."
			  "\nCannot open local file (read only mode)."
			  "\nError: %s\n",
			  size, api, directio_flag, error_string);
      }
    
      start_time = start_synchronization ();
      read (io_fd, get_recv_buffer (), size);
      end_time = MPI_Wtime ();

      close (io_fd);

    }
    else{
      MPI_File_open (MPI_COMM_SELF, io_filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &io_fh);
      MPI_File_set_view (io_fh, (MPI_Offset)0, 
			 MPI_BYTE, MPI_BYTE,
			 "native", MPI_INFO_NULL);

      start_time = start_synchronization ();
      MPI_File_read (io_fh, get_recv_buffer (), size, MPI_BYTE, MPI_STATUS_IGNORE);
      end_time = stop_synchronization ();

      MPI_File_close (&io_fh);

    }
  }
  else if (get_measurement_rank () != 0) {
    start_synchronization ();
  }
  stop_synchronization ();

  if (get_measurement_rank () == 0)
    return end_time - start_time;
  else
    return -1.0;

}

/**
 * \brief Finalization function of measure function
 *        measure_MPI_IO_read_file_once().
 *
 * Only one process is active. It reads once from a file.
 *
 * Remark:<br>
 * With the <tt>O_DIRECT</tt> flag set, cache effects are minimized, because I/O
 * is done directly to/from user space buffers. The operation system's page
 * cache is bypassed. Under Linux 2.6 alignment to 512-byte boundaries is
 * required for buffer and file offset. Thus the following parameters should be
 * set in a SKaMPI input file:
 * - <tt>set_send_buffert_alignment (512)</tt>
 * - <tt>set_recv_buffert_alignment (512)</tt>
 * - <tt>switch_buffer_cycling_off ()</tt><br>
 *
 * <tt>O_DIRECT</tt> is only relevant if the POSIX-API is used for I/O.
 * 
 * For more information please refer to the <tt>open ()</tt> man pages.
 *        
 * \param[in] size  size of memory buffer, i.e. number of <tt>MPI_BYTE</tt>s
 * \param[in] api   POSIX-API or MPI-API for I/O accesses
 * \param[in] directio_flag open file with <tt>O_DIRECT</tt> flag to minimize
 *                          cache effects
 *
 * \return    void
 */
void finalize_MPI_IO_read_file_once (int size, char *api, int directio_flag){
  MPI_File_delete (io_filename, MPI_INFO_NULL);
  mpi_free (io_filename);
}

/**
 * \brief Initialization function of measure function
 *        measure_MPI_IO_write_file_once().
 *
 * Only one process is active. It writes once to a file.
 *
 * Remark:<br>
 * With the <tt>O_DIRECT</tt> flag set, cache effects are minimized, because I/O
 * is done directly to/from user space buffers. The operation system's page
 * cache is bypassed. Under Linux 2.6 alignment to 512-byte boundaries is
 * required for buffer and file offset. Thus the following parameters should be
 * set in a SKaMPI input file:
 * - <tt>set_send_buffert_alignment (512)</tt>
 * - <tt>set_recv_buffert_alignment (512)</tt>
 * - <tt>switch_buffer_cycling_off ()</tt><br>
 * 
 * For more information please refer to the <tt>open ()</tt> man pages.
 * 
 * \param[in] size        size of memory buffer, i.e. number of <tt>MPI_BYTE</tt>s
 * \param[in] api         POSIX-API or MPI-API for I/O accesses
 * \param[in] create_flag write into existing file (FALSE) or create it (TRUE)
 * \param[in] directio_flag open file with <tt>O_DIRECT</tt> flag to minimize
 *                          cache effects
 *
 * \return    void
 */
void init_MPI_IO_write_file_once (int size, char *api, int create_flag, int directio_flag) {
  char *send_buffer;

  assert (size > 0);

  io_filename = get_io_filename (IO_FILENAME, 0);

  if (get_measurement_rank () == 0){

    if (create_flag == 0){
      send_buffer = mpi_malloc_chars (get_extent (size, MPI_BYTE));
    
      MPI_File_open (MPI_COMM_SELF, io_filename,
		     MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN,
		     MPI_INFO_NULL, &io_fh);
      MPI_File_set_view (io_fh, (MPI_Offset)0, 
			 MPI_BYTE, MPI_BYTE,
			 "native", MPI_INFO_NULL);
      MPI_File_write (io_fh, send_buffer, size, MPI_BYTE, MPI_STATUS_IGNORE);
      MPI_File_close (&io_fh);
      mpi_free (send_buffer);
    }

    set_send_buffer_usage (size);
    set_reported_message_size (size);
  }

  MPI_Barrier (get_measurement_comm ());

  /* set synchronization type:
   SYNC_BARRIER if all SKaMPI processes run on one physical processor 
   SYNC_REAL if every SKaMPI process runs on its own physical processor */
  set_synchronization (SYNC_REAL);

  init_synchronization ();
}

/**
 * \brief Measures the time to write once to a file.
 *
 * Only one process is active. It writes once to a file.
 *
 * Remark:<br>
 * With the <tt>O_DIRECT</tt> flag set, cache effects are minimized, because I/O
 * is done directly to/from user space buffers. The operation system's page
 * cache is bypassed. Under Linux 2.6 alignment to 512-byte boundaries is
 * required for buffer and file offset. Thus the following parameters should be
 * set in a SKaMPI input file:
 * - <tt>set_send_buffert_alignment (512)</tt>
 * - <tt>set_recv_buffert_alignment (512)</tt>
 * - <tt>switch_buffer_cycling_off ()</tt><br>
 *
 * <tt>O_DIRECT</tt> is only relevant if the POSIX-API is used for I/O.
 * 
 * For more information please refer to the <tt>open ()</tt> man pages.
 *
 * \param[in] size        size of memory buffer, i.e. number of <tt>MPI_BYTE</tt>s
 * \param[in] api         POSIX-API or MPI-API for I/O accesses
 * \param[in] create_flag write into existing file (FALSE) or create it (TRUE)
 * \param[in] directio_flag open file with <tt>O_DIRECT</tt> flag to minimize
 *                          cache effects
 *
 * \return    measured time 
 */
double measure_MPI_IO_write_file_once (int size, char *api, int create_flag, int directio_flag){
  double     start_time = 1.0, end_time = 0.0;
  int        open_flags;
  char       *error_string;
  
  if (get_measurement_rank () == 0){
    if (strcmp (api, POSIX_API) == 0){

      if (directio_flag != 0)
	open_flags = O_WRONLY | O_DIRECT;
      else
	open_flags = O_WRONLY;

      errno = 0;

      if (create_flag == 0){	/* open existing file */

	if ((io_fd = open (io_filename, open_flags)) < 0){
	  error_string = strerror (errno);
	  error_with_abort (errno,
			    "\nmeasure_MPI_IO_write_file_once (int %d, char * %s, int %d, int %d) failed."
			    "\nCannot open local file (write only mode)."
			    "\nError: %s\n",
			    size, api, create_flag, directio_flag, error_string);
	}
      }
      else {			/* open nonexisting file and create it */
	
	if ((io_fd = open (io_filename, open_flags|O_CREAT, S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH)) < 0){
	  error_string = strerror (errno);
	  error_with_abort (errno,
			   "\nmeasure_MPI_IO_write_file_once (int %d, char * %s, int %d, int %d) failed."
			   "\nCannot open local file (write only mode)."
			   "\nError: %s\n",
			    size, api, create_flag, directio_flag, error_string);
	}
      }

      start_time = start_synchronization ();
      write (io_fd, get_send_buffer (), size);
      fsync (io_fd);
      end_time = MPI_Wtime ();

      close (io_fd);

    }
    else{ 			/* if strcmp (api, POSIX_API) != 0 */

      if (create_flag == 0){

	MPI_File_open (MPI_COMM_SELF, io_filename, MPI_MODE_WRONLY, MPI_INFO_NULL, &io_fh);
      }
      else{ 			/* if create_flag != 0*/

	MPI_File_open (MPI_COMM_SELF, io_filename, MPI_MODE_WRONLY|MPI_MODE_CREATE, MPI_INFO_NULL, &io_fh);
      }
      
      MPI_File_set_view (io_fh, (MPI_Offset)0, 
			MPI_BYTE, MPI_BYTE,
			"native", MPI_INFO_NULL);

      start_time = start_synchronization ();
      MPI_File_write (io_fh, get_send_buffer (), size, MPI_BYTE, MPI_STATUS_IGNORE);
      MPI_File_sync (io_fh);
      end_time = MPI_Wtime ();
      
      MPI_File_close (&io_fh);
    }
  }
  else if (get_measurement_rank () != 0) {
    start_synchronization ();
  }
  stop_synchronization ();

  if (get_measurement_rank () == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/**
 * \brief Finalization function of measure function
 *        measure_MPI_IO_write_file_once().
 *
 * Only one process is active. It writes once to a file.
 *
 * Remark:<br>
 * With the <tt>O_DIRECT</tt> flag set, cache effects are minimized, because I/O
 * is done directly to/from user space buffers. The operation system's page
 * cache is bypassed. Under Linux 2.6 alignment to 512-byte boundaries is
 * required for buffer and file offset. Thus the following parameters should be
 * set in a SKaMPI input file:
 * - <tt>set_send_buffert_alignment (512)</tt>
 * - <tt>set_recv_buffert_alignment (512)</tt>
 * - <tt>switch_buffer_cycling_off ()</tt><br>
 *
 * <tt>O_DIRECT</tt> is only relevant if the POSIX-API is used for I/O.
 * 
 * For more information please refer to the <tt>open ()</tt> man pages.
 *
 * \param[in] size  size of memory buffer, i.e. number of <tt>MPI_BYTE</tt>s
 * \param[in] api   POSIX-API or MPI-API for I/O accesses
 * \param[in] create_flag write into existing file (FALSE) or create it (TRUE)
 * \param[in] directio_flag open file with <tt>O_DIRECT</tt> flag to minimize
 *                          cache effects
 *
 * \return    void
 */
void finalize_MPI_IO_write_file_once (int size, char *api, int create_flag, int directio_flag){
  MPI_File_delete (io_filename, MPI_INFO_NULL);
  mpi_free (io_filename);
}

/**
 * \brief Initialization function of measure function
 *        measure_MPI_IO_read_large_file_once().
 *
 * Only one process is active. It writes once to a file.
 *        
 * Since SKaMPI measurement functions are not allowed to use MPI_Offset
 * parameters, it is impossible to tell an init_-routine to create a file
 * which is larger than \f$2^{\mbox{\texttt{sizeof(int)}}-1}-1\f$ directly. As
 * a preliminary solution we use a parameter (<tt>power</tt>) which commits the
 * power to 2 as an indicator for the file size.
 *
 * Remark concerning the <em>HP XC6000</em>:<br>
 * Measurements showed that there is no significant difference between MPI-API
 * and POSIX-API I/O accesses, if files are larger than 1MB. Thus there is no
 * choice between these two modes like in measure_MPI_IO_read_file_once(),
 * which makes type compatibilty problems much easier. Only MPI-API is
 * supported.
 *        
 * Only one process is active. It reads once from a file.
 *
 * \param[in] power size of memory buffer; 2 to the power of `power' <tt>MPI_BYTE</tt>s
 *
 * \return    void
 */
void init_MPI_IO_read_large_file_once (int power) {
  MPI_Offset size;
  char       *error_string;

  io_filename = get_io_filename (IO_FILENAME, 0);

  if (get_measurement_rank () == 0){

    if (power > MAXIMUM_POWER || power < 0){
      error_string = strerror (EINVAL);
      error_with_abort (errno,
			"\ninit_MPI_IO_read_large_file_once (int %d) failed."
			"\nInvalid power argument."
			"\nError: %s\n",
			power, error_string);
    }

    size = ((MPI_Offset) 1) << power;

    MPI_File_open (MPI_COMM_SELF, io_filename,
		   MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN,
		   MPI_INFO_NULL, &io_fh);
    MPI_File_preallocate (io_fh, size);
    MPI_File_close (&io_fh);

    set_recv_buffer_usage (size);
    set_reported_message_size (size);
  }

  MPI_Barrier (get_measurement_comm ());

  /* set synchronization type:
   SYNC_BARRIER if all SKaMPI processes run on one physical processor 
   SYNC_REAL if every SKaMPI process runs on its own physical processor */
  set_synchronization (SYNC_REAL);

  init_synchronization ();
}

/**
 * \brief Measures the time to read once from a large file.
 *
 * Only one process is active. It writes once to a file.
 * 
 * Since SKaMPI measurement functions are not allowed to use MPI_Offset
 * parameters, it is impossible to tell an init_-routine to create a file
 * which is larger than \f$2^{\mbox{\texttt{sizeof(int)}}-1}-1\f$ directly. As
 * a preliminary solution we use a parameter (<tt>power</tt>) which commits the
 * power to 2 as an indicator for the file size.
 *
 * Remark concerning the <em>HP XC6000</em>:<br>
 * Measurements showed that there is no significant difference between MPI-API
 * and POSIX-API I/O accesses, if files are larger than 1MB. Thus there is no
 * choice between these two modes like in measure_MPI_IO_read_file_once(),
 * which makes type compatibilty problems much easier. Only MPI-API is
 * supported.
 * 
 * \param[in] power size of memory buffer; 2 to the power of `power' <tt>MPI_BYTE</tt>s
 *
 * \return    measured time 
 */
double measure_MPI_IO_read_large_file_once (int power){
  MPI_Offset size;
  double     start_time = 1.0, end_time = 0.0;
  
  if (get_measurement_rank () == 0){
    
    size = ((MPI_Offset) 1) << power;

    MPI_File_open (MPI_COMM_SELF, io_filename, MPI_MODE_RDONLY, MPI_INFO_NULL, &io_fh);
    MPI_File_set_view (io_fh, (MPI_Offset)0, 
		       MPI_BYTE, MPI_BYTE,
		       "native", MPI_INFO_NULL);
    
    start_time = start_synchronization ();
    MPI_File_read (io_fh, get_recv_buffer (), size, MPI_BYTE, MPI_STATUS_IGNORE);
    end_time = stop_synchronization ();
    
    MPI_File_close (&io_fh);
  }
  else if (get_measurement_rank () != 0) {
    start_synchronization ();
  }
  stop_synchronization ();

  if (get_measurement_rank () == 0)
    return end_time - start_time;
  else
    return -1.0;

}

/**
 * \brief Finalization function of measure function
 *        measure_MPI_IO_read_large_file_once().
 *
 * Only one process is active. It writes once to a file.
 *
 * Since SKaMPI measurement functions are not allowed to use MPI_Offset
 * parameters, it is impossible to tell an init_-routine to create a file
 * which is larger than \f$2^{\mbox{\texttt{sizeof(int)}}-1}-1\f$ directly. As
 * a preliminary solution we use a parameter (<tt>power</tt>) which commits the
 * power to 2 as an indicator for the file size.
 *
 * Remark concerning the <em>HP XC6000</em>:<br>
 * Measurements showed that there is no significant difference between MPI-API
 * and POSIX-API I/O accesses, if files are larger than 1MB. Thus there is no
 * choice between these two modes like in measure_MPI_IO_read_file_once(),
 * which makes type compatibilty problems much easier. Only MPI-API is
 * supported.
 *        
 * \param[in] power size of memory buffer; 2 to the power of `power' <tt>MPI_BYTE</tt>s
 *
 * \return    void
 */
void finalize_MPI_IO_read_large_file_once (int power){
  MPI_File_delete (io_filename, MPI_INFO_NULL);
  mpi_free (io_filename);
}

/**
 * \brief Initialization function of measure function
 *        measure_MPI_IO_write_large_file_once().
 *
 * Only one process is active. It writes once to a file.
 *
 * Since SKaMPI measurement functions are not allowed to use MPI_Offset
 * parameters, it is impossible to tell an init_-routine to create a file
 * which is larger than \f$2^{\mbox{\texttt{sizeof(int)}}-1}-1\f$ directly. As
 * a preliminary solution we use a parameter (<tt>power</tt>) which commits the
 * power to 2 as an indicator for the file size.
 *
 * Remark concerning the <em>HP XC6000</em>:<br>
 * Measurements showed that there is no significant difference between MPI-API
 * and POSIX-API I/O accesses, if files are larger than 1MB. Thus there is no
 * choice between these two modes like in measure_MPI_IO_read_file_once(),
 * which makes type compatibilty problems much easier. Only MPI-API is
 * supported.
 * 
 * \param[in] power       size of memory buffer; 2 to the power of `power' <tt>MPI_BYTE</tt>s
 * \param[in] create_flag write into existing file (FALSE) or create it (TRUE)
 *
 * \return    void
 */
void init_MPI_IO_write_large_file_once (int power, int create_flag) {
  MPI_Offset size;
  char       *error_string;

  io_filename = get_io_filename (IO_FILENAME, 0);

  if (get_measurement_rank () == 0){

    if (power > MAXIMUM_POWER || power < 0){
      error_string = strerror (EINVAL);
      error_with_abort (errno,
			"\ninit_MPI_IO_write_large_file_once (int %d, int %d) failed."
			"\nInvalid power argument."
			"\nError: %s\n",
			power, create_flag, error_string);
    }

    size = ((MPI_Offset) 1) << power;

    if (create_flag == 0){
    
      MPI_File_open (MPI_COMM_SELF, io_filename,
		     MPI_MODE_WRONLY | MPI_MODE_CREATE | MPI_MODE_UNIQUE_OPEN,
		     MPI_INFO_NULL, &io_fh);
      MPI_File_preallocate (io_fh, size);
      MPI_File_close (&io_fh);
    }
    
    set_send_buffer_usage (size);
    set_reported_message_size (size);
  }

  MPI_Barrier (get_measurement_comm ());

  /* set synchronization type:
   SYNC_BARRIER if all SKaMPI processes run on one physical processor 
   SYNC_REAL if every SKaMPI process runs on its own physical processor */
  set_synchronization (SYNC_REAL);

  init_synchronization ();
}

/**
 * \brief Measures the time to write once to a large file.
 *
 * Only one process is active. It writes once to a file.
 *
 * Since SKaMPI measurement functions are not allowed to use MPI_Offset
 * parameters, it is impossible to tell an init_-routine to create a file
 * which is larger than \f$2^{\mbox{\texttt{sizeof(int)}}-1}-1\f$ directly. As
 * a preliminary solution we use a parameter (<tt>power</tt>) which commits the
 * power to 2 as an indicator for the file size.
 *
 * Remark concerning the <em>HP XC6000</em>:<br>
 * Measurements showed that there is no significant difference between MPI-API
 * and POSIX-API I/O accesses, if files are larger than 1MB. Thus there is no
 * choice between these two modes like in measure_MPI_IO_read_file_once(),
 * which makes type compatibilty problems much easier. Only MPI-API is
 * supported.
 * 
 * \param[in] power       size of memory buffer; 2 to the power of `power' <tt>MPI_BYTE</tt>s
 * \param[in] create_flag write into existing file (FALSE) or create it (TRUE)
 *
 * \return    measured time 
 */
double measure_MPI_IO_write_large_file_once (int power, int create_flag){
  MPI_Offset size;
  double     start_time = 1.0, end_time = 0.0;
  
  if (get_measurement_rank () == 0){
    
    size = ((MPI_Offset) 1) << power;

    if (create_flag == 0){

      MPI_File_open (MPI_COMM_SELF, io_filename, MPI_MODE_WRONLY, MPI_INFO_NULL, &io_fh);
    }
    else{ 			/* if create_flag != 0*/

      MPI_File_open (MPI_COMM_SELF, io_filename, MPI_MODE_WRONLY|MPI_MODE_CREATE, MPI_INFO_NULL, &io_fh);
    }
      
    MPI_File_set_view (io_fh, (MPI_Offset)0, 
		       MPI_BYTE, MPI_BYTE,
		       "native", MPI_INFO_NULL);

    start_time = start_synchronization ();
    MPI_File_write (io_fh, get_send_buffer (), size, MPI_BYTE, MPI_STATUS_IGNORE);
    end_time = MPI_Wtime ();
      
    MPI_File_close (&io_fh);

  }
  else if (get_measurement_rank () != 0) {
    start_synchronization ();
  }
  stop_synchronization ();

  if (get_measurement_rank () == 0)
    return end_time - start_time;
  else
    return -1.0;
}

/**
 * \brief Finalization function of measure function
 *        measure_MPI_IO_write_large_file_once().
 *
 * Only one process is active. It writes once to a file.
 *
 * Since SKaMPI measurement functions are not allowed to use MPI_Offset
 * parameters, it is impossible to tell an init_-routine to create a file
 * which is larger than \f$2^{\mbox{\texttt{sizeof(int)}}-1}-1\f$ directly. As
 * a preliminary solution we use a parameter (<tt>power</tt>) which commits the
 * power to 2 as an indicator for the file size.
 *
 * Remark concerning the <em>HP XC6000</em>:<br>
 * Measurements showed that there is no significant difference between MPI-API
 * and POSIX-API I/O accesses, if files are larger than 1MB. Thus there is no
 * choice between these two modes like in measure_MPI_IO_read_file_once(),
 * which makes type compatibilty problems much easier. Only MPI-API is
 * supported.
 *        
 * \param[in] power       size of memory buffer; 2 to the power of `power' <tt>MPI_BYTE</tt>s
 * \param[in] create_flag write into existing file (FALSE) or create it (TRUE)
 *
 * \return    void
 */
void finalize_MPI_IO_write_large_file_once (int power, int create_flag){
  MPI_File_delete (io_filename, MPI_INFO_NULL);
  mpi_free (io_filename);
}

/*@}*/

#pragma weak end_skampi_extensions

#endif	/* USE_MPI_IO */
