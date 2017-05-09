#!/usr/bin/env bash

set -e

cd "${0%/*}"

echo "**** Benchmarking i-sequential ****"
./i-sequential/benchmark.sh

echo "**** Benchmarking i-sequential-improved ****"
./i-sequential-improved/benchmark.sh

echo -e "\n\n**** Benchmarking ii-openmp ****"
./ii-openmp/benchmark.sh

echo -e "\n\n**** Benchmarking ii-openmp-improved ****"
./ii-openmp-improved/benchmark.sh

echo -e "\n\n**** Benchmarking iii-mpi ****"
./iii-mpi/benchmark.sh

echo -e "\n\n**** Benchmarking iv-mpiOpenmp ****"
./iv-mpiOpenmp/benchmark.sh
