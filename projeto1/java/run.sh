#!/usr/bin/env bash

set -e

cd "${0%/*}"

bin="./bin"
src="./src"

rm -rf "$bin"
mkdir -p "$bin"
javac -sourcepath "$src" $src/**/*.java -d "$bin"

perf stat -e bus-cycles,cache-misses,cache-references,cpu-cycles,instructions,ref-cycles,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-prefetch-misses,L1-dcache-store-misses,L1-dcache-stores,L1-icache-load-misses -- java -Xms3G -Xmx3G -classpath "$bin" matrixprod.Matrixprod "$@"
