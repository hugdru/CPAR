#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include <mpi.h>

#define ROOT_MACHINE 0

// taken from the class pdf
#define BLOCK_LOW(i, n, p) ((i)*(n)/(p))
#define BLOCK_HIGH(i, n, p) (BLOCK_LOW((i)+1,n,p)-1)
#define BLOCK_SIZE(i, n, p) (BLOCK_LOW((i)+1,n,p)-BLOCK_LOW(i,n,p))

using std::cout;
using std::endl;
using std::vector;
using std::cerr;
using std::string;
using std::ostringstream;

size_t parseCmd(int argc, char **argv);
void help(char *program_name, bool quit = true);

int main(int argc, char **argv) {
  double start = 0, end = 0;

  size_t last_number = parseCmd(argc, argv);

  int rank, size;

  // mpi init needs to receve the argc and argv
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

#ifndef NDEBUG
  cout << "size (" << size << ") | rank(" << rank << ")" << endl;
#endif
  // ------------ MPI CODE ----------------

  size_t number_odd_elements = last_number / 2;
  unsigned long blockSize = BLOCK_SIZE(rank, number_odd_elements, size);
  unsigned long blockLow = BLOCK_LOW(rank, number_odd_elements, size) * 2 + 1;
  unsigned long blockHigh = BLOCK_HIGH(rank, number_odd_elements, size) * 2 + 1;
  vector<bool> sieved_vector(blockSize, false);

  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == ROOT_MACHINE) {
    start = MPI_Wtime();
  }

  size_t startBlock;
  for (size_t k = 3; k <= static_cast<size_t>(sqrt(last_number));) {
    if (k * k < blockLow) {
      startBlock = blockLow;
      if (blockLow % k != 0) {
        startBlock += -(blockLow % k) + k;
        if (startBlock % 2 == 0) {
          startBlock += k;
        }
      }
    } else {
      startBlock = k * k;
    }

    for (size_t multiple = startBlock; multiple <= blockHigh; multiple += 2 * k) {
      sieved_vector[(multiple - blockLow) / 2] = true;
    }

    if (rank == ROOT_MACHINE) {
      k += 2;
    }
    MPI_Bcast(&k, 1, MPI_UNSIGNED, ROOT_MACHINE, MPI_COMM_WORLD);
  }

  size_t blockPrimes = 0;
  size_t start_value = blockLow;
  if (rank == ROOT_MACHINE) {
#ifndef NDEBUG
    cout << "2 rank(" << rank << ")" << endl;
#endif
    start_value += 2;
    blockPrimes += 1;
  }
  for (size_t number = start_value; number <= blockHigh; number += 2) {
    if (!sieved_vector[(number - blockLow) / 2]) {
#ifndef NDEBUG
      cout << number << " rank(" << rank << ")" << endl;
#endif
      blockPrimes++;
    }
  }

  size_t AllBlocksPrimes = 0;
  //http://mpitutorial.com/tutorials/mpi-reduce-and-allreduce/
  MPI_Reduce(&blockPrimes, &AllBlocksPrimes, 1, MPI_UNSIGNED, MPI_SUM, ROOT_MACHINE, MPI_COMM_WORLD);
  if (rank == ROOT_MACHINE) {
    end = MPI_Wtime();
#ifndef NDEBUG
    cout << "Primes found: " << AllBlocksPrimes << endl;
    cout << "Time taken: ";
#endif
    cout << (end - start) << endl;
  }

  MPI_Finalize();

  return 0;
}

size_t parseCmd(int argc, char **argv) {
  char *program_name_ptr = argv[0];

  if (argc != 2) {
    help(program_name_ptr);
  }

  char *exponent_ptr = argv[1];

  size_t exponent = strtoul(exponent_ptr, nullptr, 10);
  if (errno == ERANGE) {
    std::cout << "range error, got " << exponent_ptr;
    help(program_name_ptr);
  }

  if (exponent < 1) {
    help(program_name_ptr);
  }

  return pow(2, exponent);
}

void help(char *program_name, bool quit) {
  cerr << "This program calculates prime numbers up to powe(2,N), N must be >= 1"
       << endl;
  cerr << "usage: " << program_name << " N" << endl;

  if (quit) {
    exit(EXIT_FAILURE);
  }
}
