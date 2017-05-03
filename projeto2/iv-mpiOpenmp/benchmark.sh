#!/usr/bin/env bash

set -e

cd "${0%/*}"

benchmark_file_path="../benchmarks/iv-mpiOpenmp.csv"
binary_path="./bin/iv-mpiOpenmp-sieve_of_erastosthenes"
n_repetitions=3
host_file="./host_file"

main() {
  make clean
  make releaseO3

  rm -f "$benchmark_file_path"
  touch "$benchmark_file_path"

  for number_of_threads in 1 2 4; do
    for n in {25..32}; do
      acumulator=0
      for ((repetition=0;repetition<n_repetitions;repetition++)); do
        # Only one process per node to test openmp with 1 2 4 threads in each node
        time=$(mpirun --hostfile "$host_file" --map-by ppr:1:node "$binary_path" "$n" "$number_of_threads")
        acumulator=$(python -c "print($acumulator + $time)")
      done
      echo "$number_of_threads,$n,$(python -c "print($acumulator / $n_repetitions)")" >> "$benchmark_file_path"
    done
  done
}

main "$@"
