#ifndef VEC2D_H
#define VEC2D_H

class Vec {
private:
	double x, y;
public:
	Vec();
	Vec(const double& xVal, const double& yVal);

	double getX() const;
	double getY() const;

	void setX(const double xVal);
	void setY(const double yVal);

	void set(const double xVal, const double yVal);

	Vec operator+(const Vec& v) const;
	Vec operator-(const Vec& v) const;

	Vec operator*(const double& k) const;
	Vec operator/(const double& k) const;

	double module();

	Vec unit();

	void print();

};

#endif