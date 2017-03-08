#include "matrix.hpp"

#include <omp.h>
#include <iostream>

using namespace std;

Matrix::Matrix(initializer_list<initializer_list<double>> il_matrix) {
  rows_length = il_matrix.size();
  if (rows_length <= 0) {
    throw invalid_argument("number of rows must not be <= zero");
  }
  columns_length = (*il_matrix.begin()).size();
  if (columns_length <= 0) {
    throw invalid_argument("number of columns must not be <= zero");
  }
  values = new double[rows_length * columns_length];

  size_t index_row = 0;
  for (const auto &row : il_matrix) {
    if (columns_length != row.size()) {
      throw invalid_argument("number of columns must be the same for all rows");
    }
    size_t index_column = 0;
    for (const auto &value : row) {
      (*this)(index_row, index_column) = value;
      ++index_column;
    }
    ++index_row;
  }
}

Matrix::Matrix(size_t n_rows_, size_t n_columns_, bool zero) {
  if (n_rows_ <= 0) {
    throw invalid_argument("number of rows must be greater than zero");
  }
  if (n_columns_ <= 0) {
    throw invalid_argument("number of columns must be greater than zero");
  }

  rows_length = n_rows_;
  columns_length = n_columns_;

  if (zero) {
    values = new double[rows_length * columns_length]{};
  } else {
    values = new double[rows_length * columns_length];
  }
}

Matrix::~Matrix() { delete[] values; }

size_t Matrix::GetRowsLength() const { return rows_length; }

size_t Matrix::GetColumnsLength() const { return columns_length; }

Matrix *Matrix::MultiplicationNaiveSequential(Matrix &matrix_a,
                                              Matrix &matrix_b) {
  if (!MultiplicationSizesCheck(matrix_a, matrix_b)) {
    throw invalid_argument("matrices are not compatible for multiplication");
  }

  Matrix *matrix_result =
      AllocateMultiplicationMatrix(matrix_a, matrix_b, false);

  clock_t start = clock();
  for (size_t row_a = 0; row_a < matrix_a.rows_length; row_a++) {
    for (size_t column_b = 0; column_b < matrix_b.columns_length; column_b++) {
      double temp_sum = 0;
      for (size_t k = 0; k < matrix_a.columns_length; k++) {
        temp_sum += matrix_a(row_a, k) * matrix_b(k, column_b);
      }
      (*matrix_result)(row_a, column_b) = temp_sum;
    }
  }
  clock_t end = clock();
  char buffer[100];
  snprintf(buffer, 100, "%3.3f Time in seconds\n",
           static_cast<double>(end - start) / CLOCKS_PER_SEC);
  cout << buffer;

  return matrix_result;
}

Matrix *Matrix::MultiplicationNaiveParallel(Matrix &matrix_a, Matrix &matrix_b,
                                            int number_of_threads) {
  if (!MultiplicationSizesCheck(matrix_a, matrix_b)) {
    throw invalid_argument("matrices are not compatible for multiplication");
  }

  if (number_of_threads <= 0) {
    throw invalid_argument("Number of threads must be > 0");
  }

  Matrix *matrix_result =
      AllocateMultiplicationMatrix(matrix_a, matrix_b, false);

  double start = omp_get_wtime();
#pragma omp parallel for num_threads(number_of_threads)
  for (size_t row_a = 0; row_a < matrix_a.rows_length; row_a++) {
    for (size_t column_b = 0; column_b < matrix_b.columns_length; column_b++) {
      double temp_sum = 0.0;
      for (size_t k = 0; k < matrix_a.columns_length; k++) {
        temp_sum += matrix_a(row_a, k) * matrix_b(k, column_b);
      }
      (*matrix_result)(row_a, column_b) = temp_sum;
    }
  }
  double end = omp_get_wtime();
  char buffer[100];
  snprintf(buffer, 100, "%3.3f Time in seconds\n", end - start);
  cout << buffer;

  return matrix_result;
}

Matrix *Matrix::MultiplicationLineSequential(Matrix &matrix_a,
                                             Matrix &matrix_b) {
  if (!MultiplicationSizesCheck(matrix_a, matrix_b)) {
    throw invalid_argument("matrices are not compatible for multiplication");
  }

  Matrix *matrix_result = AllocateMultiplicationMatrix(matrix_a, matrix_b);

  clock_t start = clock();
  for (size_t row_a = 0; row_a < matrix_a.rows_length; row_a++) {
    for (size_t k = 0; k < matrix_a.columns_length; k++) {
      for (size_t column_b = 0; column_b < matrix_b.columns_length;
           column_b++) {
        (*matrix_result)(row_a, column_b) +=
            matrix_a(row_a, k) * matrix_b(k, column_b);
      }
    }
  }
  clock_t end = clock();
  char buffer[100];
  snprintf(buffer, 100, "%3.3f Time in seconds\n",
           static_cast<double>(end - start) / CLOCKS_PER_SEC);
  cout << buffer;

  return matrix_result;
}

Matrix *Matrix::MultiplicationLineParallel(Matrix &matrix_a, Matrix &matrix_b,
                                           int number_of_threads) {
  if (!MultiplicationSizesCheck(matrix_a, matrix_b)) {
    throw invalid_argument("matrices are not compatible for multiplication");
  }

  if (number_of_threads <= 0) {
    throw invalid_argument("Number of threads must be > 0");
  }

  Matrix *matrix_result = AllocateMultiplicationMatrix(matrix_a, matrix_b);

  omp_set_num_threads(number_of_threads);

  double start = omp_get_wtime();
#pragma omp parallel for
  for (size_t row_a = 0; row_a < matrix_a.rows_length; row_a++) {
#pragma omp parallel for
    for (size_t k = 0; k < matrix_a.columns_length; k++) {
      for (size_t column_b = 0; column_b < matrix_b.columns_length;
           column_b++) {
        (*matrix_result)(row_a, column_b) +=
            matrix_a(row_a, k) * matrix_b(k, column_b);
      }
    }
  }
  double end = omp_get_wtime();
  char buffer[100];
  snprintf(buffer, 100, "%3.3f Time in seconds\n", end - start);
  cout << buffer;

  return matrix_result;
}

bool Matrix::MultiplicationSizesCheck(Matrix &matrix_a, Matrix &matrix_b) {
  return matrix_a.columns_length == matrix_b.rows_length;
}

Matrix *Matrix::AllocateMultiplicationMatrix(Matrix &matrix_a, Matrix &matrix_b,
                                             bool zero) {
  return new Matrix{matrix_a.rows_length, matrix_b.columns_length, zero};
}

ostream &operator<<(ostream &os, Matrix const &matrix) {
//  Matrix::Length length(matrix);
//  for (size_t row = 0; row < length.rows_length; row++) {
//    for (size_t column = 0; column < length.columns_length; column++) {
//      os << matrix(row, column) << " ";
//    }
//    os << endl;
//  }
  for (size_t row = 0; row < matrix.rows_length; row++) {
    for (size_t column = 0; column < matrix.columns_length; column++) {
      os << matrix(row, column) << " ";
    }
    os << endl;
  }
  return os;
}
