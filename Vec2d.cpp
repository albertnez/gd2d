#include "Vec2d.h"
#include <math.h>
#include <iostream>
using namespace std;

//constructors
Vec::Vec() {
	x = y = 0;
}

Vec::Vec(const double& xIni, const double& yIni) {
	x = xIni;
	y = yIni;
}

//set
void Vec::setX(const double xVal) { x = xVal; }
void Vec::setY(const double yVal) { y = yVal; }

void Vec::set(const double xVal, const double yVal) {
	x = xVal;
	y = yVal;
}

//get
double Vec::getX() const { return x; }
double Vec::getY() const { return y; }

//operators
Vec Vec::operator+(const Vec& v) const {
	return Vec(x + v.getX(), y + v.getY());
}
Vec Vec::operator-(const Vec& v) const {
	return Vec(x - v.getX(), y - v.getY());
}
Vec Vec::operator*(const double& k) const {
	return Vec(x * k, y * k);
}
Vec Vec::operator/(const double& k) const {
	return Vec(x / k, y / k);
}

//vec atributes
double Vec::module() { 
	return sqrt(x*x + y*y);
	}

Vec Vec::unit() {
	double m = sqrt(x*x + y*y);
	return Vec(x/m, y/m);
}

//print
void Vec::print() {
	cout << endl << "X: " << x << endl << "Y: " << y << endl;
}

