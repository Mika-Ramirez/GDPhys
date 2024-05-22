#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace P6 {

	class MyVector {
	public:
		float x, y, z;


		MyVector() : x(0), y(0), z(0) {}
		MyVector(const float _x, const float _y, const float _z) : x(_x), y(_y), z(_z) {}

		explicit operator glm::vec3() const { return glm::vec3(x, y, z); }

		void operator+= (const MyVector v) {
			this->x += v.x;
			this->y += v.y;
			this->z += v.z;
		}

		//Addition
		MyVector operator+ (const MyVector v) {
			return MyVector(this->x + v.x, this->y + v.y, this->z + v.z);
		}

		//Subtraction
		MyVector operator- (const MyVector v) {
			return MyVector(this->x - v.x, this->y - v.y, this->z - v.z);
		}

		//Component Product
		MyVector operator* (const MyVector v) {
			return MyVector(this->x * v.x, this->y * v.y, this->z * v.z);
		}

		//Scalar Multiplication
		MyVector operator* (const float ScalarMultiplication) {
			return MyVector(this->x * ScalarMultiplication, this->y * ScalarMultiplication, this->z * ScalarMultiplication);
		}

		//Dot Product
		float dotProduct(const MyVector v) {
			return ((this->x * v.x) + (this->y * v.y) + (this->z * v.z));
		}

		//Vector Product
		MyVector vectorProduct(const MyVector v) {
			//								x                  ,                   y                 ,                  z
			return MyVector(((this->y * v.z) - (this->z * v.y)), ((this->x * v.z) - (this->z * v.x)), ((this->x * v.y) - (this->y * v.x)));
		}

		//Magnitude
		float magnitude() {
			return sqrt(this->x * this->x + this->y * this->y + this->z * this->z);
		}

		//Direction
		MyVector direction() {
			return MyVector(this->x / magnitude(), this->y / magnitude(), this->z / magnitude());
		}
	};
}
