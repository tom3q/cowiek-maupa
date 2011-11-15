#include <cstdlib>
#include <stdexcept>
#include "Matrix2D.h"

Matrix2D::Matrix2D(int width, int height) {
	width_ = width;
	height_ = height;
	memLength_ = width_*height_;

	array_ = new double[memLength_];
	for (int i=0; i<memLength_; ++i)
		array_[i] = 0;
}

Matrix2D::Matrix2D(const Matrix2D &rhs) {
	width_ = rhs.width_;
	height_ = rhs.height_;
	memLength_ = width_*height_; 

	array_ = new double[memLength_];
	memcpy(array_, rhs.array_, memLength_ * sizeof(double));
}

Matrix2D::~Matrix2D() {
	delete array_;
}

int Matrix2D::getWidth() const {
	return width_;
}

int Matrix2D::getHeight() const {
	return height_;
}

void Matrix2D::fill(double a) {
	for (int i=0; i<width_*height_; ++i)
		array_[i] = a;
}

double &Matrix2D::at(int x, int y) {
	return array_[x + y*width_];
}

Matrix2D & Matrix2D::operator+=(Matrix2D &rhs) {
	// wrong operand matrix size
	if (rhs.width_ != width_ || rhs.height_ != height_) {
		throw std::invalid_argument("wrong operand matrix size");
		//return;
	}

	for (int i=0; i<width_*height_; ++i)
		array_[i] += rhs.array_[i];

	return *this;
}

inline void Matrix2D::multiplyMatrices(Matrix2D &dest, Matrix2D &lhs, Matrix2D &rhs, int lw, int lh, int rw, int rh)
{
	for (int x=0; x<rw; ++x) {
		for (int y=0; y<lh; ++y) {
			array_[x + y*rw] = 0;
			for (int i=0; i<lw; ++i) {
				array_[x + y*rw] +=
					(double) lhs.array_[i + y*lw] * rhs.array_[x + i*rw];
			}
		}
	}
}

Matrix2D Matrix2D::operator*(Matrix2D &rhs) {
	// wrong operand matrix size
	if (width_ != rhs.height_) {
		throw std::invalid_argument("wrong operand matrix size");
		//return;
	}

	Matrix2D result(rhs.width_, height_);

	multiplyMatrices(result, *this, rhs, width_, height_, rhs.width_, rhs.height_);

	return result;
}

Matrix2D &Matrix2D::operator=(Matrix2D &rhs) {
	if (&rhs != this)
	{
		// free if there is a need
		if (rhs.width_*rhs.height_ > memLength_) {
			delete array_;
			memLength_ = rhs.width_*rhs.height_;
			array_ = new double[memLength_];
		}

		// copy new data
		width_ = rhs.width_;
		height_ = rhs.height_;

		memcpy(array_, rhs.array_, memLength_ * sizeof(double));
	}

	return *this;
}
