#!/usr/bin/env bash

cd "${0%/*}"

bin="./matrixprod/matrixprod/bin/Release/matrixprod.exe"

if [[ ! -f "$bin" ]]; then
  1>&2 echo "$bin not found compile for Release."
  exit 1
fi

perf stat -e bus-cycles,cache-misses,cache-references,cpu-cycles,instructions,ref-cycles,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-prefetch-misses,L1-dcache-store-misses,L1-dcache-stores,L1-icache-load-misses -- mono "$bin" "$@"
