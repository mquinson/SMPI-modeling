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

#include <stdlib.h>
#include <assert.h>
#include <mpi.h>


#include "../misc.h"
#include "../private_misc.h"
#include "../exec.h"

#pragma weak begin_skampi_extensions

MPI_Datatype func_mpi_type_contiguous(int count, MPI_Datatype oldtype)
{
  MPI_Datatype newtype;

  MPI_Type_contiguous(count, oldtype, &newtype); 
  MPI_Type_commit(&newtype);
  return newtype;
}

MPI_Datatype func_mpi_type_vector(int count, int length, int stride, MPI_Datatype oldtype)
{
  MPI_Datatype newtype;

  MPI_Type_vector(count, length, stride, oldtype, &newtype);
  MPI_Type_commit(&newtype);
  return newtype;
}

MPI_Datatype func_mpi_type_hvector(int count, int length, int stride, MPI_Datatype oldtype)
{
  MPI_Datatype newtype;

  MPI_Type_hvector(count, length, stride, oldtype, &newtype);
  MPI_Type_commit(&newtype);
  return newtype;
}

MPI_Datatype func_mpi_type_indexed(IArray blocklengths, IArray displacements, MPI_Datatype oldtype)
{
  MPI_Datatype newtype;
  
  assert (blocklengths.n == displacements.n);
  MPI_Type_indexed(blocklengths.n, blocklengths.v, displacements.v, oldtype, &newtype);
  MPI_Type_commit(&newtype);
  
  return newtype;
}

MPI_Datatype func_mpi_type_hindexed(IArray blocklengths, IArray displacements, MPI_Datatype oldtype)
{
  MPI_Datatype newtype;
  
  assert (blocklengths.n == displacements.n);
  MPI_Type_hindexed(blocklengths.n, blocklengths.v, (MPI_Aint*) displacements.v, oldtype, &newtype);
  MPI_Type_commit(&newtype);
  
  return newtype;
}

MPI_Datatype func_mpi_type_struct(IArray blocklengths, IArray displacements, DArray dtypes)
{
  MPI_Datatype newtype;
  
  assert((blocklengths.n == displacements.n) && (displacements.n == dtypes.n));
  MPI_Type_struct(blocklengths.n, blocklengths.v, (MPI_Aint*) displacements.v, dtypes.v, &newtype);
  MPI_Type_commit(&newtype);
  
  return newtype;
}

/* we assume that MPI implementations that understand at least the MPI-IO 
   routines, would understand the next datatype constructor, too;
   USE_MPI_IO is defined if MPI_VERSION is greater or equal than 2 */

#ifdef USE_MPI_IO
MPI_Datatype func_mpi_type_create_subarray(IArray sizes, IArray subsizes, IArray starts, MPI_Datatype oldtype)
{
  MPI_Datatype newtype;

  assert((sizes.n == subsizes.n) && (subsizes.n == starts.n));
  MPI_Type_create_subarray(sizes.n, sizes.v, subsizes.v, starts.v, MPI_ORDER_C, oldtype, &newtype);
  MPI_Type_commit(&newtype);

  return newtype;
}

MPI_Datatype func_mpi_type_create_darray(int size, int rank, IArray gsizes, IArray distribs, IArray dargs, IArray psizes, MPI_Datatype oldtype)
{
  MPI_Datatype newtype;

  assert(gsizes.n == distribs.n);
  assert(distribs.n == dargs.n);
  assert(dargs.n == psizes.n);

  MPI_Type_create_darray(size, rank, gsizes.n, gsizes.v, distribs.v, dargs.v, psizes.v, MPI_ORDER_C, oldtype, &newtype);
  MPI_Type_commit(&newtype);

  return newtype;
} 
#endif

#pragma weak end_skampi_extensions

/* do we need these:

    MPI_Address
    MPI_Type_extent
    MPI_Type_size
    MPI_Type_ub
    MPI_Type_lb

*/
