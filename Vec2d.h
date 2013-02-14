#ifndef VEC2D_H
#define VEC2D_H

class Vec {
private:
	float x, y;
public:
	Vec();
	Vec(const float& xVal, const float& yVal);

	float getX() const;
	float getY() const;

	void setX(const float xVal);
	void setY(const float yVal);

	void set(const float xVal, const float yVal);

	Vec operator+(const Vec& v) const;
	Vec operator-(const Vec& v) const;

	Vec operator*(const float& k) const;
	Vec operator/(const float& k) const;

	float module();

	Vec unit();

	void print();

};

#endif