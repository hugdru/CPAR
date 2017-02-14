import sys
from matrix import Matrix, multiplication_naive_sequential, multiplication_line_sequential


def main():
    multiplication_caller = {
        1: multiplication_naive_sequential,
        2: multiplication_line_sequential
    }

    while True:
        print("\n1. Multiplication Sequential")
        print("2. Line Multiplication Sequential")

        operation = int(input("Selection?: "))
        if operation == 0:
            break

        matrix_a_sizes = [int(x) for x in input("Dimensions: matrix_a rows columns ? ").split()]
        if len(matrix_a_sizes) != 2:
            raise ValueError("Failed: expecting rows and columns")

        matrix_b_sizes = [int(x) for x in input("Dimensions: matrix_b rows columns ? ").split()]
        if len(matrix_b_sizes) != 2:
            raise ValueError("Failed: expecting rows and columns")

        multiplication = multiplication_caller.get(operation)
        if multiplication is None:
            print("FAILED: wrong operation", file=sys.stderr)
            continue

        matrix_a = Matrix(*matrix_a_sizes)
        fill_matrix_a(matrix_a)

        matrix_b = Matrix(*matrix_b_sizes)
        fill_matrix_b(matrix_b)

        matrix_result = multiplication(matrix_a, matrix_b)
        print_matrix(matrix_result)


def fill_matrix_a(matrix):
    for index in range(matrix.rowsLength*matrix.columnsLength):
            matrix.values[index] = 1.0


def fill_matrix_b(matrix):
    for row in range(matrix.rowsLength):
        for column in range(matrix.columnsLength):
            matrix.values[row * matrix.columnsLength + column] = row + 1


def print_matrix(matrix):
    print("Result matrix: ")
    for column in range(min(10, matrix.columnsLength)):
        print(matrix.values[column], end=" ")
    print()


if __name__ == "__main__":
    main()

