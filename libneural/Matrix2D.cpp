#include <cstdlib>
#include "Matrix2D.h"

Matrix2D::Matrix2D(int width, int height) {
	width_ = width;
	height_ = height;
	memLength_ = width_*height_;

	array_ = new double[width_*height_];
	for (int i=0; i<width_*height_; ++i)
		array_[i] = 0;
}

Matrix2D::Matrix2D(const Matrix2D &rhs) {
	width_ = rhs.width_;
	height_ = rhs.height_;
	memLength_ = width_*height_; 

	array_ = new double[width_*height_];
	for (int i=0; i<width_*height_; ++i)
		array_[i] = rhs.array_[i];
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
		throw -1;
		//return;
	}

	for (int i=0; i<width_*height_; ++i)
		array_[i] += rhs.array_[i];

	return *this;
}

Matrix2D Matrix2D::operator*(Matrix2D &rhs) {
	// wrong operand matrix size
	if (width_ != rhs.height_) {
		throw -2;
		//return;
	}

	Matrix2D result(rhs.width_, height_);

	for (int x=0; x<rhs.width_; ++x)
	for (int y=0; y<height_; ++y) {
		result.array_[x + y*rhs.width_] = 0;
		for (int i=0; i<width_; ++i)
			result.array_[x + y*rhs.width_] += 
			(double) array_[i + y*width_] * rhs.array_[x + i*rhs.width_];
	}

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

		for (int i=0; i<width_*height_; ++i)
			array_[i] = rhs.array_[i];
	}

	return *this;
}
