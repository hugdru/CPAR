#!/usr/bin/env bash

cd "${0%/*}"

main() {
  make clean
  make releaseO3

  local repetitions=6

  bench 1 600 400 3000 "$repetitions"
  bench 2 600 400 3000 "$repetitions"
  bench 3 600 400 3000 "$repetitions"
  bench 4 600 400 3000 "$repetitions"
  bench 5 600 400 3000 "$repetitions"
  bench 6 600 400 3000 "$repetitions"
  bench 7 600 400 3000 "$repetitions"
  bench 8 600 400 3000 "$repetitions"
  bench 9 600 400 3000 "$repetitions"
  bench 10 600 400 3000 "$repetitions"
}

bench() {
  if [[ $# -ne 5 ]]; then
    exit 1
  fi

  local option="$1"
  local start="$2"
  local incrementor="$3"
  local end="$4"
  local repetitions="$5"

  local current="$start"
  local current_repetition=1
  while [[ "$current" -le "$end" ]]; do
    local option_dir="benchPapi/$option"
    mkdir -p "$option_dir"

    bench_file="$option_dir/${current}_${current_repetition}.txt"
    echo "$bench_file"

    ./bin/matrixprod "$option" "$current" > "$bench_file" 2>&1

    if [[ "$current_repetition" -ge "$repetitions" ]]; then
      current_repetition=1
      ((current+="$incrementor"))
    else
      ((current_repetition++))
    fi
  done
}

main "$@"
