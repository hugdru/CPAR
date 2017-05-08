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
  bool *sieved_vector = new bool[last_number - 1]{false};
  size_t limit = static_cast<size_t>(sqrt(static_cast<double>(last_number)));
  for (size_t k = 2; k <= limit;) {
    for (size_t multiple = k * k; multiple <= last_number; multiple += k) {
      sieved_vector[multiple - 2] = true;
    }
    do {
      k += 1;
    } while (k <= limit && sieved_vector[k - 2]);
  }
  clock_t end = clock();
  cout << static_cast<double>(end - start) / CLOCKS_PER_SEC << endl;

#ifndef NDEBUG
  size_t conter = 0;
  for (size_t number = 2; number <= last_number; ++number) {
    if (!sieved_vector[number - 2]) {
      cout << number << endl;
      conter++;
    };
  }
  cout << "primes found: " << conter << endl;
#endif

  delete[] sieved_vector;
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
