#!/bin/bash
rm -rf converted
mkdir converted
python3 converter.py ../projeto1/java/bench/1 converted/converted_java_1
python3 converter.py ../projeto1/java/bench/2 converted/converted_java_2
python3 converter.py ../projeto1/c#/dotnet/bench/1 converted/converted_c#_1
python3 converter.py ../projeto1/c#/dotnet/bench/2 converted/converted_c#_2 
python3 converter.py ../projeto1/cpp/benchPerf/1 converted/converted_cpp_perf_1
python3 converter.py ../projeto1/cpp/benchPerf/2 converted/converted_cpp_perf_2
python3 converter.py ../projeto1/cpp/benchPapi/3 converted/converted_cpp_papi_3
python3 converter.py ../projeto1/cpp/benchPapi/4 converted/converted_cpp_papi_4
python3 converter.py ../projeto1/cpp/benchPapi/5 converted/converted_cpp_papi_5
python3 converter.py ../projeto1/cpp/benchPapi/6 converted/converted_cpp_papi_6
python3 converter.py ../projeto1/cpp/benchPapi/7 converted/converted_cpp_papi_7
python3 converter.py ../projeto1/cpp/benchPapi/8 converted/converted_cpp_papi_8
python3 converter.py ../projeto1/cpp/benchPapi/9 converted/converted_cpp_papi_9
python3 converter.py ../projeto1/cpp/benchPapi/10 converted/converted_cpp_papi_10