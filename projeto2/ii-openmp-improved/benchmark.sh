#!/usr/bin/env bash

set -e

cd "${0%/*}"

benchmark_file_path="../benchmarks/ii-openmp-improved.csv"
binary_path="./bin/ii-openmp-improved-sieve_of_erastosthenes"
n_repetitions=6

main() {
  make clean
  make releaseO3

  mkdir -p "${benchmark_file_path%/*}"
  rm -f "$benchmark_file_path"

  echo "threads,2^n,time(s)" >> "$benchmark_file_path"
  for n in {25..32}; do
    for number_of_threads in {1..4}; do
      min_time=''
      for ((repetition=0;repetition<n_repetitions;repetition++)); do
        current_time=$("$binary_path" "$n" "$number_of_threads")
        if [[ -z $min_time || "$(python -c "print($min_time > $current_time)")" == True ]]; then
          min_time=$current_time
        fi
      done
      echo "$number_of_threads,$n,$min_time" | tee -a "$benchmark_file_path"
    done
  done
}

main "$@"
