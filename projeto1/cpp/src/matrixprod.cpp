#include <papi.h>
#include <iomanip>
#include <iostream>

#include "matrix.hpp"

#ifndef NPAPI
#include "papi.hpp"
#endif

static void PrintMatrix(Matrix &matrix);
static void FillMatrixA(Matrix &matrix);
static void FillMatrixB(Matrix &matrix);

using namespace std;

int main(int argc, char *argv[]) {
#ifndef NPAPI
  Papi papi = Papi();

  papi.Init();
  papi.InstallEvents();
#endif

  int operation = 0;
  size_t a_rows = 10, a_columns = 10, b_rows = 10, b_columns = 10;
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
        cout << "1. Naive Multiplication Sequential -> ";
      } else if (operation == 2) {
        cout << "2. Line Multiplication Sequential -> ";
      } else if (operation == 3) {
        cout << "3. Naive Multiplication Parallel 1 thread -> ";
      } else if (operation == 4) {
        cout << "4. Naive Multiplication Parallel 2 threads -> ";
      } else if (operation == 5) {
        cout << "5. Naive Multiplication Parallel 3 threads -> ";
      } else if (operation == 6) {
        cout << "6. Naive Multiplication Parallel 4 threads -> ";
      } else if (operation == 7) {
        cout << "7. Line Multiplication Parallel 1 thread -> ";
      } else if (operation == 8) {
        cout << "8. Line Multiplication Parallel 2 threads -> ";
      } else if (operation == 9) {
        cout << "9. Line Multiplication Parallel 3 threads -> ";
      } else if (operation == 10) {
        cout << "10. Line Multiplication Parallel 4 threads -> ";
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
      cout << endl << "1. Naive Multiplication Sequential" << endl;
      cout << "2. Line Multiplication Sequential" << endl;
      cout << "3. Naive Multiplication Parallel 1 thread" << endl;
      cout << "4. Naive Multiplication Parallel 2 threads" << endl;
      cout << "5. Naive Multiplication Parallel 3 threads" << endl;
      cout << "6. Naive Multiplication Parallel 4 threads" << endl;
      cout << "7. Line Multiplication Parallel 1 thread" << endl;
      cout << "8. Line Multiplication Parallel 2 threads" << endl;
      cout << "9. Line Multiplication Parallel 3 threads" << endl;
      cout << "10. Line Multiplication Parallel 4 threads" << endl;

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

#ifndef NPAPI
    papi.Start();
#endif

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
        matrix_result =
            Matrix::MultiplicationLineSequential(matrix_a, matrix_b);
        break;
      case 3:
        matrix_result =
            Matrix::MultiplicationNaiveParallel(matrix_a, matrix_b, 1);
        break;
      case 4:
        matrix_result =
            Matrix::MultiplicationNaiveParallel(matrix_a, matrix_b, 2);
        break;
      case 5:
        matrix_result =
            Matrix::MultiplicationNaiveParallel(matrix_a, matrix_b, 3);
        break;
      case 6:
        matrix_result =
            Matrix::MultiplicationNaiveParallel(matrix_a, matrix_b, 4);
        break;
      case 7:
        matrix_result =
            Matrix::MultiplicationLineParallel(matrix_a, matrix_b, 1);
        break;
      case 8:
        matrix_result =
            Matrix::MultiplicationLineParallel(matrix_a, matrix_b, 2);
        break;
      case 9:
        matrix_result =
            Matrix::MultiplicationLineParallel(matrix_a, matrix_b, 3);
        break;
      case 10:
        matrix_result =
            Matrix::MultiplicationLineParallel(matrix_a, matrix_b, 4);
        break;
      default:
        cerr << "FAILED: wrong operation";
    }
    if (matrix_result != nullptr) {
      PrintMatrix(*matrix_result);
      delete matrix_result;
      matrix_result = nullptr;
    }

#ifndef NPAPI
    papi.StopAndReset();
#endif

  } while (inLoop);
}

static void FillMatrixA(Matrix &matrix) {
//  Matrix::Length length{matrix};
//  for (size_t row = 0; row < length.rows_length; row++) {
//    for (size_t column = 0; column < length.columns_length; column++) {
//      matrix(row, column) = 1.0;
//    }
//  }
  for (size_t row = 0; row < matrix.rows_length; row++) {
    for (size_t column = 0; column < matrix.columns_length; column++) {
      matrix(row, column) = 1.0;
    }
  }
}

static void FillMatrixB(Matrix &matrix) {
//  Matrix::Length length{matrix};
//  for (size_t row = 0; row < length.rows_length; row++) {
//    for (size_t column = 0; column < length.columns_length; column++) {
//      matrix(row, column) = static_cast<double>(row + 1);
//    }
//  }
  for (size_t row = 0; row < matrix.rows_length; row++) {
    for (size_t column = 0; column < matrix.columns_length; column++) {
      matrix(row, column) = static_cast<double>(row + 1);
    }
  }
}

static void PrintMatrix(Matrix &matrix) {
  cout << "Result matrix: " << endl;
//  for (size_t column = 0;
//       column < min(static_cast<size_t>(10), matrix.GetColumnsLength());
//       column++) {
//    cout << matrix(0, column) << " ";
//  }
  for (size_t column = 0;
       column < min(static_cast<size_t>(10), matrix.columns_length);
       column++) {
    cout << matrix(0, column) << " ";
  }
  cout << endl;
}
