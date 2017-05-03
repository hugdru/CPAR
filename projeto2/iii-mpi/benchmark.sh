#!/usr/bin/env bash

set -e

cd "${0%/*}"

benchmark_file_path="../benchmarks/iii-mpi.csv"
binary_path="./bin/iii-mpi-sieve_of_erastosthenes"
n_repetitions=3
host_file="./host_file"

main() {
  make clean
  make releaseO3

  rm -f "$benchmark_file_path"
  touch "$benchmark_file_path"

  for p in 1 2 4; do
    for n in {25..32}; do
      acumulator=0
      for ((repetition=0;repetition<n_repetitions;repetition++)); do
        # 1 2 4 processes in each node
        time=$(mpirun --hostfile "$host_file" --map-by ppr:$p:node "$binary_path" "$n")
        acumulator=$(python -c "print($acumulator + $time)")
      done
      echo "$p,$n,$(python -c "print($acumulator / $n_repetitions)")" >> "$benchmark_file_path"
    done
  done
}

main "$@"
