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

#ifndef MPI_VERSION
#define MPI_VERSION 1
#endif

#ifndef MPI_SUBVERSION
#define MPI_SUBVERSION 0
#endif

#ifndef __SKAMPI_EXTRACT_IS_RUNNING__

#if MPI_VERSION == 2
#ifndef USE_ONESIDED
#define USE_ONESIDED
#endif
#ifndef USE_MPI_IO
#define USE_MPI_IO
#endif
#ifndef USE_MPI_ALLOC_MEM
#define USE_MPI_ALLOC_MEM
#endif
#endif

#else /* __SKAMPI_EXTRACT_IS_RUNNING__  *is* defined */
/* we use extract to choose between USE_ONESIDED and not using it */
#endif /* matching #ifndef __SKAMPI_EXTRACT_IS_RUNNING__ */

#if defined(USE_ONESIDED) || defined(USE_MPI_IO) || defined(USE_MPI_ALLOC_MEM)
#ifndef USE_MPI_INFO
#define USE_MPI_INFO
#endif
#endif
