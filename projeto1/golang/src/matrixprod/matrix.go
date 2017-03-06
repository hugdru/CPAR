package main

import (
	"fmt"
	"time"
)

type Matrix struct {
	Values        []float64
	RowsLength    int
	ColumnsLength int
}

func BuildMatrix(rowsLength, columnsLength int) *Matrix {
	if rowsLength <= 0 {
		panic("FAILED: Matrix, number of rows must be greater than zero")
	}

	if columnsLength <= 0 {
		panic("FAILED: Matrix, number of columns must be greater than zero")
	}

	return &Matrix{
		Values:        make([]float64, rowsLength*columnsLength),
		RowsLength:    rowsLength,
		ColumnsLength: columnsLength,
	}
}

func (matrix *Matrix) Get(row, column int) float64 {
	return matrix.Values[row*matrix.ColumnsLength+column]
}

func (matrix *Matrix) GetPtr(row, column int) *float64 {
	return &matrix.Values[row*matrix.ColumnsLength+column]
}

func MultiplicationNaiveSequential(matrixA, matrixB *Matrix) *Matrix {

	if !multiplicationSizesCheck(matrixA, matrixB) {
		panic("matrices are not compatible for multiplication")
	}

	matrixResult := allocateMultiplicationMatrix(matrixA, matrixB)

	start := time.Now()
	for rowA := 0; rowA < matrixA.RowsLength; rowA++ {
		for columnB := 0; columnB < matrixB.ColumnsLength; columnB++ {
			var tempSum float64 = 0.0
			for k := 0; k < matrixA.ColumnsLength; k++ {
				//tempSum += matrixA.Get(rowA, k) * matrixB.Get(k, columnB)
				tempSum +=
					matrixA.Values[rowA*matrixA.ColumnsLength+k] *
						matrixB.Values[k*matrixB.ColumnsLength+columnB]
			}
			//*matrixResult.GetPtr(rowA, columnB) = tempSum
			matrixResult.Values[rowA*matrixResult.ColumnsLength+columnB] = tempSum
		}
	}
	elapsed := time.Since(start)
	fmt.Printf("%3.3f Time in seconds\n", elapsed.Seconds())

	return matrixResult
}

func MultiplicationLineSequential(matrixA, matrixB *Matrix) *Matrix {

	if !multiplicationSizesCheck(matrixA, matrixB) {
		panic("matrices are not compatible for multiplication")
	}

	matrixResult := allocateMultiplicationMatrix(matrixA, matrixB)

	start := time.Now()
	for rowA := 0; rowA < matrixA.RowsLength; rowA++ {
		for k := 0; k < matrixA.ColumnsLength; k++ {
			for columnB := 0; columnB < matrixB.ColumnsLength; columnB++ {
				//*matrixResult.GetPtr(rowA, columnB) += matrixA.Get(rowA, k) * matrixB.Get(k, columnB)
				matrixResult.Values[rowA*matrixResult.ColumnsLength+columnB] +=
					matrixA.Values[rowA*matrixA.ColumnsLength+k] *
						matrixB.Values[k*matrixB.ColumnsLength+columnB]
			}
		}
	}
	elapsed := time.Since(start)
	fmt.Printf("%3.3f Time in seconds\n", elapsed.Seconds())

	return matrixResult
}

func multiplicationSizesCheck(matrixA, matrixB *Matrix) bool {
	return matrixA.ColumnsLength == matrixB.RowsLength
}

func allocateMultiplicationMatrix(matrixA, matrixB *Matrix) *Matrix {
	return BuildMatrix(matrixA.RowsLength, matrixB.ColumnsLength)
}
