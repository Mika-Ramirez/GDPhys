#include "MyVector.h"


using namespace P6;

float MyVector::Magnitude() const {
	return sqrt(x * x + y * y + z * z);
}

MyVector MyVector::Direction() const
{
	float magnitude = Magnitude();

	if (magnitude == 0) {
		return MyVector(0, 0, 0);
	}

	else {
		return MyVector(this->x / magnitude, this->y / magnitude, this->z / magnitude);
	}
}

void MyVector::operator+= (const MyVector v) {
	this->x += v.x;
	this->y += v.y;
	this->z += v.z;
}

void MyVector::operator-= (const MyVector v) {
	this->x -= v.x;
	this->y -= v.y;
	this->z -= v.z;
}

MyVector MyVector::ScalarMultiplication(float toScale) const {
	return MyVector(toScale * x, toScale * y, toScale * z);
}

void MyVector::operator*= (const MyVector v) {
	this->x *= v.x;
	this->y *= v.y;
	this->z *= v.z;
}

float MyVector::DotProduct(const MyVector& v) const {
	return x * v.x + y * v.y + z * v.z;
}


MyVector MyVector::CrossProduct(const MyVector& v) const {
	return MyVector(
		this->y * v.z - z * v.y,
		this->z * v.x - x * v.z,
		this->x * v.y - y * v.x
	);
}