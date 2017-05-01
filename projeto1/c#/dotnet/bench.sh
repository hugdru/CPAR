#!/usr/bin/env bash

cd "${0%/*}"

main() {
  rm -rf ./bin ./obj
  dotnet restore
  dotnet build -c Release

  local repetitions=6

  bench 1 600 400 3000 "$repetitions"
  bench 2 600 400 3000 "$repetitions"
  bench 2 4000 2000 10000 "$repetitions"
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
    local option_dir="bench/$option"
    mkdir -p "$option_dir"

    bench_file="$option_dir/${current}_${current_repetition}.txt"
    echo "$bench_file"

    (perf stat -e bus-cycles,cache-misses,cache-references,cpu-cycles,instructions,ref-cycles,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-prefetch-misses,L1-dcache-store-misses,L1-dcache-stores,L1-icache-load-misses -- dotnet run -c Release "$option" "$current" ) > "$bench_file" 2>&1

    if [[ "$current_repetition" -ge "$repetitions" ]]; then
      current_repetition=1
      ((current+="$incrementor"))
    else
      ((current_repetition++))
    fi
  done
}

main "$@"
