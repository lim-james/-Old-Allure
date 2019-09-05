#ifndef MATRIX_TRANSFORM_HPP
#define MATRIX_TRANSFORM_HPP

#include "Matrix.hpp"

namespace Math {

	template<typename T>
	mat4<T> Frustum(const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far) {
		return mat4<T>(
			2.f * near / (right - left), 0, 0, 0,
			0, 2.f * near / (top - bottom), 0, 0,
			(right + left) / (right - left), (top + bottom) / (top - bottom), -(far + near) / (far - near), -1,
			0, 0, -2.f * far * near / (far - near), 0
		);
	}

	template<typename T>
	mat4<T> Orthographic(const T& left, const T& right, const T& bottom, const T& top, const T& near, const T& far) {
		return mat4<T>(
			2.f / (right - left), 0, 0, 0,
			0, 2.f / (top - bottom), 0, 0,
			0, 0, -2.f / (far - near), 0,
			-(right + left) / (right - left), -(top + bottom) / (top - bottom), -(far + near) / (far - near), 1
		);
	}

	template<typename T>
	mat4<T> Perspective(const T& fovy, const T& aspect, const T& near, const T& far) {
		T f = 1.f / tan(Rad(fovy / 2.f));
		return mat4<T>(
			f / aspect, 0, 0, 0,
			0,	f, 0, 0,
			0, 0, (far + near) / (near - far), -1,
			0, 0, 2.f * far * near / (near - far), 0
		);
	}

	template<typename T>
	mat4<T> Scale(const mat4<T>& mat, const vec3<T>& vec) {
		mat4<T> transform;
		SetToIdentity(transform);
		transform[0] = vec.x;
		transform[5] = vec.y;
		transform[10] = vec.z;
		return mat * transform;
	}

	template<typename T>
	void SetToScale(mat4<T>& mat, const vec3<T>& vec) {
		SetToIdentity(mat);
		mat[0] = vec.x;
		mat[5] = vec.y;
		mat[10] = vec.z;
	}

	template<typename T>
	mat4<T> Rotate(const mat4<T>& mat, const T& angle, const vec3<T>& axis) {
		mat4<T> transform;
		SetToIdentity(transform);

		const T mag = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

		const T x = axis.x / mag, y = axis.y / mag, z = axis.z / mag;
		const T c = cos(Rad(angle)), s = sin(Rad(angle));

		transform[0] = x * x * (1 - c) + c;
		transform[1] = y * x * (1 - c) + z * s;
		transform[2] = x * z * (1 - c) - y * s;
		transform[3] = 0;
		transform[4] = x * y * (1 - c) - z * s;
		transform[5] = y * y * (1 - c) + c;
		transform[6] = y * z * (1 - c) + x * s;
		transform[7] = 0;
		transform[8] = x * z * (1 - c) + y * s;
		transform[9] = y * z * (1 - c) - x * s;
		transform[10] = z * z * (1 - c) + c;
		transform[11] = 0;
		transform[12] = 0;
		transform[13] = 0;
		transform[14] = 0;
		transform[15] = 1;

		return mat * transform;
	}

	template<typename T>
	void SetToRotation(mat4<T>& mat, const T& angle, const vec3<T>& axis) {
		SetToIdentity(mat);

		const T mag = sqrt(axis.x * axis.x + axis.y * axis.y + axis.z * axis.z);

		const T x = axis.x / mag, y = axis.y / mag, z = axis.z / mag;
		const T c = cos(rad(angle)), s = sin(rad(angle));

		mat[0] = x * x * (1 - c) + c;
		mat[1] = y * x * (1 - c) + z * s;
		mat[2] = x * z * (1 - c) - y * s;
		mat[3] = 0;
		mat[4] = x * y * (1 - c) - z * s;
		mat[5] = y * y * (1 - c) + c;
		mat[6] = y * z * (1 - c) + x * s;
		mat[7] = 0;
		mat[8] = x * z * (1 - c) + y * s;
		mat[9] = y * z * (1 - c) - x * s;
		mat[10] = z * z * (1 - c) + c;
		mat[11] = 0;
		mat[12] = 0;
		mat[13] = 0;
		mat[14] = 0;
		mat[15] = 1;
	}

	template<typename T>
	void SetToRotation(mat4<T>& mat, const vec3<T>& angles) {
		SetToIdentity(mat);
		mat = rotate(mat, angles.y, vec3f(0.f, 1.f, 0.f));
		mat = rotate(mat, angles.x, vec3f(1.f, 0.f, 0.f));
		mat = rotate(mat, angles.z, vec3f(0.f, 0.f, 1.f));
	}

	template<typename T>
	mat4<T> Translate(const mat4<T>& mat, const vec3<T>& vec) {
		mat4<T> transform;
		SetToIdentity(transform);
		transform[12] = vec.x;
		transform[13] = vec.y;
		transform[14] = vec.z;
		return mat * transform;
	}

	template<typename T>
	void SetToTranslation(mat4<T>& mat, const vec3<T>& vec) {
		SetToIdentity(mat);
		mat[12] = vec.x;
		mat[13] = vec.y;
		mat[14] = vec.z;
	}

	template<typename T>
	mat4<T> LookAt(const vec3f& eye, const vec3<T>& center, const vec3<T>& up) {
		const vec3<T> f(center.x - eye.x, center.y - eye.y, center.z - eye.z);

		Normalize(f);

		const vec3<T> s = Cross(f, Normalized(up));
		const vec3<T> u = Cross(s, f);

		mat4<T> mat(
			s.x, u.x, -f.x, 0,
			s.y, u.y, -f.y, 0,
			s.z, u.z, -f.z, 0,
			0, 0, 0, 1
		);

		return Translate(mat, -eye);
	}

}

#endif