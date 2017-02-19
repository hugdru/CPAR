#!/usr/bin/env bash

cd "${0%/*}"

rm -rf __pycache__/
python -m compileall .
perf stat -e bus-cycles,cache-misses,cache-references,cpu-cycles,instructions,ref-cycles,stalled-cycles-frontend,L1-dcache-load-misses,L1-dcache-loads,L1-dcache-prefetch-misses,L1-dcache-store-misses,L1-dcache-stores,L1-icache-load-misses -- python3 matrixprod.py "$@"
