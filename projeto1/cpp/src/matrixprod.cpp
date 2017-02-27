#include <papi.h>
#include <iomanip>
#include <iostream>

#include "matrix.hpp"
#include "papi.hpp"

static void PrintMatrix(Matrix &matrix);
static void FillMatrixA(Matrix &matrix);
static void FillMatrixB(Matrix &matrix);

using namespace std;

int main(int argc, char *argv[]) {
  Papi papi = Papi();

  papi.Init();
  papi.InstallEvents();

  int operation = 0;
  size_t a_rows, a_columns;
  size_t b_rows, b_columns;
  bool inLoop = true;
  do {
    if (argc == 3) {
      istringstream ss1(argv[1]);
      istringstream ss2(argv[2]);
      size_t size;
      if (!(ss1 >> operation) || !(ss2 >> size)) {
        cerr << "the arguments must be integers " << endl;
        break;
      }
      if (operation == 1) {
        cout << "1. Multiplication Sequential -> ";
      } else if (operation == 2) {
        cout << "2. Line Multiplication Sequential -> ";
      } else if (operation == 3) {
        cout << "3. Line Multiplication Parallel -> ";
      } else if (operation == 4) {
        cout << "4. Line Multiplication Parallel -> ";
      } else {
        cerr << "Invalid operation." << endl;
        break;
      }

      if (size <= 1) {
        cerr << "Invalid size." << endl;
        break;
      }
      printf("size: %ld\n", size);
      a_rows = size;
      a_columns = size;
      b_rows = size;
      b_columns = size;
      inLoop = false;
    } else if (argc != 1) {
      cerr << "usage: " << argv[0] << " <operation> <size>" << endl;
      break;
    } else {
      cout << endl << "1. Multiplication Sequential" << endl;
      cout << "2. Multiplication Parallel" << endl;
      cout << "3. Line Multiplication Sequential" << endl;
      cout << "4. Line Multiplication Parallel" << endl;

      cout << "Selection?: ";
      cin >> operation;
      if (operation == 0) {
        break;
      }
      printf("Dimensions: matrix_a rows columns ? ");
      cin >> a_rows >> a_columns;

      printf("Dimensions: matrix_b rows columns ? ");
      cin >> b_rows >> b_columns;
    }

    papi.Start();

    Matrix matrix_a(a_rows, a_columns);
    FillMatrixA(matrix_a);

    Matrix matrix_b(b_rows, b_columns);
    FillMatrixB(matrix_b);

    Matrix *matrix_result = nullptr;
    switch (operation) {
      case 1:
        matrix_result =
            Matrix::MultiplicationNaiveSequential(matrix_a, matrix_b);
        break;
      case 2:
        matrix_result = Matrix::MultiplicationNaiveParallel(matrix_a, matrix_b);
        break;
      case 3:
        matrix_result =
            Matrix::MultiplicationLineSequential(matrix_a, matrix_b);
        break;
      case 4:
        matrix_result = Matrix::MultiplicationLineParallel(matrix_a, matrix_b);
        break;
      default:
        cerr << "FAILED: wrong operation";
    }
    if (matrix_result != nullptr) {
      PrintMatrix(*matrix_result);
      delete matrix_result;
      matrix_result = nullptr;
    }

    papi.StopAndReset();

  } while (inLoop);
}

static void FillMatrixA(Matrix &matrix) {
  Matrix::Length length{matrix};
  for (size_t row = 0; row < length.rows_length; row++) {
    for (size_t column = 0; column < length.columns_length; column++) {
      matrix(row, column) = 1.0;
    }
  }
}

static void FillMatrixB(Matrix &matrix) {
  Matrix::Length length{matrix};
  for (size_t row = 0; row < length.rows_length; row++) {
    for (size_t column = 0; column < length.columns_length; column++) {
      matrix(row, column) = static_cast<double>(row + 1);
    }
  }
}

static void PrintMatrix(Matrix &matrix) {
  cout << "Result matrix: " << endl;
  for (size_t column = 0;
       column < min(static_cast<size_t>(10), matrix.GetColumnsLength());
       column++) {
    cout << matrix(0, column) << " ";
  }
  cout << endl;
}
