package matrixprod;

import java.util.Scanner;

public class Matrixprod {
  private static Scanner scanner = new Scanner(System.in);

  public static void main(String[] args) {
    int operation = 0, a_rows = 10, a_columns = 10, b_rows = 10, b_columns = 10;
    boolean inLoop = true;
    do {
      if (args.length == 2) {
        try {
          operation = Integer.parseInt(args[0]);
          if(operation == 1){
            System.out.print("1. Multiplication Sequential -> ");
          }else if (operation == 2){
            System.out.print("2. Line Multiplication Sequential -> ");
          }else{
            System.err.println("Invalid operation.");
            break;
          }
          a_rows = a_columns = b_rows = b_columns = Integer.parseInt(args[1]);
          if(a_rows <= 1){
            System.err.println("Invalid size.");
            break;
          }
          System.out.println("size: " + a_rows);
          inLoop = false;
        } catch (NumberFormatException ex) {
          System.err.println("all args must be integers.");
          break;
        }
      } else if (args.length != 0) {
        System.err.println("usage: <name>.jar <option> <size>");
        break;
      } else {
        System.out.println("\n1. Multiplication Sequential");
        System.out.println("2. Line Multiplication Sequential");

        System.out.print("Selection?: ");
        operation = scanner.nextInt();
        scanner.nextLine();

        if (operation == 0)
          break;

        try {
          System.out.print("Dimensions: matrix_a rows columns ? ");
          String[] dimensions = scanner.nextLine().split(" ");
          if (dimensions.length != 2) {
            throw new Exception("Failed: expecting rows and columns");
          }
          a_rows = Integer.parseInt(dimensions[0]);
          a_columns = Integer.parseInt(dimensions[1]);

          System.out.print("Dimensions: matrix_b rows columns ? ");
          dimensions = scanner.nextLine().split(" ");
          if (dimensions.length != 2) {
            throw new Exception("Failed: expecting rows and columns");
          }
          b_rows = Integer.parseInt(dimensions[0]);
          b_columns = Integer.parseInt(dimensions[1]);
        } catch (Exception e) {
          System.err.println("all args must be integers.");
          break;
        }
      }
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

    } while (inLoop);
  }

  private static void FillMatrixA(Matrix matrix) {
    for (int row = 0; row < matrix.rows_length; row++)
      for (int column = 0; column < matrix.columns_length; column++)
        matrix.values[row * matrix.columns_length + column] = 1.0;
  }

  private static void FillMatrixB(Matrix matrix) {
    for (int row = 0; row < matrix.rows_length; row++)
      for (int column = 0; column < matrix.columns_length; column++)
        matrix.values[row * matrix.columns_length + column] = (double) (row + 1.0);
  }

  private static void PrintMatrix(Matrix matrix) {
    System.out.println("Result matrix: ");
    for (int column = 0; column < Math.min(10, matrix.columns_length); column++)
      System.out.print(matrix.values[column] + " ");
    System.out.println();
  }
}
