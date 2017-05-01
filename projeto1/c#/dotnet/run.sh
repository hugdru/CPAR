#!/usr/bin/env bash

set -e

cd "${0%/*}"

rm -rf ./bin ./obj
dotnet restore
dotnet build -c Release

perf stat -e bus-cycles,cache-misses,cache-references,cpu-cycles,instructions,ref-cycles,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-prefetch-misses,L1-dcache-store-misses,L1-dcache-stores,L1-icache-load-misses -- dotnet run -c Release "$@"
