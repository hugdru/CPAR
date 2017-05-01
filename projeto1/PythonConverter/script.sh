#!/bin/bash
cd "${0%/*}"
../c#/dotnet/bench.sh
../java/bench.sh
../cpp/benchPerf.sh
../cpp/benchPapi.sh
rm -rf converted
mkdir converted
python3 converter.py ../java/bench/1 converted/java-perf-lbc
python3 converter.py ../java/bench/2 converted/java-perf-lbl
python3 converter.py ../c#/dotnet/bench/1 converted/c#-perf-lbc
python3 converter.py ../c#/dotnet/bench/2 converted/c#-perf-lbl
python3 converter.py ../cpp/benchPerf/1 converted/cpp-perf-lbc
python3 converter.py ../cpp/benchPerf/2 converted/cpp-perf-lbl
python3 converter.py ../cpp/benchPapi/1 converted/cpp-papi-lbc
python3 converter.py ../cpp/benchPapi/2 converted/cpp-papi-lbl
python3 converter.py ../cpp/benchPapi/3 converted/cpp-papi-lbc1t
python3 converter.py ../cpp/benchPapi/4 converted/cpp-papi-lbc2t
python3 converter.py ../cpp/benchPapi/5 converted/cpp-papi-lbc3t
python3 converter.py ../cpp/benchPapi/6 converted/cpp-papi-lbc4t
python3 converter.py ../cpp/benchPapi/7 converted/cpp-papi-lbl1t
python3 converter.py ../cpp/benchPapi/8 converted/cpp-papi-lbl2t
python3 converter.py ../cpp/benchPapi/9 converted/cpp-papi-lbl3t
python3 converter.py ../cpp/benchPapi/10 converted/cpp-papi-lbl4t
