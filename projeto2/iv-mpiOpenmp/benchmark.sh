#!/usr/bin/env bash

set -e

cd "${0%/*}"

benchmark_file_path="../benchmarks/iv-mpiOpenmp.csv"
binary_path="./bin/iv-mpiOpenmp-sieve_of_erastosthenes"
n_repetitions=6
host_file="./host_file"
processes_per_node=1

main() {
  make clean
  make releaseO3

  mkdir -p "${benchmark_file_path%/*}"
  rm -f "$benchmark_file_path"

  echo "threads,processes/node,2^n,time(s)" >> "$benchmark_file_path"
  for number_of_threads in 1 2 4; do
    for n in {25..32}; do
      min_time=''
      for ((repetition=0;repetition<n_repetitions;repetition++)); do
        # Only one process per node to test openmp with 1 2 4 threads in each node
        current_time=$(mpirun --hostfile "$host_file" --map-by ppr:$processes_per_node:node "$binary_path" "$n" "$number_of_threads")
        if [[ -z $min_time || "$(python -c "print($min_time > $current_time)")" == True ]]; then
          min_time=$current_time
        fi
      done
      echo "$number_of_threads,$processes_per_node,$n,$min_time" | tee -a "$benchmark_file_path"
    done
  done
}

main "$@"
