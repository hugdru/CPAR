import sys
import re
from os import walk


def get_result_value_int(num):
    try:
        return str(int(num))
    except ValueError:
        return "<not supported>"


def convert_files(files_map):
    out_file = open("converted.csv", "w")
    for key in files_map:
        time = []
        bus_cycles = []
        cache_misses = []
        cache_references = []
        cpu_cycles = []
        instructions = []
        ref_cycles = []
        stalled_cycles_frontend = []
        l1_dcache_load_misses = []
        l1_dcache_loads = []
        l1_dcache_prefetch_misses = []
        l1_dcache_store_misses = []
        l1_dcache_stores = []
        l1_icache_load_misses = []

        for file in files_map[key]:
            in_file = open(file, "r")
            for i, line in enumerate(in_file):
                if i == 1:
                    time.append('{:.3f}'.format(float(line[:-1])))
                elif i == 7:
                    bus_cycles.append(get_result_value_int(line.split()[0]))
                elif i == 8:
                    cache_misses.append(get_result_value_int(line.split()[0]))
                elif i == 9:
                    cache_references.append(get_result_value_int(line.split()[0]))
                elif i == 10:
                    cpu_cycles.append(get_result_value_int(line.split()[0]))
                elif i == 11:
                    instructions.append(get_result_value_int(line.split()[0]))
                elif i == 12:
                    ref_cycles.append(get_result_value_int(line.split()[0]))
                elif i == 13:
                    stalled_cycles_frontend.append(get_result_value_int(line.split()[0]))
                elif i == 14:
                    l1_dcache_load_misses.append(get_result_value_int(line.split()[0]))
                elif i == 15:
                    l1_dcache_loads.append(get_result_value_int(line.split()[0]))
                elif i == 16:
                    l1_dcache_prefetch_misses.append(get_result_value_int(line.split()[0]))
                elif i == 17:
                    l1_dcache_store_misses.append(get_result_value_int(line.split()[0]))
                elif i == 18:
                    l1_dcache_stores.append(get_result_value_int(line.split()[0]))
                elif i == 19:
                    l1_icache_load_misses.append(get_result_value_int(line.split()[0]))
                elif i >= 20:
                    break
            in_file.close()
        out_file.write(key + ",Time (s)," + ','.join(time) + '\n')
        out_file.write(",bus-cycles," + ','.join(bus_cycles) + '\n')
        out_file.write(",cache-misses," + ','.join(cache_misses) + '\n')
        out_file.write(",cache-references," + ','.join(cache_references) + '\n')
        out_file.write(",cpu-cycles," + ','.join(cpu_cycles) + '\n')
        out_file.write(",instructions," + ','.join(instructions) + '\n')
        out_file.write(",ref-cycles," + ','.join(ref_cycles) + '\n')
        out_file.write(",stalled-cycles-frontend," + ','.join(stalled_cycles_frontend) + '\n')
        out_file.write(",L1-dcache-load-misses," + ','.join(l1_dcache_load_misses) + '\n')
        out_file.write(",L1-dcache-loads," + ','.join(l1_dcache_loads) + '\n')
        out_file.write(",L1-dcache-prefetch-misses," + ','.join(l1_dcache_prefetch_misses) + '\n')
        out_file.write(",L1-dcache-store-misses," + ','.join(l1_dcache_store_misses) + '\n')
        out_file.write(",L1-dcache-stores," + ','.join(l1_dcache_stores) + '\n')
        out_file.write(",L1-icache-load-misses," + ','.join(l1_icache_load_misses) + '\n')
        out_file.write(",,,," + '\n')
        out_file.write(",,,," + '\n')
    out_file.close()


def main():
    if len(sys.argv) <= 1:
        print('Usage: ' + str(sys.argv[0]) + ' <folder path>')
        return

    f = [["" for i in range(3)]]
    map = {}
    for root, dirs, files in walk(sys.argv[1]):
        for file in files:
            key = file.split("_")[0]
            if re.search("\d+_\d+.txt", file, flags=0):
                if key not in map:
                    map[key] = []
                map[key].append(file)
    convert_files(map)


if __name__ == "__main__":
    main()
