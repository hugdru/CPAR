using System;

namespace matrixprod
{
    public class Matrix
    {
        public double[] values;
        public int rowsLength;
        public int columnsLength;

        /*public double this[int row, int column]
        {
            get { return values[row * columnsLength + column]; }
            set { values[row * columnsLength + column] = value; }
        }*/

        public Matrix(int rowsLength_, int columnsLength_)
        {
            if (rowsLength_ <= 0)
            {
                throw new System.ArgumentException("Failed: number of rows must not be <= zero");
            }

            if (columnsLength_ <= 0)
            {
                throw new System.ArgumentException("Failed: number of columns must not be <= zero");
            }

            rowsLength = rowsLength_;
            columnsLength = columnsLength_;

            values = new double[rowsLength * columnsLength];
        }

        public static Matrix MultiplicationNaiveSequential(ref Matrix matrixA, ref Matrix matrixB)
        {
            if (!MultiplicationSizesCheck(ref matrixA, ref matrixB))
            {
                throw new System.ArgumentException("matrices are not compatible for multiplication");
            }

            Matrix matrixResult = AllocateMultiplicationMatrix(matrixA, matrixB);

            var watch = System.Diagnostics.Stopwatch.StartNew();
            for (int rowA = 0; rowA < matrixA.rowsLength; rowA++)
            {
                for (int columnB = 0; columnB < matrixB.columnsLength; columnB++)
                {
                    double tempSum = 0;
                    for (int k = 0; k < matrixA.columnsLength; k++)
                    { 
                        tempSum += matrixA.values[rowA * matrixA.columnsLength + k] * matrixB.values[k * matrixB.columnsLength + columnB];
                        //tempSum += matrixA[rowA, k] * matrixB[k, columnB];
                    }
                    matrixResult.values[rowA * matrixResult.columnsLength + columnB] = tempSum;
                    //matrixResult[rowA, columnB] = tempSum;
                }
            }
            watch.Stop();
            Console.WriteLine("{0}", watch.Elapsed.TotalSeconds);

            return matrixResult;
        }

        public static Matrix MultiplicationLineSequential(ref Matrix matrixA, ref Matrix matrixB)
        {
            if (!MultiplicationSizesCheck(ref matrixA, ref matrixB))
            {
                throw new System.ArgumentException("matrices are not compatible for multiplication");
            }

            Matrix matrixResult = AllocateMultiplicationMatrix(matrixA, matrixB);

            var watch = System.Diagnostics.Stopwatch.StartNew();
            for (int rowA = 0; rowA < matrixA.rowsLength; rowA++)
            {
                for (int k = 0; k < matrixA.columnsLength; k++)
                {
                    for (int columnB = 0; columnB < matrixB.columnsLength; columnB++)
                    {
                        matrixResult.values[rowA * matrixResult.columnsLength + columnB] += 
                            matrixA.values[rowA * matrixA.columnsLength + k] * matrixB.values[k * matrixB.columnsLength + columnB];
                        //matrixResult[rowA, columnB] += matrixA[rowA, k] * matrixB[k, columnB];
                    }
                }
            }
            watch.Stop();
            Console.WriteLine("{0}", watch.Elapsed.TotalSeconds);

            return matrixResult;
        }

        private static bool MultiplicationSizesCheck(ref Matrix matrixA, ref Matrix matrixB)
        {
            return matrixA.columnsLength == matrixB.rowsLength;
        }

        private static Matrix AllocateMultiplicationMatrix(Matrix matrixA, Matrix matrixB)
        {
            return new Matrix(matrixA.rowsLength, matrixB.columnsLength);
        }
    }
}
