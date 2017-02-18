JC = javac

SRC = ./src/
BIN = ./bin/

default:
	mkdir -p "${BIN}"
	javac -sourcepath "${SRC}" ${SRC}/**/*.java -d "${BIN}"
	java -classpath "${BIN}" matrixprod.Matrixprod

clean:
	rm -rf "${BIN}"
