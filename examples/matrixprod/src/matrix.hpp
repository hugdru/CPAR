#ifndef MATRIXPROD_MATRIX_HPP_
#define MATRIXPROD_MATRIX_HPP_

#include <cstddef>
#include <initializer_list>
#include <ostream>
#include <utility>

class Matrix {
 private:
  double *values;
  size_t rows_length;
  size_t columns_length;

 public:
  typedef struct Length {
    size_t rows_length;
    size_t columns_length;

    explicit Length(Matrix const &matrix)
        : rows_length{matrix.rows_length},
          columns_length{matrix.columns_length} {}
  } Length;

  inline double &operator()(size_t row, size_t column) {
    return values[row * columns_length + column];
  }

  inline double operator()(size_t row, size_t column) const {
    return values[row * columns_length + column];
  }

  explicit Matrix(
      std::initializer_list<std::initializer_list<double>> il_matrix);
  explicit Matrix(size_t n_rows_, size_t n_columns_, bool zero = true);
  virtual ~Matrix();

  size_t GetRowsLength() const;
  size_t GetColumnsLength() const;

  static Matrix *MultiplicationNaiveSerial(Matrix &matrix_a, Matrix &matrix_b);
  static Matrix *MultiplicationLineSerial(Matrix &matrix_a, Matrix &matrix_b);

 private:
  static bool MultiplicationSizesCheck(Matrix &matrix_a, Matrix &matrix_b);
  static Matrix *AllocateMultiplicationMatrix(Matrix &matrix_a,
                                              Matrix &matrix_b,
                                              bool zero = true);
};

std::ostream &operator<<(std::ostream &os, Matrix const &matrix);

#endif  // MATRIXPROD_MATRIX_HPP_
