using System;

namespace matrixprod
{
    class MainClass
    {
        private struct InputMatrixSize
        {
            public int rowsLength;
            public int columnsLength;

            public InputMatrixSize(int rowsLength_, int columnsLength_)
            {
                rowsLength = rowsLength_;
                columnsLength = columnsLength_;
            }
        }

        public static void Main(string[] args)
        {            
            int operation = 0;
            int size = 10;
            var matrixASizes = new InputMatrixSize(size, size);
            var matrixBSizes = new InputMatrixSize(size, size);
            bool inLoop = true;
            while (inLoop)
            {
                if (args.Length == 2)
                {
                    Console.WriteLine(args[0] + " " + args[1] );
                    operation = int.Parse(args[0]);
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
                    size = int.Parse(args[1]);
                    matrixASizes.rowsLength = matrixASizes.columnsLength = matrixBSizes.rowsLength = matrixBSizes.columnsLength = size;
                    if (matrixASizes.rowsLength <= 1)
                    {
                        Console.WriteLine("Invalid dize.");
                        break;
                    }
                    Console.WriteLine("size: " + matrixASizes.rowsLength);
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
                    matrixASizes = ReadMatrixSizes();

                    Console.Write("Dimensions: matrixB rows columns ? ");
                    matrixBSizes = ReadMatrixSizes();

                }
                Matrix matrixA = new Matrix(matrixASizes.rowsLength, matrixASizes.columnsLength);
                FillMatrixA(ref matrixA);

                Matrix matrixB = new Matrix(matrixBSizes.rowsLength, matrixBSizes.columnsLength);
                FillMatrixB(ref matrixB);

                Matrix matrixResult = null;
                switch (operation)
                {
                    case 1:
                        matrixResult = Matrix.MultiplicationNaiveSequential(ref matrixA, ref matrixB);
                        break;
                    case 2:
                        matrixResult = Matrix.MultiplicationLineSequential(ref matrixA, ref matrixB);
                        break;
                    default:
                        Console.Error.Write("FAILED: wrong operation");
                        break;
                }
                if (matrixResult != null)
                {
                    PrintMatrix(ref matrixResult);
                    matrixResult = null;
                }
            }
        }

        private static InputMatrixSize ReadMatrixSizes()
        {
            string[] sizes = Console.ReadLine().Split();
            if (sizes.Length != 2)
            {
                throw new System.ArgumentException("Failed: expecting rows and columns");
            }
            var rows = int.Parse(sizes[0]);
            var columns = int.Parse(sizes[1]);

            return new InputMatrixSize(rows, columns);
        }

        private static void FillMatrixA(ref Matrix matrix)
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

        private static void FillMatrixB(ref Matrix matrix)
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

        private static void PrintMatrix(ref Matrix matrix)
        {
            Console.WriteLine("Result matrix: ");
            for (int column = 0; column < Math.Min(10, matrix.columnsLength); column++)
            {
                Console.Write("{0}", matrix.values[column].ToString());
                //Console.Write("{0}", matrix[0, column].ToString());
            }
            Console.WriteLine();
        }
    }
}
