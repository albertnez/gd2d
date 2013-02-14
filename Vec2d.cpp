#include "Vec2d.h"
#include <math.h>
#include <iostream>
using namespace std;

//constructors
Vec::Vec() {
	x = y = 0;
}

Vec::Vec(const float& xIni, const float& yIni) {
	x = xIni;
	y = yIni;
}

//set
void Vec::setX(const float xVal) { x = xVal; }
void Vec::setY(const float yVal) { y = yVal; }

void Vec::set(const float xVal, const float yVal) {
	x = xVal;
	y = yVal;
}

//get
float Vec::getX() const { return x; }
float Vec::getY() const { return y; }

//operators
Vec Vec::operator+(const Vec& v) const {
	return Vec(x + v.getX(), y + v.getY());
}
Vec Vec::operator-(const Vec& v) const {
	return Vec(x - v.getX(), y - v.getY());
}
Vec Vec::operator*(const float& k) const {
	return Vec(x * k, y * k);
}
Vec Vec::operator/(const float& k) const {
	return Vec(x / k, y / k);
}

//vec atributes
float Vec::module() { 
	return sqrt(x*x + y*y);
	}

Vec Vec::unit() {
	float m = sqrt(x*x + y*y);
	return Vec(x/m, y/m);
}

//print
void Vec::print() {
	cout << endl << "X: " << x << endl << "Y: " << y << endl;
}

