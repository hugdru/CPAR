#include <papi.h>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "matrix.hpp"

static void PrintMatrix(Matrix &matrix);
static void FillMatrixA(Matrix &matrix);
static void FillMatrixB(Matrix &matrix);

using namespace std;

int main(int argc, char *argv[]) {
  int ret = PAPI_library_init(static_cast<int>(PAPI_VER_CURRENT));
  if (ret != static_cast<int>(PAPI_VER_CURRENT)) {
    cerr << "FAILED: PAPI_library_init" << endl;
  }

  int EventSet = PAPI_NULL;

  ret = PAPI_create_eventset(&EventSet);
  if (ret != PAPI_OK) {
    cerr << "FAILED: PAPI_create_eventset" << endl;
  }

  ret = PAPI_add_event(EventSet, PAPI_L1_DCM);
  if (ret != PAPI_OK) {
    cerr << "FAILED: PAPI_add_event PAPI_L1_DCM" << endl;
  }

  ret = PAPI_add_event(EventSet, PAPI_L2_DCM);
  if (ret != PAPI_OK) {
    cerr << "FAILED: PAPI_add_event PAPI_L2_DCM" << endl;
  }

  int operation = 0;
  size_t a_rows, a_columns;
  size_t b_rows, b_columns;
  bool inLoop = true;
  do {
  	if(argc == 3){
  		istringstream ss1(argv[1]);
  		istringstream ss2(argv[2]);
  		size_t size;
  		if (!(ss1 >> operation) || !(ss2 >> size)){
    		cerr << "the arguments must be integers " << endl;
    		break;
  		}
    if(operation == 1){
			cout << "1. Multiplication Sequential -> ";
		}else if (operation == 2){
			cout << "2. Line Multiplication Sequential -> ";
		}else if (operation == 3){
      cout << "3. Line Multiplication Parallel -> ";
    }else if (operation == 4){
      cout << "4. Line Multiplication Parallel -> ";
    }else{
			cerr << "Invalid operation." << endl;
			break;
		}

		if(size <= 1){
			cerr << "Invalid size." << endl;
			break;
		}
		printf("size: %ld\n", size);
    a_rows = size;
    a_columns = size;
    b_rows = size;
    b_columns = size;
		inLoop = false;
    }else if (argc != 1){
    	cerr << "usage: " << argv[0] << " <operation> <size>"<< endl;
    	break;
    }else{
	    cout << endl << "1. Multiplication Sequential" << endl;
	    cout << "2. Multiplication Parallel" << endl;
	    cout << "3. Line Multiplication Sequential" << endl;
	    cout << "4. Line Multiplication Parallel" << endl;

	    cout << "Selection?: ";
	    cin >> operation;
	    if (operation == 0) {
	      break;
	    }
	    printf("Dimensions: matrix_a rows columns ? ");
	    cin >> a_rows >> a_columns;

	    printf("Dimensions: matrix_b rows columns ? ");
	    cin >> b_rows >> b_columns;
	}
    // Start counting
    ret = PAPI_start(EventSet);
    if (ret != PAPI_OK) {
      cerr << "FAILED: PAPI_start" << endl;
    }

    Matrix matrix_a(a_rows, a_columns);
    FillMatrixA(matrix_a);

    Matrix matrix_b(b_rows, b_columns);
    FillMatrixB(matrix_b);

    Matrix *matrix_result = nullptr;
    switch (operation) {
      case 1:
        matrix_result = Matrix::MultiplicationNaiveSequential(matrix_a, matrix_b);
        break;
      case 2:
        matrix_result = Matrix::MultiplicationNaiveParallel(matrix_a, matrix_b);
        break;
      case 3:
        matrix_result = Matrix::MultiplicationLineSequential(matrix_a, matrix_b);
        break;
      case 4:
        matrix_result = Matrix::MultiplicationLineParallel(matrix_a, matrix_b);
        break;
      default:
        cerr << "FAILED: wrong operation";
    }
    if (matrix_result != nullptr) {
      PrintMatrix(*matrix_result);
      delete matrix_result;
      matrix_result = nullptr;
    }

    long long values[2];
    ret = PAPI_stop(EventSet, values);
    if (ret != PAPI_OK) {
      cerr << "FAILED: PAPI_stop" << endl;
    } else {
      printf("L1 DCM: %lld \n", values[0]);
      printf("L2 DCM: %lld \n", values[1]);
    }

    ret = PAPI_reset(EventSet);
    if (ret != PAPI_OK) {
      cerr << "FAILED: PAPI_reset" << endl;
    }
  } while (inLoop);

  ret = PAPI_remove_event(EventSet, PAPI_L1_DCM);
  if (ret != PAPI_OK) {
    cerr << "FAILED: PAPI_remove_event" << endl;
  }

  ret = PAPI_remove_event(EventSet, PAPI_L2_DCM);
  if (ret != PAPI_OK) {
    cerr << "FAILED: PAPI_remove_event" << endl;
  }

  ret = PAPI_destroy_eventset(&EventSet);
  if (ret != PAPI_OK) {
    cerr << "FAILED: PAPI_destroy_eventset" << endl;
  }
}

static void FillMatrixA(Matrix &matrix) {
  Matrix::Length length{matrix};
  for (size_t row = 0; row < length.rows_length; row++) {
    for (size_t column = 0; column < length.columns_length; column++) {
      matrix(row, column) = 1.0;
    }
  }
}

static void FillMatrixB(Matrix &matrix) {
  Matrix::Length length{matrix};
  for (size_t row = 0; row < length.rows_length; row++) {
    for (size_t column = 0; column < length.columns_length; column++) {
      matrix(row, column) = static_cast<double>(row + 1);
    }
  }
}

static void PrintMatrix(Matrix &matrix) {
  cout << "Result matrix: " << endl;
  for (size_t column = 0;
       column < min(static_cast<size_t>(10), matrix.GetColumnsLength());
       column++) {
    cout << matrix(0, column) << " ";
  }
  cout << endl;
}
