#include <omp.h>
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
using std::stoi;
using std::invalid_argument;
using std::out_of_range;

using parsed_t = struct {
  size_t last_number;
  int number_of_threads;
};

void parseCmd(const int argc, char const *const *const argv, parsed_t &parsed);
void help(char const *const program_name, bool quit = true);

int main(const int argc, char const *const *const argv) {
  parsed_t parsed;
  parseCmd(argc, argv, parsed);

  double start = omp_get_wtime();

  bool *sieved_vector = new bool[parsed.last_number / 2]{false};
  size_t limit = static_cast<size_t>(sqrt(parsed.last_number));
#pragma omp parallel for num_threads(parsed.number_of_threads) schedule(static)
  for (size_t k = 3; k <= limit; k += 2) {
    if (!sieved_vector[k / 2]) {
      for (size_t multiple = k * k; multiple < parsed.last_number; multiple += 2 * k) {
        sieved_vector[multiple / 2] = true;
      }
    }
  }
  double end = omp_get_wtime();
  cout << end - start << endl;

#ifndef NDEBUG
  size_t conter = 1;
  cout << 2 << ", ";
  for (size_t k = 3; k < parsed.last_number; k += 2) {
    if (!sieved_vector[k / 2]) {
      cout << k << ", ";
      conter++;
    }
  }
  cout << "primes found: " << conter << endl;
#endif

  delete[] sieved_vector;
  return EXIT_SUCCESS;
}

void parseCmd(const int argc, char const *const *const argv, parsed_t &parsed) {
  char const *const program_name_ptr = argv[0];
  if (argc != 3) {
    help(program_name_ptr);
  }

  char const *const exponent_ptr = argv[1];
  size_t exponent = strtoul(exponent_ptr, nullptr, 10);
  if (errno == ERANGE) {
    cerr << "range error, got " << exponent_ptr;
    help(program_name_ptr);
  }

  if (exponent < 1) {
    help(program_name_ptr);
  }
  parsed.last_number = pow(2, exponent);

  char const *const number_of_threads_ptr = argv[2];
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

void help(char const *const program_name, bool quit) {
  cerr << "This program calculates prime numbers up to pow(2, N) with X threads, N >= "
      "1 and X > 0"
       << endl;
  cerr << "usage: " << program_name << " N X" << endl;

  if (quit) {
    exit(EXIT_FAILURE);
  }
}
