#include <cmath>
#include <forward_list>
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
  vector<bool> numbers_vector(last_number - 1, true);
  size_t limit = static_cast<size_t>(sqrt(static_cast<double>(last_number)));
  for (size_t k = 2; k <= limit;) {
    for (size_t multiple = k * k; multiple <= last_number; multiple += k) {
      numbers_vector[multiple - 2] = false;
    }
    do {
      k += 1;
    } while (k <= limit && numbers_vector[k - 2] == false);
  }

  clock_t end = clock();
  ostringstream sstream;
  sstream << "Time: " << static_cast<double>(end - start) / CLOCKS_PER_SEC
          << endl;
  cout << sstream.str();

  for (size_t number = 2; number <= last_number; ++number) {
    if (numbers_vector[number - 2]) {
      cout << number << endl;
    };
  }

  return EXIT_SUCCESS;
}

size_t parseCmd(const int argc, char const *const *const argv) {
  char const *const program_name_ptr = argv[0];

  if (argc != 2) {
    help(program_name_ptr);
  }

  char const *const last_number_ptr = argv[1];

  size_t last_number = strtoul(last_number_ptr, nullptr, 10);
  if (errno == ERANGE) {
    std::cout << "range error, got " << last_number_ptr;
    help(program_name_ptr);
  }

  if (last_number < 2) {
    help(program_name_ptr);
  }

  return last_number;
}

void help(char const *const program_name, bool quit) {
  cerr << "This program calculates prime numbers up to N, N must be >= 2"
       << endl;
  cerr << "usage: " << program_name << " N" << endl;

  if (quit) {
    exit(EXIT_FAILURE);
  }
}