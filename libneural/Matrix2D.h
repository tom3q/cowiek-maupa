#ifndef MATRIX2D_H
#define MATRIX2D_H

class Matrix2D
{
public:
	Matrix2D(int width = 1, int height = 1);
	Matrix2D(const Matrix2D &rhs);
	~Matrix2D();

	int getWidth() const;
	int getHeight() const; 

	void fill(double a);
	double &at(int x, int y);
	Matrix2D &operator+=(Matrix2D &rhs); 
	Matrix2D operator*(Matrix2D &rhs);
	Matrix2D &operator=(Matrix2D &rhs);

private:
	double *array_;
	int width_, height_, memLength_;
};

#endif
