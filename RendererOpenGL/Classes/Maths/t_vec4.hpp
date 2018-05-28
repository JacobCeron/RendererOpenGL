#pragma once
#define T_vec4
#include <ostream>

template<typename T, size_t N>
struct t_vec2;
template<typename T, size_t N>
struct t_vec3;

template<typename T, size_t N>
struct t_vec4
{
	union
	{
		struct { T x, y, z, w; };
		struct { T r, g, b, a; };
	};

	static const size_t length();

	t_vec4();
	t_vec4(T _x);
	t_vec4(T _x, T _y, T _z, T _w);
	t_vec4(const t_vec2<T, 2>& v, T _z, T _w);
	t_vec4(const t_vec3<T, 3>& v, T _w);

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	t_vec4<T, N>& operator++();
	const t_vec4<T, N> operator++(int);
	t_vec4<T, N>& operator--();
	const t_vec4<T, N> operator--(int);

	t_vec4<T, N>& operator+=(T scalar);
	t_vec4<T, N>& operator+=(const t_vec4<T, N>& v);
	t_vec4<T, N>& operator-=(T scalar);
	t_vec4<T, N>& operator-=(const t_vec4<T, N>& v);
	t_vec4<T, N>& operator*=(T scalar);
	t_vec4<T, N>& operator*=(const t_vec4<T, N>& v);
	t_vec4<T, N>& operator/=(T scalar);
	t_vec4<T, N>& operator/=(const t_vec4<T, N>& v);

	template<typename T, size_t N>
	friend t_vec4<T, N> operator+(T scalar, const t_vec4<T, N>& v);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator+(const t_vec4<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator+(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator-(T scalar, const t_vec4<T, N>& v);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator-(const t_vec4<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator-(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator*(T scalar, const t_vec4<T, N>& v);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator*(const t_vec4<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator*(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator/(T scalar, const t_vec4<T, N>& v);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator/(const t_vec4<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec4<T, N> operator/(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2);

	template<typename T, size_t N>
	friend bool operator==(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2);
	template<typename T, size_t N>
	friend bool operator!=(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2);

	template<typename T, size_t N>
	friend std::ostream& operator<<(std::ostream& ost, const t_vec4<T, N>& v);
};

#ifdef T_vec4
#include "t_vec4.inl"
#endif