#include "Matrices.h"

namespace Matrices {

	Matrix::Matrix(int _rows, int _cols) {
		a.resize(_rows, vector<double>(_cols, 0));
		rows = _rows;
		cols = _cols;
	}

	Matrix operator+(const Matrix& a, const Matrix& b) {
		if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) {
			throw runtime_error("Error: dimensions must agree");
		}
		Matrix c(a.getRows(), a.getCols());

		for (int i = 0; i < a.getRows(); ++i) {
			for (int j = 0; j < a.getCols(); ++j) {
				c(i, j) = a(i, j) + b(i, j);
			}
		}

		return c;

	}
	Matrix operator*(const Matrix& a, const Matrix& b) {
		if (a.getCols() != b.getRows()) {
			throw runtime_error("Error: dimensions must agree");
		}

		Matrix c(a.getRows(), b.getCols()); //something going wrong here

		for (int i = 0; i < a.getRows(); i++) {
			for (int k = 0; k < b.getCols(); k++) {
				for (int j = 0; j < a.getCols(); j++) {
					c(i, k) += a(i, j) * b(j, k);
				}
			}
		}
		return c;
	}

	bool operator==(const Matrix& a, const Matrix& b) {
		if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) {
			return false;
		}
		for (int i = 0; i < a.getRows(); i++) {
			for (int j = 0; j < a.getCols(); j++) {
				if (a(i, j) != b(i, j)) {
					return false;
				}
			}
		}
		return true;
	}
	bool operator!=(const Matrix& a, const Matrix& b) {
		if (a.getRows() != b.getRows() || a.getCols() != b.getCols()) {
			return true;
		}
		for (int i = 0; i < a.getRows(); i++) {
			for (int j = 0; j < a.getCols(); j++) {
				if (a(i, j) != b(i, j)) {
					return true;
				}
			}
		}
		return false;
	}
	ostream& operator<<(ostream& os, const Matrix& a) {
		for (int i = 0; i < a.getRows(); i++) {
			for (int j = 0; j < a.getCols(); j++) {
				os << setw(10) << a(i, j);
			}
			os << std::endl;
		}
		return os;
	}
	RotationMatrix::RotationMatrix(double theta) : Matrix(2,2)
	{
		(*this)(0, 0) = cos(theta);
		(*this)(0, 1) = -sin(theta);
		(*this)(1, 0) = sin(theta);
		(*this)(1, 1) = cos(theta);
	}
	ScalingMatrix::ScalingMatrix(double c) : Matrix(2,2)
	{
		(*this)(0, 0) = c;
		(*this)(0, 1) = 0;
		(*this)(1, 0) = 0;
		(*this)(1, 1) = c;

	}
	TranslationMatrix::TranslationMatrix(double xShift, double yShift, int nCols): Matrix(2,nCols)
	{

		for (int i = 0; i < nCols; i++)
		{
			(*this)(0, i) = xShift;
			(*this)(1, i) = yShift;
		}
	}
}