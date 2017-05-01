#!/usr/bin/env bash

set -e

cd "${0%/*}"

rm -rf pkg bin

if ! type gb > /dev/null 2>&1; then
  go get github.com/constabulary/gb/...
fi

gb build

perf stat -e bus-cycles,cache-misses,cache-references,cpu-cycles,instructions,ref-cycles,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-prefetch-misses,L1-dcache-store-misses,L1-dcache-stores,L1-icache-load-misses -- ./bin/matrixprod "$@"
