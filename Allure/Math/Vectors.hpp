#ifndef VECTORS_HPP
#define VECTORS_HPP

#include <iostream>
#include <sstream>

namespace Math {

	template<typename T> struct vec2;
	template<typename T> struct vec3;
	template<typename T> struct vec4;

	template<typename T>
	struct vec2 {
		T x, y;

		vec2() {}
		vec2(const T& i) : x(i), y(i) {}
		vec2(const T& x, const T& y) : x(x), y(y) {}
		vec2(const vec3<T>& v) : x(v.x), y(v.y) {}
		vec2(const vec4<T>& v) : x(v.x), y(v.y) {}

		void set(const T& i) {
			this->x = i;
			this->y = i;
		}

		void set(const T& x, const T& y) {
			this->x = x;
			this->y = y;
		}

		void set(const vec2<T>& v) {
			this->x = v.x;
			this->y = v.y;
		}

		void set(const vec3<T>& v) {
			this->x = v.x;
			this->y = v.y;
		}

		void set(const vec4<T>& v) {
			this->x = v.x;
			this->y = v.y;
		}
	};

	// standard maths
	// vec2 + vec2 = vec2
	template<typename T> 
	inline vec2<T> operator+(const vec2<T>& lhs, const vec2<T>& rhs) { 
		return vec2<T>(lhs.x + rhs.x, lhs.y + rhs.y); 
	}
	// vec2 + T = vec2
	template<typename T>
	inline vec2<T> operator+(const vec2<T>& lhs, const T& rhs) {
		return vec2<T>(lhs.x + rhs, lhs.y + rhs);
	}
	// T + vec2 = vec2
	template<typename T>
	inline vec2<T> operator+(const T& lhs, const vec2<T>& rhs) {
		return vec2<T>(lhs + rhs.x, lhs + rhs.y);
	}
	// vec2 - vec2 = vec2
	template<typename T>
	inline vec2<T> operator-(const vec2<T>& lhs, const vec2<T>& rhs) { 
		return vec2<T>(lhs.x - rhs.x, lhs.y - rhs.y); 
	}
	// vec2 - T = vec2
	template<typename T>
	inline vec2<T> operator-(const vec2<T>& lhs, const T& rhs) {
		return vec2<T>(lhs.x - rhs, lhs.y - rhs);
	}
	// T - vec2 = vec2
	template<typename T>
	inline vec2<T> operator-(const T& lhs, const vec2<T>& rhs) {
		return vec2<T>(lhs - rhs.x, lhs - rhs.y);
	}
	// vec2 * vec2 = vec2
	template<typename T>
	inline vec2<T> operator*(const vec2<T>& lhs, const vec2<T>& rhs) { 
		return vec2<T>(lhs.x * rhs.x, lhs.y * rhs.y); 
	}
	// vec2 * T = vec2
	template<typename T> 
	inline vec2<T> operator*(const vec2<T>& lhs, const T& rhs) { 
		return vec2<T>(lhs.x * rhs, lhs.y * rhs); 
	}
	// T * vec2 = vec2
	template<typename T>
	inline vec2<T> operator*(const T& lhs, const vec2<T>& rhs) { 
		return vec2<T>(lhs * rhs.x, lhs * rhs.y); 
	}
	// vec2 * vec2 = vec2
	template<typename T> 
	inline vec2<T> operator/(const vec2<T>& lhs, const vec2<T>& rhs) { 
		return vec2<T>(lhs.x / rhs.x, lhs.y / rhs.y); 
	}
	// vec2 / T = vec2
	template<typename T> 
	inline vec2<T> operator/(const vec2<T>& lhs, const T& rhs) { 
		return vec2<T>(lhs.x / rhs, lhs.y / rhs); 
	}
	// shorthand operators
	// += vec2
	template<typename T> 
	vec2<T>& operator+=(vec2<T>& lhs, const vec2<T>& rhs) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		return lhs;
	}
	// -= vec2
	template<typename T> 
	vec2<T>& operator-=(vec2<T>& lhs, const vec2<T>& rhs) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		return lhs;
	}
	// *= vec2
	template<typename T> 
	vec2<T>& operator*=(vec2<T>& lhs, const vec2<T>& rhs) {
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		return lhs;
	}
	// *= T
	template<typename T> 
	vec2<T>& operator*=(vec2<T>& lhs, const T& rhs) {
		lhs.x *= rhs;
		lhs.y *= rhs;
		return lhs;
	}
	// /= vec2
	template<typename T> 
	vec2<T>& operator/=(vec2<T>& lhs, const vec2<T>& rhs) {
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		return lhs;
	}
	// /= T
	template<typename T> 
	vec2<T>& operator/=(vec2<T>& lhs, const T& rhs) {
		lhs.x /= rhs;
		lhs.y /= rhs;
		return lhs;
	}
	// unary operators
	template<typename T> 
	inline vec2<T> operator-(const vec2<T>& v) { 
		return vec2<T>(-v.x, -v.y); 
	}
	// comparison operator
	// equal
	template<typename T> 
	inline bool operator==(const vec2<T>& lhs, const vec2<T>& rhs) { 
		return lhs.x == rhs.x && lhs.y == rhs.y; 
	}
	// not equal
	template<typename T> 
	inline bool operator!=(const vec2<T>& lhs, const vec2<T>& rhs) { 
		return lhs.x != rhs.x || lhs.y != rhs.y; 
	}
	// >
	template<typename T>
	inline bool operator>(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x > rhs.x && lhs.y > rhs.y;
	}
	// >=
	template<typename T>
	inline bool operator>=(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x >= rhs.x && lhs.y >= rhs.y;
	}
	// <
	template<typename T>
	inline bool operator<(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x < rhs.x && lhs.y < rhs.y;
	}
	// <=
	template<typename T>
	inline bool operator<=(const vec2<T>& lhs, const vec2<T>& rhs) {
		return lhs.x <= rhs.x && lhs.y <= rhs.y;
	}
	// io operators
	// input stream
	template<typename T> 
	std::istream& operator>>(std::istream& is, vec2<T>& v) {
		std::string buffer;
		std::stringstream ss;
		float value;

		for (int i = 0; i < 2; ++i) {
			std::getline(is, buffer, ',');
			ss.str(buffer);
			ss >> value;
			*(&v.x + i) = value;
		}

		return is;
	}
	// output stream
	template<typename T>
	std::ostream& operator<<(std::ostream& os, const vec2<T>& v) { 
		os << v.x << ',' << v.y;
		return os;
	}
	// length of vector
	template<typename T>
	inline T length(const vec2<T>& v) { 
		return sqrt(v.x * v.x + v.y * v.y); 
	}
	// length squared
	template<typename T> 
	inline T lengthSquared(const vec2<T>& v) { 
		return v.x * v.x + v.y * v.y; 
	}
	// dot product
	template<typename T> 
	inline T dot(const vec2<T>& lhs, const vec2<T>& rhs) { 
		return lhs.x * rhs.x + lhs.y * rhs.y; 
	}
	// normalize
	template<typename T>
	inline void normalize(vec2<T>& v) {
		v /= length(v);
	}
	// normalized
	template<typename T>
	inline vec2<T> normalized(const vec2<T>& v) {
		return v / length(v);
	}
	// projection
	template<typename T>
	inline vec2<T> project(const vec2<T>& v, const vec2<T>& onto) {
		return dot(v, onto) / lengthSquared(onto) * onto;
	}
	// abs
	template<typename T>
	inline vec2<T> abs(const vec2<T>& v) {
		return vec2<T>(std::abs(v.x), std::abs(v.y));
	}

	template<typename T>
	struct vec3 {
		T x, y, z;

		vec3() {}
		vec3(const T& i) : x(i), y(i), z(i) {}
		vec3(const T& x, const T& y, const T& z) : x(x), y(y), z(z) {}
		vec3(const vec2<T>& v, const T& z) : x(v.x), y(v.y), z(z) {}
		vec3(const vec4<T>& v) : x(v.x), y(v.y), z(v.z) {}

		void set(const T& i) {
			this->x = i;
			this->y = i;
			this->z = i;
		}

		void set(const T& x, const T& y, const T& z) {
			this->x = x;  
			this->y = y;
			this->z = z;
		}

		void set(const vec3<T>& v) {
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
		}

		void set(const vec2<T>& v, const T& z) {
			this->x = v.x;
			this->y = v.y;
			this->z = z;
		}
	};

	// standard maths
	// vec3 + vec3 = vec3
	template<typename T> 
	inline vec3<T> operator+(const vec3<T>& lhs, const vec3<T>& rhs) { 
		return vec3<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); 
	}
	// vec3 + T = vec3
	template<typename T>
	inline vec3<T> operator+(const vec3<T>& lhs, const T& rhs) {
		return vec3<T>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs);
	}
	// T + vec3 = vec3
	template<typename T>
	inline vec3<T> operator+(const T& lhs, const vec3<T>& rhs) {
		return vec3<T>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z);
	}
	// vec3 - vec3 = vec3
	template<typename T> 
	inline vec3<T> operator-(const vec3<T>& lhs, const vec3<T>& rhs) { 
		return vec3<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); 
	}
	// vec3 - T = vec3
	template<typename T>
	inline vec3<T> operator-(const vec3<T>& lhs, const T& rhs) {
		return vec3<T>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs);
	}
	// T - vec3 = vec3
	template<typename T>
	inline vec3<T> operator-(const T& lhs, const vec3<T>& rhs) {
		return vec3<T>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z);
	}
	// vec3 * vec3 = vec3
	template<typename T>
	inline vec3<T> operator*(const vec3<T>& lhs, const vec3<T>& rhs) { 
		return vec3<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); 
	}
	// vec3 * T = vec3
	template<typename T> 
	inline vec3<T> operator*(const vec3<T>& lhs, const T& rhs) { 
		return vec3<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs); 
	}
	// T * vec3 = vec3
	template<typename T> 
	inline vec3<T> operator*(const T& lhs, const vec3<T>& rhs) { 
		return vec3<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z); 
	}
	// vec3 / vec3 = vec3
	template<typename T>
	inline vec3<T> operator/(const vec3<T>& lhs, const vec3<T>& rhs) { 
		return vec3<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z); 
	}
	// vec3 / T = vec3
	template<typename T> 
	inline vec3<T> operator/(const vec3<T>& lhs, const T& rhs) { 
		return vec3<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs); 
	}
	// shorthand operators
	// += vec3
	template<typename T>
	vec3<T>& operator+=(vec3<T>& lhs, const vec3<T>& rhs) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		return lhs;
	}
	// -= vec3
	template<typename T> 
	vec3<T>& operator-=(vec3<T>& lhs, const vec3<T>& rhs) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		return lhs;
	}
	// *= vec3
	template<typename T>
	vec3<T>& operator*=(vec3<T>& lhs, const vec3<T>& rhs) {
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		return lhs;
	}
	// *= T
	template<typename T>
	vec3<T>& operator*=(vec3<T>& lhs, const T& rhs) {
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		return lhs;
	}
	// /= vec3
	template<typename T> 
	vec3<T>& operator/=(vec3<T>& lhs, const vec3<T>& rhs) {
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
		return lhs;
	}
	// /= T
	template<typename T>
	vec3<T>& operator/=(vec3<T>& lhs, const T& rhs) {
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;
		return lhs;
	}
	// unary operators
	template<typename T>
	inline vec3<T> operator-(const vec3<T>& v) { 
		return vec3<T>(-v.x, -v.y, -v.z); 
	}
	// comparison operator
	// equal
	template<typename T>
	inline bool operator==(const vec3<T>& lhs, const vec3<T>& rhs) { 
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; 
	}
	// not equal
	template<typename T> 
	inline bool operator!=(const vec3<T>& lhs, const vec3<T>& rhs) { 
		return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z; 
	}
	// >
	template<typename T>
	inline bool operator>(const vec3<T>& lhs, const vec3<T>& rhs) {
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z;
	}
	// >=
	template<typename T>
	inline bool operator>=(const vec3<T>& lhs, const vec3<T>& rhs) {
		return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z;
	}
	// <
	template<typename T>
		inline bool operator<(const vec3<T>&lhs, const vec3<T>&rhs) {
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
	}
	// <=
	template<typename T>
		inline bool operator<=(const vec3<T>&lhs, const vec3<T>&rhs) {
		return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z;
	}
	// io operators
	// input stream
	template<typename T>
	std::istream& operator>>(std::istream& is, vec3<T>& v) {
		std::string buffer;
		std::stringstream ss;
		float value;

		for (int i = 0; i < 3; ++i) {
			std::getline(is, buffer, ',');
			ss.str(buffer);
			ss >> value;
			*(&v.x + i) = value;
		}

		return is;
	}
	// output stream
	template<typename T> 
	std::ostream& operator<<(std::ostream& os, const vec3<T>& v) {
		os << v.x << ',' << v.y << ',' << v.z;
		return os;
	}
	// length of vector
	template<typename T> 
	inline T length(const vec3<T>& v) { 
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z); 
	}
	// length squared
	template<typename T> 
	inline T lengthSquared(const vec3<T>& v) { 
		return v.x * v.x + v.y * v.y + v.z * v.z; 
	}
	// dot product
	template<typename T> 
	inline T dot(const vec3<T>& lhs, const vec3<T>& rhs) { 
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; 
	}
	// cross product
	template<typename T>
	inline vec3<T> cross(const vec3<T>& lhs, const vec3<T>& rhs) {
		return vec3<T>(
			lhs.y * rhs.z - lhs.z * rhs.y, 
			lhs.z * rhs.x - lhs.x * rhs.z, 
			lhs.x * rhs.y - lhs.y * rhs.x
		);
	}
	// normalize
	template<typename T>
	inline void normalize(vec3<T>& v) {
		v /= length(v);
	}
	// normalized
	template<typename T>
	inline vec3<T> normalized(const vec3<T>& v) {
		return v / length(v);
	}
	// projection
	template<typename T>
	inline vec3<T> project(const vec3<T>& v, const vec3<T>& onto) {
		return dot(v, onto) / lengthSquared(onto) * onto;
	}
	// abs
	template<typename T>
	inline vec3<T> abs(const vec3<T>& v) {
		return vec3<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z));
	}

	template<typename T>
	struct vec4 {
		T x, y, z, w;

		vec4() {}
		vec4(const T& i) : x(i), y(i), z(i), w(i) {}
		vec4(const T& x, const T& y, const T& z, const T& w) : x(x), y(y), z(z), w(w) {}
		vec4(const vec2<T>& v, const T& z, const T& w) : x(v.x), y(v.y), z(z), w(w) {}
		vec4(const vec3<T>& v, const T& w) : x(v.x), y(v.y), z(v.z), w(w) {}
	
		void set(const T& i) {
			this->x = i;
			this->y = i;
			this->z = i;
			this->w = i;
		}

		void set(const T& x, const T& y, const T& z, const T& w) {
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		void set(const vec4<T>& v) {
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = v.w;
		}

		void set(const vec2<T>& v, const T& z, const T& w) {
			this->x = v.x;
			this->y = v.y;
			this->z = z;
			this->w = w;
		}

		void set(const vec3<T>& v, const T& w) {
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = w;
		}
	};

	// standard maths
	// vec4 + vec4 = vec4
	template<typename T> 
	inline vec4<T> operator+(const vec4<T>& lhs, const vec4<T>& rhs) { 
		return vec4<T>(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); 
	}
	// vec4 + T = vec4
	template<typename T>
	inline vec4<T> operator+(const vec4<T>& lhs, const T& rhs) {
		return vec4<T>(lhs.x + rhs, lhs.y + rhs, lhs.z + rhs, lhs.w + rhs);
	}
	// T + vec4 = vec4
	template<typename T>
	inline vec4<T> operator+(const T& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs + rhs.x, lhs + rhs.y, lhs + rhs.z, lhs + rhs.w);
	}
	// vec4 - vec4 = vec4
	template<typename T> 
	inline vec4<T> operator-(const vec4<T>& lhs, const vec4<T>& rhs) { 
		return vec4<T>(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); 
	}
	// vec4 - T = vec4
	template<typename T>
	inline vec4<T> operator-(const vec4<T>& lhs, const T& rhs) {
		return vec4<T>(lhs.x - rhs, lhs.y - rhs, lhs.z - rhs, lhs.w - rhs);
	}
	// T - vec4 = vec4
	template<typename T>
	inline vec4<T> operator-(const T& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs - rhs.x, lhs - rhs.y, lhs - rhs.z, lhs - rhs.w);
	}
	// vec4 * vec4 = vec4
	template<typename T> 
	inline vec4<T> operator*(const vec4<T>& lhs, const vec4<T>& rhs) {
		return vec4<T>(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); 
	}
	// vec4 * T = vec4
	template<typename T> 
	inline vec4<T> operator*(const vec4<T>& lhs, const T& rhs) { 
		return vec4<T>(lhs.x * rhs, lhs.y * rhs, lhs.z * rhs, lhs.w * rhs); 
	}
	// T * vec4 = vec4
	template<typename T> 
	inline vec4<T> operator*(const T& lhs, const vec4<T>& rhs) { 
		return vec4<T>(lhs * rhs.x, lhs * rhs.y, lhs * rhs.z, lhs * rhs.w); 
	}
	// vec4 / vec4 = vec4
	template<typename T> 
	inline vec4<T> operator/(const vec4<T>& lhs, const vec4<T>& rhs) { 
		return vec4<T>(lhs.x / rhs.x, lhs.y / rhs.y, lhs.z / rhs.z, lhs.w / rhs.w); 
	}
	// vec4 / T = vec4
	template<typename T> 
	inline vec4<T> operator/(const vec4<T>& lhs, const T& rhs) { 
		return vec4<T>(lhs.x / rhs, lhs.y / rhs, lhs.z / rhs, lhs.w / rhs); 
	}
	// shorthand operators
	// += vec4
	template<typename T> 
	vec4<T>& operator+=(vec4<T>& lhs, const vec4<T>& rhs) {
		lhs.x += rhs.x;
		lhs.y += rhs.y;
		lhs.z += rhs.z;
		lhs.w += rhs.w;
		return lhs;
	}
	// -= vec4
	template<typename T> 
	vec4<T>& operator-=(vec4<T>& lhs, const vec4<T>& rhs) {
		lhs.x -= rhs.x;
		lhs.y -= rhs.y;
		lhs.z -= rhs.z;
		lhs.w -= rhs.w;
		return lhs;
	}
	// *= vec4
	template<typename T> 
	vec4<T>& operator*=(vec4<T>& lhs, const vec4<T>& rhs) {
		lhs.x *= rhs.x;
		lhs.y *= rhs.y;
		lhs.z *= rhs.z;
		lhs.w *= rhs.w;
		return lhs;
	}
	// *= T
	template<typename T> 
	vec4<T>& operator*=(vec4<T>& lhs, const T& rhs) {
		lhs.x *= rhs;
		lhs.y *= rhs;
		lhs.z *= rhs;
		lhs.w *= rhs;
		return lhs;
	}
	// /= vec4
	template<typename T> 
	vec4<T>& operator/=(vec4<T>& lhs, const vec4<T>& rhs) {
		lhs.x /= rhs.x;
		lhs.y /= rhs.y;
		lhs.z /= rhs.z;
		lhs.w /= rhs.w;
		return lhs;
	}
	// /= T
	template<typename T> 
	vec4<T>& operator/=(vec4<T>& lhs, const T& rhs) {
		lhs.x /= rhs;
		lhs.y /= rhs;
		lhs.z /= rhs;
		lhs.w /= rhs;
		return lhs;
	}
	// unary operators
	template<typename T> 
	inline vec4<T> operator-(const vec4<T>& v) { 
		return vec4<T>(-v.x, -v.y, -v.z, -v.w); 
	}
	// comparison operator
	// equal
	template<typename T>
	inline bool operator==(const vec4<T>& lhs, const vec4<T>& rhs) { 
		return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z && lhs.w == rhs.w; 
	}
	// not equal
	template<typename T> 
	inline bool operator!=(const vec4<T>& lhs, const vec4<T>& rhs) { 
		return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z || lhs.w != rhs.w; 
	}
	// >
	template<typename T>
	inline bool operator>(const vec4<T>& lhs, const vec4<T>& rhs) {
		return lhs.x > rhs.x && lhs.y > rhs.y && lhs.z > rhs.z && lhs.w > rhs.w;
	}
	// >=
	template<typename T>
	inline bool operator>=(const vec4<T>& lhs, const vec4<T>& rhs) {
		return lhs.x >= rhs.x && lhs.y >= rhs.y && lhs.z >= rhs.z && lhs.w >= rhs.w;
	}
	// <
	template<typename T>
	inline bool operator<(const vec4<T>&lhs, const vec4<T>&rhs) {
		return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z && lhs.w < rhs.w;
	}
	// <=
	template<typename T>
	inline bool operator<=(const vec4<T>&lhs, const vec4<T>&rhs) {
		return lhs.x <= rhs.x && lhs.y <= rhs.y && lhs.z <= rhs.z && lhs.w <= rhs.w;
	}
	// io operators
	// input stream
	template<typename T> 
	std::istream& operator>>(std::istream& is, vec4<T>& v) {
		std::string buffer;
		std::stringstream ss;
		float value;

		for (int i = 0; i < 4; ++i) {
			std::getline(is, buffer, ',');
			ss.str(buffer);
			ss >> value;
			*(&v.x + i) = value;
		}

		return is;
	} 
	// output stream
	template<typename T> 
	std::ostream& operator<<(std::ostream& os, const vec4<T>& v) {
		os << v.x << ',' << v.y << ',' << v.z << ',' << v.w;
		return os;
	}
	// length of vector
	// length
	template<typename T> 
	inline T length(const vec4<T>& v) { 
		return sqrt(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w); 
	}
	// length squared
	template<typename T> 
	inline T lengthSquared(const vec4<T>& v) { 
		return v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w; 
	}
	// dot product
	template<typename T> 
	inline T dot(const vec4<T>& lhs, const vec4<T>& rhs) { 
		return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; 
	}
	// normalize
	template<typename T>
	inline void normalize(vec4<T>& v) {
		v /= length(v);
	}
	// normalized
	template<typename T>
	inline vec4<T> normalized(const vec4<T>& v) {
		return v / length(v);
	}
	// projection
	template<typename T>
	inline vec4<T> project(const vec4<T> v, const vec4<T> onto) {
		return dot(v, onto) / lengthSquared(onto) * onto;
	}
	// abs
	template<typename T>
	inline vec4<T> abs(const vec4<T>& v) {
		return vec4<T>(std::abs(v.x), std::abs(v.y), std::abs(v.z), std::abs(v.w));
	}

}

typedef Math::vec2<float> vec2f;
typedef Math::vec3<float> vec3f;
typedef Math::vec4<float> vec4f;

#endif