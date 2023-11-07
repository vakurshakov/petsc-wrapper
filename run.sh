#!/bin/bash

MPI_NUM_PROC=1

mpiexec -np $MPI_NUM_PROC ./a.out $@
