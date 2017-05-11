#include <omp.h>
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
#define BLOCK_OWNER(index, n, p) ((((p)*(index)+1)-1)/(n))

using std::cout;
using std::endl;
using std::vector;
using std::cerr;
using std::string;
using std::ostringstream;
using std::stoi;
using std::invalid_argument;
using std::out_of_range;

using parsed_t = struct {
  size_t last_number = 0;
  int number_of_threads = 0;
};

void parseCmd(int argc, char **argv, parsed_t &parsed);
void help(char *program_name, bool quit = true);

int main(int argc, char **argv) {
  parsed_t parsed = {2, 1};
  parseCmd(argc, argv, parsed);

  double start = 0, end = 0;

  int rank, size;

  // mpi init needs to receve the argc and argv
  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

#ifndef NDEBUG
  cout << "size (" << size << ") | rank(" << rank << ")" << endl;
#endif
  // ------------ MPI CODE ----------------

  //clock_t start = clock();
  size_t number_odd_elements = parsed.last_number / 2;
  unsigned long blockSize = BLOCK_SIZE(rank, number_odd_elements, size);
  unsigned long blockLow = BLOCK_LOW(rank, number_odd_elements, size) * 2 + 1;
  unsigned long blockHigh = BLOCK_HIGH(rank, number_odd_elements, size) * 2 + 1;

  bool *sieved_vector = new bool[blockSize]{false};

  MPI_Barrier(MPI_COMM_WORLD);
  if (rank == ROOT_MACHINE)
    start = MPI_Wtime();


  size_t startBlock;
  for (size_t k = 3; k <= static_cast<size_t>(sqrt(parsed.last_number));) {
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

#pragma omp parallel for num_threads(parsed.number_of_threads) schedule(static)
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

  // crear the vectorEE
  delete[] sieved_vector;

  MPI_Finalize();

  return 0;
}

void parseCmd(int argc, char **argv, parsed_t &parsed) {
  char *program_name_ptr = argv[0];
  if (argc != 3) {
    help(program_name_ptr);
  }

  char *exponent_ptr = argv[1];
  size_t exponent = strtoul(exponent_ptr, nullptr, 10);
  if (errno == ERANGE) {
    cerr << "range error, got " << exponent_ptr;
    help(program_name_ptr);
  }

  if (exponent < 1) {
    help(program_name_ptr);
  }
  parsed.last_number = pow(2, exponent);

  char *number_of_threads_ptr = argv[2];
  try {
    int number_of_threads = stoi(number_of_threads_ptr, nullptr, 10);
    if (number_of_threads < 1) {
      help(program_name_ptr);
    }
    parsed.number_of_threads = number_of_threads;
  } catch (invalid_argument e) {
    cerr << e.what();
    help(program_name_ptr);
  } catch (out_of_range e) {
    cerr << e.what();
    help(program_name_ptr);
  }
}

void help(char *program_name, bool quit) {
  cerr << "This program calculates prime numbers up to pow(2,N) with X threads, N >= "
      "1 and X > 0"
       << endl;
  cerr << "usage: " << program_name << " N X" << endl;

  if (quit) {
    exit(EXIT_FAILURE);
  }
}
