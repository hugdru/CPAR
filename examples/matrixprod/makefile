SRC = $(wildcard src/*.cpp)

OBJ = $(patsubst src%, ./buildtemp%.o, $(SRC))

OUT = bin/matrixprod

# include directories
INCLUDES = -I./src -I/usr/local/include

# compiler
CCC = g++ -std=c++14

# library paths
LIBS = -L/usr/local/lib -lpapi -fopenmp -lpthread

.SUFFIXES: .cpp

default: debug
debug: CCFLAGS = -ggdb -pipe -Wundef -Wstrict-overflow=5 -Wsign-promo -Woverloaded-virtual -Wold-style-cast -Wctor-dtor-privacy -Wformat=2 -Winvalid-pch -Wmissing-include-dirs -Wpacked -Wpadded -Wall -Wextra -pedantic -Wdouble-promotion -Wshadow -Wfloat-equal -Wcast-align -Wcast-qual -Wwrite-strings -Wconversion -Wsign-conversion -Wmissing-declarations -Wredundant-decls -Wdisabled-optimization -Wstack-protector -Winline -Wswitch-default -Wswitch-enum -Wuseless-cast -Wlogical-op -Wzero-as-null-pointer-constant -Wnoexcept -Wstrict-null-sentinel
debug: $(OUT)

release: CCFLAGS = -O2 -pipe -s -DNDEBUG -D_FORTIFY_SOURCE=2 -fstack-protector-strong -Wall -Wstack-protector
release: $(OUT)

./buildtemp/%.o: src/%
	mkdir -p ./buildtemp
	$(CCC) $(INCLUDES) $(CCFLAGS) -c $< -o $@

$(OUT): $(OBJ)
	mkdir -p bin
	$(CCC) $(INCLUDES) $(CCFLAGS) $(OBJ) $(LIBS) -o $(OUT)

clean:
	rm -f $(OBJ) $(OUT)

test:
	echo $(SRC)
	echo $(OBJ)
