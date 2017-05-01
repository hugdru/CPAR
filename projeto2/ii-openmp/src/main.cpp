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
  vector<bool> sieved_vector(parsed.last_number - 1, false);
  size_t limit =
      static_cast<size_t>(sqrt(static_cast<double>(last_number)));
  for (size_t k = 2; k <= limit;) {
#pragma omp parallel for num_threads(parsed.number_of_threads) schedule(static)
    for (size_t multiple = k * k; multiple <= parsed.last_number;
         multiple += k) {
      sieved_vector[multiple - 2] = true;
    }
    do {
      ++k;
    } while (k <= limit && sieved_vector[k - 2]);
  }
  double end = omp_get_wtime();
  ostringstream sstream;
  sstream << "Time: " << end - start << endl;
  cout << sstream.str();

#ifndef NDEBUG
  for (size_t number = 2; number <= parsed.last_number; ++number) {
    if (!sieved_vector[number - 2]) {
      cout << number << endl;
    };
  }
#endif

  return EXIT_SUCCESS;
}

void parseCmd(const int argc, char const *const *const argv, parsed_t &parsed) {
  char const *const program_name_ptr = argv[0];
  if (argc != 3) {
    help(program_name_ptr);
  }

  char const *const last_number_ptr = argv[1];
  size_t last_number = strtoul(last_number_ptr, nullptr, 10);
  if (errno == ERANGE) {
    cerr << "range error, got " << last_number_ptr;
    help(program_name_ptr);
  }
  if (last_number < 2) {
    help(program_name_ptr);
  }
  parsed.last_number = last_number;

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
  cerr << "This program calculates prime numbers up to N with X threads, N >= "
          "2 and X > 0"
       << endl;
  cerr << "usage: " << program_name << " N X" << endl;

  if (quit) {
    exit(EXIT_FAILURE);
  }
}
