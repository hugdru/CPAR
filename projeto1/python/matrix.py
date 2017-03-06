from array import array
import time


class Matrix:

    values = None
    rowsLength = 0
    columnsLength = 0

    def __init__(self, rows_length, columns_length):
        if rows_length <= 0:
            raise ValueError("number of rows must be greater than zero")

        if columns_length <= 0:
            raise ValueError("number of columns must be greater than zero")

        self.rowsLength = rows_length
        self.columnsLength = columns_length

        self.values = array("d", (0,) * (self.rowsLength * self.columnsLength))


def multiplication_naive_sequential(matrix_a, matrix_b):
    if not multiplication_sizes_check(matrix_a, matrix_b):
        raise ValueError("matrices are not compatible for multiplication")

    start = time.process_time()
    matrix_result = allocate_multiplication_matrix(matrix_a, matrix_b)
    for row_a in range(matrix_a.rowsLength):
        for column_b in range(matrix_b.columnsLength):
            temp_sum = 0.0
            for k in range(matrix_a.columnsLength):
                temp_sum +=\
                    matrix_a.values[row_a * matrix_a.columnsLength + k] *\
                    matrix_b.values[k * matrix_b.columnsLength + column_b]
            matrix_result.values[row_a * matrix_result.columnsLength + column_b] = temp_sum
    end = time.process_time()
    print("{:f} Time in seconds".format(end-start))

    return matrix_result


def multiplication_line_sequential(matrix_a, matrix_b):
    if not multiplication_sizes_check(matrix_a, matrix_b):
        raise ValueError("matrices are not compatible for multiplication")

    start = time.process_time()
    matrix_result = allocate_multiplication_matrix(matrix_a, matrix_b)
    for row_a in range(matrix_a.rowsLength):
        for k in range(matrix_a.columnsLength):
            for column_b in range(matrix_b.columnsLength):
                matrix_result.values[row_a * matrix_result.columnsLength + column_b] +=\
                    matrix_a.values[row_a * matrix_a.columnsLength + k] *\
                    matrix_b.values[k * matrix_b.columnsLength + column_b]
    end = time.process_time()
    print("{:f} Time in seconds".format(end-start))

    return matrix_result


def multiplication_sizes_check(matrix_a, matrix_b):
    return matrix_a.columnsLength == matrix_b.rowsLength


def allocate_multiplication_matrix(matrix_a, matrix_b):
    return Matrix(matrix_a.rowsLength, matrix_b.columnsLength)
