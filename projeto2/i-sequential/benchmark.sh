#!/usr/bin/env bash

set -e

cd "${0%/*}"

benchmark_file_path="../benchmarks/i-sequential.csv"
binary_path="./bin/i-sequential-sieve_of_erastosthenes"
n_repetitions=3

main() {
  make clean
  make releaseO3

  rm -f "$benchmark_file_path"

  echo "2^n,time(s)" >> "$benchmark_file_path"
  for n in {25..32}; do
    min_time=''
    for ((repetition=0;repetition<n_repetitions;repetition++)); do
      current_time=$("$binary_path" $n)
      if [[ -z $min_time || "$(python -c "print($min_time > $current_time)")" == True ]]; then
        min_time=$current_time
      fi
    done
    echo "$n,$min_time" | tee -a "$benchmark_file_path"
  done
}

main "$@"
