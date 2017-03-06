using System;

namespace matrixprod
{
    class MainClass
    {
        public static void Main(string[] args)
        {
            int operation = 0;
            int a_rows = 10, a_columns = 10, b_rows = 10, b_columns = 10;
            bool inLoop = true;
            while (inLoop)
            {
                if (args.Length == 2)
                {
                    operation = int.Parse(args[0]);
                    int size = int.Parse(args[1]);

                    if (operation == 1)
                    {
                        Console.Write("1. Multiplication Sequential -> ");
                    }
                    else if (operation == 2)
                    {
                        Console.Write("2. Line Multiplication Sequential -> ");
                    }
                    else
                    {
                        Console.WriteLine("Invalid operation.");
                        break;
                    }

                    if (size <= 1)
                    {
                        Console.WriteLine("Invalid size.");
                        break;
                    }
                    Console.WriteLine("size: " + size);
                    a_rows = size;
                    a_columns = size;
                    b_rows = size;
                    b_columns = size;
                    inLoop = false;
                }
                else if (args.Length != 0)
                {
                    Console.WriteLine("usage: <name> <option> <size>");
                    break;
                }
                else
                {
                    Console.WriteLine("\n1. Multiplication Sequential");
                    Console.WriteLine("2. Line Multiplication Sequential");

                    Console.Write("Selection?: ");
                    operation = int.Parse(Console.ReadLine());
                    if (operation == 0)
                    {
                        break;
                    }
                    Console.Write("Dimensions: matrixA rows columns ? ");
                    string[] sizes = Console.ReadLine().Split();
                    if (sizes.Length != 2)
                    {
                        throw new System.ArgumentException("Failed: expecting rows and columns");
                    }
                    a_rows = int.Parse(sizes[0]);
                    a_columns = int.Parse(sizes[1]);

                    Console.Write("Dimensions: matrixB rows columns ? ");
                    sizes = Console.ReadLine().Split();
                    if (sizes.Length != 2)
                    {
                        throw new System.ArgumentException("Failed: expecting rows and columns");
                    }
                    b_rows = int.Parse(sizes[0]);
                    b_columns = int.Parse(sizes[1]);
                }

                Matrix matrixA = new Matrix(a_rows, a_columns);
                FillMatrixA(matrixA);

                Matrix matrixB = new Matrix(b_rows, b_columns);
                FillMatrixB(matrixB);

                Matrix matrixResult = null;
                switch (operation)
                {
                    case 1:
                        matrixResult = Matrix.MultiplicationNaiveSequential(matrixA, matrixB);
                        break;
                    case 2:
                        matrixResult = Matrix.MultiplicationLineSequential(matrixA, matrixB);
                        break;
                    default:
                        Console.Error.Write("FAILED: wrong operation");
                        break;
                }
                if (matrixResult != null)
                {
                    PrintMatrix(matrixResult);
                    matrixResult = null;
                }
            }
        }

        private static void FillMatrixA(Matrix matrix)
        {
            for (int row = 0; row < matrix.rowsLength; row++)
            {
                for (int column = 0; column < matrix.columnsLength; column++)
                {
                    matrix.values[row * matrix.columnsLength + column] = 1.0;
                    //matrix[row, column] = 1.0;
                }
            }
        }

        private static void FillMatrixB(Matrix matrix)
        {
            for (int row = 0; row < matrix.rowsLength; row++)
            {
                for (int column = 0; column < matrix.columnsLength; column++)
                {
                    matrix.values[row * matrix.columnsLength + column] = Convert.ToDouble(row + 1);
                    //matrix[row, column] = Convert.ToDouble(row + 1);
                }
            }
        }

        private static void PrintMatrix(Matrix matrix)
        {
            Console.WriteLine("Result matrix: ");
            for (int column = 0; column < Math.Min(10, matrix.columnsLength); column++)
            {
                Console.Write("{0} ", matrix.values[column].ToString());
                //Console.Write("{0} ", matrix[0, column].ToString());
            }
            Console.WriteLine();
        }
    }
}
