import sys
import re
from os import walk
from os import sep


def cmp(a, b):
    return (a > b)-(a < b)


def _compare_keys(x_file, y_file):
    try:
        x = int(x_file)
    except ValueError:
        xint = False
    else:
        xint = True
    try:
        y = int(y_file)
    except ValueError:
        if xint:
            return -1
        return cmp(x.lower(), y.lower())
    else:
        if xint:
            return cmp(x,y)
        return 1


def cmp_to_key(mycmp):
    class K(object):
        def __init__(self, obj, *args):
            self.obj = obj
        def __lt__(self, other):
            return mycmp(self.obj, other.obj) < 0
        def __gt__(self, other):
            return mycmp(self.obj, other.obj) > 0
        def __eq__(self, other):
            return mycmp(self.obj, other.obj) == 0
        def __le__(self, other):
            return mycmp(self.obj, other.obj) <= 0
        def __ge__(self, other):
            return mycmp(self.obj, other.obj) >= 0
        def __ne__(self, other):
            return mycmp(self.obj, other.obj) != 0
    return K


def get_result_value_int(num):
    try:
        value = str(int(num.replace(',', '')))
        return value if not value.isspace() else "<not supported>"
    except ValueError:
        return "<not supported>"


def convert_files_perf(files_map, name_out):
    with open(name_out + ".csv", "w") as out_file:
        for key in sorted(files_map, key=cmp_to_key(_compare_keys)):
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
                with open(sys.argv[1] + "/" + file, "r") as in_file:
                    print("adding: " + file)
                    for i, line in enumerate(in_file):
                        if len(line.split()) >= 2:
                            typee = line.split()[1]
                            if typee == "Time":
                                time.append('{:.3f}'.format(float(line.split()[0])))
                            else:
                                value = get_result_value_int(line.split()[0])
                                if typee == "bus-cycles":
                                    bus_cycles.append(value)
                                elif typee == "cache-misses":
                                    cache_misses.append(value)
                                elif typee == "cache-references":
                                    cache_references.append(value)
                                elif typee == "cpu-cycles":
                                    cpu_cycles.append(value)
                                elif typee == "instructions":
                                    instructions.append(value)
                                elif typee == "ref-cycles":
                                    ref_cycles.append(value)
                                elif typee == "stalled-cycles-frontend":
                                    stalled_cycles_frontend.append(value)
                                elif typee == "L1-dcache-load-misses":
                                    l1_dcache_load_misses.append(value)
                                elif typee == "L1-dcache-loads":
                                    l1_dcache_loads.append(value)
                                elif typee == "L1-dcache-prefetch-misses":
                                    l1_dcache_prefetch_misses.append(value)
                                elif typee == "L1-dcache-store-misses":
                                    l1_dcache_store_misses.append(value)
                                elif typee == "L1-dcache-stores":
                                    l1_dcache_stores.append(value)
                                elif typee == "L1-icache-load-misses":
                                    l1_icache_load_misses.append(value)
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


def convert_files_papi(files_map, name_out):
    with open(name_out + ".csv", "w") as out_file:
        for key in sorted(files_map, key=cmp_to_key(_compare_keys)):
            time = []
            papi_l1_dcm = []
            papi_l1_icm = []
            papi_l1_tcm = []
            papi_l2_dcm = []
            papi_l2_icm = []
            papi_l2_tcm = []
            papi_l3_dca = []
            papi_l3_ica = []
            papi_tot_ins = []

            for file in files_map[key]:
                with open(sys.argv[1] + "/" + file, "r") as in_file:
                    print("adding: " + file)
                    for i, line in enumerate(in_file):
                        if i == 2:
                            time.append('{:.3f}'.format(float(line.split()[0])))
                        elif i >= 4 and len(line.split()) >= 2:
                            typee = line.split()[0]
                            value = get_result_value_int(line.split()[1])
                            if typee == "PAPI_L1_DCM:":
                                papi_l1_dcm.append(value)
                            elif typee == "PAPI_L1_ICM:":
                                papi_l1_icm.append(value)
                            elif typee == "PAPI_L1_TCM:":
                                papi_l1_tcm.append(value)
                            elif typee == "PAPI_L2_DCM:":
                                papi_l2_dcm.append(value)
                            elif typee == "PAPI_L2_ICM:":
                                papi_l2_icm.append(value)
                            elif typee == "PAPI_L2_TCM:":
                                papi_l2_tcm.append(value)
                            elif typee == "PAPI_L3_DCA:":
                                papi_l3_dca.append(value)
                            elif typee == "PAPI_L3_ICA:":
                                papi_l3_ica.append(value)
                            elif typee == "PAPI_TOT_INS:":
                                papi_tot_ins.append(value)
            out_file.write(key + ",Time (s)," + ','.join(time) + '\n')
            out_file.write(",PAPI_L1_DCM," + ','.join(papi_l1_dcm) + '\n')
            out_file.write(",PAPI_L1_ICM," + ','.join(papi_l1_icm) + '\n')
            out_file.write(",PAPI_L1_TCM," + ','.join(papi_l1_tcm) + '\n')
            out_file.write(",PAPI_L2_DCM," + ','.join(papi_l2_dcm) + '\n')
            out_file.write(",PAPI_L2_ICM," + ','.join(papi_l2_icm) + '\n')
            out_file.write(",PAPI_L2_TCM," + ','.join(papi_l2_tcm) + '\n')
            out_file.write(",PAPI_L3_DCA," + ','.join(papi_l3_dca) + '\n')
            out_file.write(",PAPI_L3_ICA," + ','.join(papi_l3_ica) + '\n')
            out_file.write(",PAPI_TOT_INS," + ','.join(papi_tot_ins) + '\n')
            out_file.write(",,,," + '\n')
            out_file.write(",,,," + '\n')


def main():
    if len(sys.argv) <= 2:
        print('Usage: ' + str(sys.argv[0]) + ' <folder path> <new file name>')
        return

    map = {}
    folders = sys.argv[1].split(sep);
    print(sys.argv[2])
    for root, dirs, files in walk(sys.argv[1]):
        for file in files:
            key = file.split("_")[0]
            if re.search("\d+_\d+.txt", file, flags=0):
                if key not in map:
                    map[key] = []
                map[key].append(file)
    if "bench" in folders or "benchPerf" in folders:
        convert_files_perf(map, sys.argv[2])
    elif "benchPapi" in folders:
        convert_files_papi(map, sys.argv[2])
    else: 
        print("error: folder 'bench', 'benchPerf' or 'benchPapi' not found in the path")

if __name__ == "__main__":
    main()
