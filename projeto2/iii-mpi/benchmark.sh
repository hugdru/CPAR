#!/usr/bin/env bash

set -e

cd "${0%/*}"

benchmark_file_path="../benchmarks/iii-mpi.csv"
binary_path="./bin/iii-mpi-sieve_of_erastosthenes"
n_repetitions=6
host_files_dir_path="../host_files"
host_files=("host_file_1pc" "host_file_2pc" "host_file_3pc" "host_file_4pc")

main() {
  make clean
  make releaseO3

  mkdir -p "${benchmark_file_path%/*}"
  rm -f "$benchmark_file_path"

  echo "pcs,processes/node,2^n,time(s)" >> "$benchmark_file_path"
  for host_file in "${host_files[@]}"; do
    for p in {1..4}; do
      for n in {25..32}; do
        min_time=''
        for ((repetition=0;repetition<n_repetitions;repetition++)); do
          # 1 2 4 processes in each node
          current_time=$(mpirun --hostfile "$host_files_dir_path/$host_file" --map-by ppr:$p:node "$binary_path" "$n")
          if [[ -z $min_time || "$(python -c "print($min_time > $current_time)")" == True ]]; then
            min_time=$current_time
          fi
        done
        echo "${host_file##*_},$p,$n,$min_time" | tee -a "$benchmark_file_path"
      done
    done
  done
}

main "$@"
