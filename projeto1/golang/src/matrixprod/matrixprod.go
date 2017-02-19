package main

import (
	"fmt"
	"os"
    "strconv"
)

func main() {
	args := os.Args[1:]
	inLoop := true;
	var aRows, aColumns int
	var bRows, bColumns int
	for inLoop == true {
		operation := 0
		if len(args) == 2 {
			var err error
			size := 10;
    		if operation, err = strconv.Atoi(args[0]); err != nil {
        		panic(err)
    			break
    		}
			if size, err = strconv.Atoi(args[1]); err != nil {
        		panic(err)
        		break
    		}

    		if operation == 1 {
				fmt.Print("1. Multiplication Sequential -> ")
			}else if operation == 2 {
				fmt.Print("2. Line Multiplication Sequential -> ")
			}else{
				fmt.Println("Invalid operation.")
				break
			}
			if size <= 1 {
				fmt.Println("Invalid size.")
				break
			}
			fmt.Println("size:", size)
    		aRows = size
    		aColumns = size
    		bRows = size
    		bColumns = size 
    		inLoop = false
		}else if len(args) != 0{
			fmt.Print("Usage:" , os.Args[0], "<operation> <size>")
			break
		}else{

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
			
			fmt.Print("Dimensions: matrix_a rows columns ? ")
			_, err = fmt.Scan(&aRows, &aColumns)
			if err != nil {
				panic("FAILED: Scan, expecting two uint rows and columns")
			}

			fmt.Print("Dimensions: matrix_b rows columns ? ")
			_, err = fmt.Scan(&bRows, &bColumns)
			if err != nil {
				panic("FAILED: Scan, expecting two uint rows and columns")
			}
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
	for column := 0; column < min(10, matrix.ColumnsLength); column++ {
		//fmt.Print(matrix.Get(0, column), " ")
		fmt.Print(matrix.Values[column], " ")
	}
	fmt.Println()
}

func fillMatrixA(matrix *Matrix) {
	for row := 0; row < matrix.RowsLength; row++ {
		for column := 0; column < matrix.ColumnsLength; column++ {
			//*matrix.GetPtr(row, column) = 1.0
			matrix.Values[row * matrix.ColumnsLength + column] = 1.0
		}
	}
}
func fillMatrixB(matrix *Matrix) {
	for row := 0; row < matrix.RowsLength; row++ {
		for column := 0; column < matrix.ColumnsLength; column++ {
			//*matrix.GetPtr(row, column) = float64(row + 1)
			matrix.Values[row * matrix.ColumnsLength + column] = float64(row + 1)
		}
	}
}

func min(a, b int) int {
	if a < b {
		return a
	}
	return b
}
