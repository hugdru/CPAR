#!/usr/bin/env bash

set -e

cd "${0%/*}"

benchmark_file_path="../benchmarks/iii-mpi.csv"
binary_path="./bin/iii-mpi-sieve_of_erastosthenes"
n_repetitions=6
host_file="./host_file"

main() {
  make clean
  make releaseO3

  mkdir -p "${benchmark_file_path%/*}"
  rm -f "$benchmark_file_path"

  echo "processes/node,2^n,time(s)" >> "$benchmark_file_path"
  for p in 1 2 4; do
    for n in {25..32}; do
      min_time=''
      for ((repetition=0;repetition<n_repetitions;repetition++)); do
        # 1 2 4 processes in each node
        current_time=$(mpirun --hostfile "$host_file" --map-by ppr:$p:node "$binary_path" "$n")
        if [[ -z $min_time || "$(python -c "print($min_time > $current_time)")" == True ]]; then
          min_time=$current_time
        fi
      done
      echo "$p,$n,$min_time" | tee -a "$benchmark_file_path"
    done
  done
}

main "$@"
