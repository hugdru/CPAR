package main

import (
	"fmt"
	"time"
)

type Matrix struct {
	values        []float64
	RowsLength    uint
	ColumnsLength uint
}

func BuildMatrix(rowsLength, columnsLength uint) *Matrix {
	if rowsLength <= 0 {
		panic("FAILED: Matrix, number of rows must be greater than zero")
	}

	if columnsLength <= 0 {
		panic("FAILED: Matrix, number of columns must be greater than zero")
	}

	return &Matrix{
		values:        make([]float64, rowsLength*columnsLength),
		RowsLength:    rowsLength,
		ColumnsLength: columnsLength,
	}
}

func (matrix *Matrix) Get(row, column uint) float64 {
	return matrix.values[row*matrix.ColumnsLength+column]
}

func (matrix *Matrix) GetPtr(row, column uint) *float64 {
	return &matrix.values[row*matrix.ColumnsLength+column]
}

func MultiplicationNaiveSequential(matrixA, matrixB *Matrix) *Matrix {

	if !multiplicationSizesCheck(matrixA, matrixB) {
		panic("matrices are not compatible for multiplication")
	}

	matrixResult := allocateMultiplicationMatrix(matrixA, matrixB)

	start := time.Now()
	for rowA := uint(0); rowA < matrixA.RowsLength; rowA++ {
		for columnB := uint(0); columnB < matrixB.ColumnsLength; columnB++ {
			var tempSum float64 = 0.0
			for k := uint(0); k < matrixA.ColumnsLength; k++ {
				//tempSum += matrixA.Get(rowA, k) * matrixB.Get(k, columnB)
				tempSum +=
					matrixA.values[rowA*matrixA.ColumnsLength+k] *
						matrixB.values[k*matrixB.ColumnsLength+columnB]
			}
			//*matrixResult.GetPtr(rowA, columnB) = tempSum
			matrixResult.values[rowA*matrixResult.ColumnsLength+columnB] = tempSum
		}
	}
	elapsed := time.Since(start)
	fmt.Printf("Time: %3.3f\n", elapsed.Seconds())

	return matrixResult
}

func MultiplicationLineSequential(matrixA, matrixB *Matrix) *Matrix {

	if !multiplicationSizesCheck(matrixA, matrixB) {
		panic("matrices are not compatible for multiplication")
	}

	matrixResult := allocateMultiplicationMatrix(matrixA, matrixB)

	start := time.Now()
	for rowA := uint(0); rowA < matrixA.RowsLength; rowA++ {
		for k := uint(0); k < matrixA.ColumnsLength; k++ {
			for columnB := uint(0); columnB < matrixB.ColumnsLength; columnB++ {
				//*matrixResult.GetPtr(rowA, columnB) += matrixA.Get(rowA, k) * matrixB.Get(k, columnB)
				matrixResult.values[rowA*matrixResult.ColumnsLength+columnB] +=
					matrixA.values[rowA*matrixA.ColumnsLength+k] *
						matrixB.values[k*matrixB.ColumnsLength+columnB]
			}
		}
	}
	elapsed := time.Since(start)
	fmt.Printf("Time: %3.3f\n", elapsed.Seconds())

	return matrixResult
}

func multiplicationSizesCheck(matrixA, matrixB *Matrix) bool {
	return matrixA.ColumnsLength == matrixB.RowsLength
}

func allocateMultiplicationMatrix(matrixA, matrixB *Matrix) *Matrix {
	return BuildMatrix(matrixA.RowsLength, matrixB.ColumnsLength)
}
