#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using std::cerr;
using std::string;
using std::ostringstream;

size_t parseCmd(const int argc, char const *const *const argv);
void help(char const *const program_name, bool quit = true);

int main(const int argc, char const *const *const argv) {
  size_t last_number = parseCmd(argc, argv);

  clock_t start = clock();
  // vector<bool> acts like a dynamic_bitset
  // that is a bitset whose size is only known at runtime
  // It is faster and a lot more memory efficient (1/8) than an array of
  // booleans, given that more information can be cached. bits vs bytes.
  // The instruction overhead of getting the bit in the byte is more and more
  // insignificant as the last_number increases because of the cache hits.
  // The array of booleans is only advantageous for a very small range which is
  // not expected in the use of this algorithm.
  // Also "initializing" the vector to false == zero is a lot faster.
  // Because with the right operating system call we get pre scrubbed zero memory
  // http://en.cppreference.com/w/cpp/utility/bitset in notes
  // http://stackoverflow.com/questions/2688466/why-mallocmemset-is-slower-than-calloc
  vector<bool> sieved_vector(last_number / 2, false);
  size_t limit = static_cast<size_t>(sqrt(last_number));
  for (size_t k = 3; k <= limit; k += 2) {
    if (!sieved_vector[k / 2]) {
      for (size_t multiple = k * k; multiple < last_number; multiple += 2 * k) {
        sieved_vector[multiple / 2] = true;
      }
    }
  }
  clock_t end = clock();
  cout << static_cast<double>(end - start) / CLOCKS_PER_SEC << endl;

#ifndef NDEBUG
  size_t conter = 1;
  cout << 2 << ", ";
  for (size_t k = 3; k < last_number; k += 2) {
    if (!sieved_vector[k / 2]) {
      cout << k << ", ";
      conter++;
    }
  }
  cout << "primes found: " << conter << endl;
#endif

  return EXIT_SUCCESS;
}

size_t parseCmd(const int argc, char const *const *const argv) {
  char const *const program_name_ptr = argv[0];

  if (argc != 2) {
    help(program_name_ptr);
  }

  char const *const exponent_ptr = argv[1];

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

void help(char const *const program_name, bool quit) {
  cerr << "This program calculates prime numbers up to pow(N,2), N must be >= 1"
       << endl;
  cerr << "usage: " << program_name << " N" << endl;

  if (quit) {
    exit(EXIT_FAILURE);
  }
}
