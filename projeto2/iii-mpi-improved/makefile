SRC = $(wildcard src/*.cpp)

OBJ = $(patsubst src%, ./buildtemp%.o, $(SRC))

OUT = bin/iii-mpi-improved-sieve_of_erastosthenes

# include directories
INCLUDES = -I./src -I/usr/local/include

# compiler

CCC = mpic++ -std=c++14

# library paths
LIBS = -L/usr/local/lib

.SUFFIXES: .cpp

default: releaseO3
debug: CCFLAGS = -ggdb -pipe -Wundef -Wstrict-overflow=5 -Wsign-promo -Woverloaded-virtual -Wold-style-cast -Wctor-dtor-privacy -Wformat=2 -Winvalid-pch -Wmissing-include-dirs -Wpacked -Wpadded -Wall -Wextra -pedantic -Wdouble-promotion -Wshadow -Wfloat-equal -Wcast-align -Wcast-qual -Wwrite-strings -Wconversion -Wsign-conversion -Wmissing-declarations -Wredundant-decls -Wdisabled-optimization -Winline -Wswitch-default -Wswitch-enum -Wuseless-cast -Wlogical-op -Wzero-as-null-pointer-constant -Wnoexcept -Wstrict-null-sentinel
debug: $(OUT)

release: CCFLAGS = -O2 -pipe -s -DNDEBUG -D_FORTIFY_SOURCE=1 -fstack-protector-strong -Wall -Wdisabled-optimization -Wstack-protector -Winline
release: $(OUT)

releaseO2: CCFLAGS = -O2 -pipe -s -DNDEBUG -Wall -Wdisabled-optimization -Wstack-protector -Winline
releaseO2: $(OUT)

releaseO3: CCFLAGS = -O3 -pipe -s -DNDEBUG -Wall -Wdisabled-optimization -Wstack-protector -Winline
releaseO3: $(OUT)

./buildtemp/%.o: src/%
	mkdir -p ./buildtemp
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	mkdir -p bin
	$(CCC) $(INCLUDES) $(CCFLAGS) $(OBJ) $(LIBS) $(COMPILE_AND_LINK_COMMON_OPTIONS) -o $(OUT)

clean:
	rm -f $(OBJ) $(OUT)

test:
	echo $(SRC)
	echo $(OBJ)
