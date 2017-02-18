package matrixprod;

import java.util.Scanner;

public class Matrixprod {
  private static Scanner scanner = new Scanner(System.in);

  public static void main(String[] args) {
    int operation = 0;
    do {
      System.out.println();
      System.out.println("1. Multiplication Sequential");
      System.out.println("2. Line Multiplication Sequential");

      System.out.println("Selection?: ");
      operation = scanner.nextInt();

      if (operation == 0)
        break;

      System.out.println("..:: Dimensions ::..");
      System.out.print("matrix_a rows: ");
      int a_rows = scanner.nextInt();

      System.out.print("matrix_a columns: ");
      int a_columns = scanner.nextInt();

      System.out.print("matrix_b rows: ");
      int b_rows = scanner.nextInt();

      System.out.print("matrix_b columns: ");
      int b_columns = scanner.nextInt();

      Matrix matrix_result = null;
      try {
        Matrix matrix_a = new Matrix(a_rows, a_columns);
        FillMatrixA(matrix_a);

        Matrix matrix_b = new Matrix(b_rows, b_columns);
        FillMatrixB(matrix_b);

        switch (operation) {
        case 1:
          matrix_result = Matrix.MultiplicationNaiveSequential(matrix_a, matrix_b);
          break;
        case 2:
          matrix_result = Matrix.MultiplicationLineSequential(matrix_a, matrix_b);
          break;
        default:
          System.err.println("FAILED: wrong operation");
        }
      } catch (Exception e) {
        System.err.println(e.getMessage());
      }

      if (matrix_result != null) {
        PrintMatrix(matrix_result);
        matrix_result = null;
      }

    } while (operation != 0);
  }

  private static void FillMatrixA(Matrix matrix) {
    for (int row = 0; row < matrix.getRowsLength(); row++)
      for (int column = 0; column < matrix.getColumnsLength(); column++)
        matrix.getValues()[row * matrix.getColumnsLength() + column] = 1.0;
  }

  private static void FillMatrixB(Matrix matrix) {
    for (int row = 0; row < matrix.getRowsLength(); row++)
      for (int column = 0; column < matrix.getColumnsLength(); column++)
        matrix.getValues()[row * matrix.getColumnsLength() + column] = (double) (row + 1.0);
  }

  private static void PrintMatrix(Matrix matrix) {
    System.out.println("Result matrix: ");
    for (int column = 0; column < Math.min(10, matrix.getColumnsLength()); column++)
      System.out.print(matrix.getValues()[0 * matrix.getColumnsLength() + column] + " ");
    System.out.println();
  }
}
