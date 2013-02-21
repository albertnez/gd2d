#ifndef VEC2D_H
#define VEC2D_H

class Vec {
private:
	float x, y;
public:
	//default constructor
	Vec();
	//constructor with initial values
	Vec(const float& xVal, const float& yVal);
	//getters
	float getX() const;
	float getY() const;
	//setters
	void setX(const float xVal);
	void setY(const float yVal);

	void set(const float xVal, const float yVal);
	
	//operators
	Vec operator+(const Vec& v) const;
	Vec operator-(const Vec& v) const;

	Vec operator*(const float& k) const;
	Vec operator/(const float& k) const;

	//return module
	float module();
	//unitary vector
	Vec unit();
	//print
	void print();

};

#endif