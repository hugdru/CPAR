#!/usr/bin/env bash

set -e

cd "${0%/*}"

benchmark_file_path="../benchmarks/ii-openmp.csv"
exec_path="./bin/ii-openmp-sieve_of_erastosthenes"
n_repetitions=3
number_of_threads=4

main() {
  make clean
  make releaseO3

  rm -f "$benchmark_file_path"
  touch "$benchmark_file_path"

  for n in {25..32}; do
    acumulator=0
    for ((repetition=0;repetition<n_repetitions;repetition++)); do
      time=$("$exec_path" "$n" "$number_of_threads")
      acumulator=$(python -c "print($acumulator + $time)")
    done
    echo "$n,$(python -c "print($acumulator / $n_repetitions)")" >> "$benchmark_file_path"
  done
}

main "$@"
