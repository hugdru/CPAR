#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

#include "mpi.h"

#define ROOT_MACHINE 0

// taken from the class pdf
#define BLOCK_LOW(i,n,p) ((i)*(n)/(p))
#define BLOCK_HIGH(i,n,p) (BLOCK_LOW((i)+1,n,p)-1)
#define BLOCK_SIZE(i,n,p) (BLOCK_LOW((i)+1,n,p)-BLOCK_LOW(i,n,p))
#define BLOCK_OWNER(index,n,p) ((((p)*(index)+1)-1)/(n))

using std::cout;
using std::endl;
using std::vector;
using std::cerr;
using std::string;
using std::ostringstream;

size_t parseCmd(int argc, char ** argv);
void help(char *program_name, bool quit = true);

int main(int argc, char** argv) {
  double start = 0, end = 0;

  size_t last_number = parseCmd(argc,argv);

  int rank, size;

  // mpi init needs to receve the argc and argv
  MPI_Init( &argc, &argv );

  MPI_Comm_size( MPI_COMM_WORLD, &size);
  MPI_Comm_rank( MPI_COMM_WORLD, &rank);

  // ------------ MPI CODE ----------------

  //clock_t start = clock();
  size_t limit = static_cast<size_t>(last_number - 1);

  unsigned long blockSize = BLOCK_SIZE(rank, limit, size);
  unsigned long blockLow = BLOCK_LOW(rank, limit, size) + 2;
  unsigned long blockHigh = BLOCK_HIGH(rank, limit, size) + 2;

  bool *sieved_vector = new bool[blockSize] {false};

  MPI_Barrier(MPI_COMM_WORLD);
  if(rank == ROOT_MACHINE)
    start = MPI_Wtime();

  size_t k = 2;
  size_t startBlock;
  while(k*k < limit) {
    // to determine the block start index for the peer
    if (k*k < blockLow) {
      startBlock = blockLow;// se k * k < comeco do block vai diretamente para o comeco do bloco
      if (blockLow % k != 0)
        startBlock += (k - (blockLow % k));
    } else {
      startBlock = k * k;
    }

    // mark multiples
    for (size_t multiple = startBlock; multiple <= blockHigh; multiple += k) {
      sieved_vector[multiple - blockLow] = true;
    }

    //
    if(rank == ROOT_MACHINE) {
      do {
        k++;
      } while (k*k < blockHigh && sieved_vector[k - blockLow]);
    }
    // int MPI_Bcast(void *buffer, int count, MPI_Datatype datatype, int root, MPI_Comm comm)
    // buffer -> buffer to pass
    // count -> size of buffer
    // datatype -> type of buffer (this case unsigned int)
    // root -> root machine (rank = 0)
    // comm -> we are going to use the default communicator (MPI_COMM_WORLD for simple group applications)
    MPI_Bcast(&k, 1, MPI_UNSIGNED, ROOT_MACHINE, MPI_COMM_WORLD);
  }
  // clock_t end = clock();



  //ostringstream sstream;
  //sstream << "Time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC
  //        << endl;
  //cout << sstream.str();

  #ifndef NDEBUG
    cout << "Primes found by pc rank (" << rank << ")" << endl;
  #endif

  size_t blockPrimes = 0;
  for (size_t number = 0; number < blockSize; number++) {
    if (!sieved_vector[number]) {
      #ifndef NDEBUG
        cout << (blockLow + number) << ", ";
      #endif
      blockPrimes++;
    }
  }
  cout << endl;

  size_t AllBlocksPrimes = 0;
  //http://mpitutorial.com/tutorials/mpi-reduce-and-allreduce/
  MPI_Reduce(&blockPrimes, &AllBlocksPrimes, 1, MPI_UNSIGNED, MPI_SUM, ROOT_MACHINE, MPI_COMM_WORLD);
  if(rank == ROOT_MACHINE) {
    end = MPI_Wtime();
    cout << "Primes found: " << AllBlocksPrimes << endl;
    cout << "Time taken: " << (end - start) << endl;
  }
  // crear the vectorEE
  delete[] sieved_vector;

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
