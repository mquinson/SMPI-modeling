/* Copyright (c) 2009-2010, 2013-2014. The SimGrid Team.
 * All rights reserved.                                                     */

/* This program is free software; you can redistribute it and/or modify it
 * under the terms of the license (GNU LGPL) which comes with this package. */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "mpi.h"

#ifndef EXIT_SUCCESS
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

int main(int argc, char *argv[])
{
  int rank, proc_count;
  int i;
  char *sb;
  char *rb;
  int status;
  int datasize;
   
  /* Make sure that previous output are written over the NFS even if G5K decides to kill the current machine */
  fdatasync(1);
  fdatasync(2);
   
  MPI_Init(&argc, &argv);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &proc_count);

  if (argc<2 || atoi(argv[1]) == 0) {
     printf("Usage: alltoall datasize\n");
     exit(1);
  }
  datasize=atoi(argv[1]);
   
  rb = sb = (char *) SMPI_SHARED_MALLOC(proc_count * datasize);

  if (sb == NULL) {
     printf("Malloc error");
     exit(1);
  }   
   
  memset(sb, 1, proc_count * datasize);

  status = MPI_Alltoall(sb, datasize, MPI_CHAR, rb, datasize, MPI_CHAR, MPI_COMM_WORLD);

  if (rank == 0) {
    if (status != MPI_SUCCESS) {
      printf("[%f] all_to_all returned %d", MPI_Wtime(), status);
      fflush(stdout);
      return status;
    } else  {
      printf("simTime:%f Success numproc=%d msgsize=%d", MPI_Wtime(),proc_count,datasize);
    }
  }
  SMPI_SHARED_FREE(sb);
  MPI_Finalize();
  return EXIT_SUCCESS;
}
