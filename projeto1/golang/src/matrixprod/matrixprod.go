package main

import (
	"fmt"
	"os"
)

func main() {
	for {
		operation := 0
		fmt.Println("\n1. Multiplication Sequential")
		fmt.Println("2. Line Multiplication Sequential")

		fmt.Print("Selection?: ")
		_, err := fmt.Scan(&operation)
		if err != nil {
			panic("FAILED: Scan, expecting a valid operation")
		}

		if operation == 0 {
			break
		}

		var aRows, aColumns uint
		fmt.Print("Dimensions: matrix_a rows columns ? ")
		_, err = fmt.Scan(&aRows, &aColumns)
		if err != nil {
			panic("FAILED: Scan, expecting two uint rows and columns")
		}

		var bRows, bColumns uint
		fmt.Print("Dimensions: matrix_b rows columns ? ")
		_, err = fmt.Scan(&bRows, &bColumns)
		if err != nil {
			panic("FAILED: Scan, expecting two uint rows and columns")
		}

		matrixA := BuildMatrix(aRows, aColumns)
		fillMatrixA(matrixA)

		matrixB := BuildMatrix(bRows, bColumns)
		fillMatrixB(matrixB)

		var matrixResult *Matrix = nil
		switch operation {
		case 1:
			matrixResult = MultiplicationNaiveSequential(matrixA, matrixB)
		case 2:
			matrixResult = MultiplicationLineSequential(matrixA, matrixB)
		default:
			os.Stderr.WriteString("FAILED: wrong operation\n")
		}
		if matrixResult != nil {
			printMatrix(matrixResult)
		}
	}
}

func printMatrix(matrix *Matrix) {
	fmt.Println("Result matrix: ")
	for column := uint(0); column < min(10, matrix.ColumnsLength); column++ {
		//fmt.Print(matrix.Get(0, column), " ")
		fmt.Print(matrix.Values[column], " ")
	}
	fmt.Println()
}

func fillMatrixA(matrix *Matrix) {
	for row := uint(0); row < matrix.RowsLength; row++ {
		for column := uint(0); column < matrix.ColumnsLength; column++ {
			//*matrix.GetPtr(row, column) = 1.0
			matrix.Values[row * matrix.ColumnsLength + column] = 1.0
		}
	}
}
func fillMatrixB(matrix *Matrix) {
	for row := uint(0); row < matrix.RowsLength; row++ {
		for column := uint(0); column < matrix.ColumnsLength; column++ {
			//*matrix.GetPtr(row, column) = float64(row + 1)
			matrix.Values[row * matrix.ColumnsLength + column] = float64(row + 1)
		}
	}
}

func min(a, b uint) uint {
	if a < b {
		return a
	}
	return b
}
