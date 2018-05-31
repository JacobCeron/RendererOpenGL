#pragma once
#define T_VEC3
#include <ostream>

template<typename T, size_t N>
struct t_vec2;
template<typename T, size_t N>
struct t_vec4;

template<typename T, size_t N>
struct t_vec3
{
	union
	{
		struct { T x, y, z; };
		struct { T r, g, b; };
	};

	static const size_t length();

	t_vec3();
	t_vec3(T _x);
	t_vec3(T _x, T _y, T _z);
	t_vec3(const t_vec2<T, 2>& v, T _z);
	t_vec3(T _x, const t_vec2<T, 2>& v);
	t_vec3(const t_vec4<T, 4>& v);

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	t_vec3<T, N>& operator++();
	const t_vec3<T, N> operator++(int);
	t_vec3<T, N>& operator--();
	const t_vec3<T, N> operator--(int);

	t_vec3<T, N>& operator+=(T scalar);
	t_vec3<T, N>& operator+=(const t_vec3<T, N>& v);
	t_vec3<T, N>& operator-=(T scalar);
	t_vec3<T, N>& operator-=(const t_vec3<T, N>& v);
	t_vec3<T, N>& operator*=(T scalar);
	t_vec3<T, N>& operator*=(const t_vec3<T, N>& v);
	t_vec3<T, N>& operator/=(T scalar);
	t_vec3<T, N>& operator/=(const t_vec3<T, N>& v);

	template<typename T, size_t N>
	friend t_vec3<T, N> operator+(T scalar, const t_vec3<T, N>& v);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator+(const t_vec3<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator+(const t_vec3<T, N>& v1, const t_vec3<T, N>& v2);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator-(T scalar, const t_vec3<T, N>& v);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator-(const t_vec3<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator-(const t_vec3<T, N>& v1, const t_vec3<T, N>& v2);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator*(T scalar, const t_vec3<T, N>& v);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator*(const t_vec3<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator*(const t_vec3<T, N>& v1, const t_vec3<T, N>& v2);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator/(T scalar, const t_vec3<T, N>& v);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator/(const t_vec3<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec3<T, N> operator/(const t_vec3<T, N>& v1, const t_vec3<T, N>& v2);

	template<typename T, size_t N>
	friend bool operator==(const t_vec3<T, N>& v1, const t_vec3<T, N>& v2);
	template<typename T, size_t N>
	friend bool operator!=(const t_vec3<T, N>& v1, const t_vec3<T, N>& v2);

	template<typename T, size_t N>
	friend std::ostream& operator<<(std::ostream& ost, const t_vec3<T, N>& v);
};

#ifdef T_VEC3
#include "t_vec3.inl"
#endif