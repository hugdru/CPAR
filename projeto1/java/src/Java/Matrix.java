package Java;

import java.util.List;

public class Matrix {
	private double[] values;
	private int rows_length;
	private int columns_length;

	public Matrix(List<List<Double>> il_matrix) throws Exception {
		rows_length = il_matrix.size();
		if (rows_length <= 0)
			throw new Exception("number of rows must not be <= zero");

		columns_length = il_matrix.size();
		if (columns_length <= 0)
			throw new Exception("number of columns must not be <= zero");

		values = new double[rows_length * columns_length];

		int index_row = 0;
		for (List<Double> row : il_matrix) {
			if (columns_length != row.size())
				throw new Exception("number of columns must be the same for all rows");

			int index_column = 0;
			for (Double value : row) {
				values[index_row + index_column * rows_length] = value;
				++index_column;
			}
			++index_row;
		}
	}

	public Matrix(int n_rows_, int n_columns_) throws Exception {
		if (n_rows_ <= 0)
			throw new Exception("number of rows must be greater than zero");
		if (n_columns_ <= 0)
			throw new Exception("number of columns must be greater than zero");

		rows_length = n_rows_;
		columns_length = n_columns_;
		values = new double[rows_length * columns_length];
	}

	public double[] getValues() {
		return values;
	}

	public int getRowsLength() {
		return rows_length;
	}

	public int getColumnsLength() {
		return columns_length;
	}

	public static Matrix MultiplicationNaiveSequential(Matrix matrix_a, Matrix matrix_b) throws Exception {
		if (!MultiplicationSizesCheck(matrix_a, matrix_b))
			throw new Exception("matrices are not compatible for multiplication");

		int mat_a_rows = matrix_a.getRowsLength();
		int mat_a_col = matrix_a.getColumnsLength();
		double[] mat_a_val = matrix_a.getValues();

		int mat_b_col = matrix_b.getColumnsLength();
		double[] mat_b_val = matrix_b.getValues();

		Matrix matrix_result = new Matrix(mat_a_rows, mat_b_col);
		int mat_res_col = matrix_result.getColumnsLength();
		double[] mat_res_val = matrix_result.getValues();

		long start = System.currentTimeMillis();
		for (int row_a = 0; row_a < mat_a_rows; row_a++) {
			for (int column_b = 0; column_b < mat_b_col; column_b++) {
				double temp_sum = 0;
				for (int k = 0; k < mat_a_col; k++)
					temp_sum += mat_a_val[row_a * mat_a_col + k] * mat_b_val[k * mat_b_col + column_b];
				mat_res_val[row_a + mat_res_col * column_b] = temp_sum;
			}
		}
		long end = System.currentTimeMillis();
		System.out.printf("Time: %3.3f seconds\n", (end - start) / 1000.0f);
		return matrix_result;
	}

	public static Matrix MultiplicationLineSequential(Matrix matrix_a, Matrix matrix_b) throws Exception {
		if (!MultiplicationSizesCheck(matrix_a, matrix_b))
			throw new Exception("matrices are not compatible for multiplication");

		int mat_a_rows = matrix_a.getRowsLength();
		int mat_a_col = matrix_a.getColumnsLength();
		double[] mat_a_val = matrix_a.getValues();

		int mat_b_col = matrix_b.getColumnsLength();
		double[] mat_b_val = matrix_b.getValues();

		Matrix matrix_result = new Matrix(mat_a_rows, mat_b_col);
		int mat_res_col = matrix_result.getColumnsLength();
		double[] mat_res_val = matrix_result.getValues();

		long start = System.currentTimeMillis();
		for (int row_a = 0; row_a < mat_a_rows; row_a++)
			for (int k = 0; k < mat_a_col; k++)
				for (int column_b = 0; column_b < mat_b_col; column_b++)
					mat_res_val[row_a * mat_res_col + column_b] += mat_a_val[row_a * mat_a_col + k] * mat_b_val[k * mat_b_col + column_b];
		long end = System.currentTimeMillis();
		System.out.printf("Time: %3.3f seconds\n", (end - start) / 1000.0f);
		return matrix_result;
	}

	private static boolean MultiplicationSizesCheck(Matrix matrix_a, Matrix matrix_b) {
		return matrix_a.getColumnsLength() == matrix_b.getRowsLength();
	}
}
