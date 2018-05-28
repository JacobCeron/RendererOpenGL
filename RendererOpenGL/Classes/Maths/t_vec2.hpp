#pragma once
#define T_VEC2
#include <ostream>

template<typename T, size_t N>
struct t_vec3;
template<typename T, size_t N>
struct t_vec4;

template<typename T, size_t N>
struct t_vec2
{
	union
	{
		struct { T x, y; };
		struct { T r, g; };
	};

	static const size_t length();

	t_vec2();
	t_vec2(T _x);
	t_vec2(T _x, T _y);
	t_vec2(const t_vec3<T, 3>& v);
	t_vec2(const t_vec4<T, 4>& v);

	T& operator[](size_t index);
	const T& operator[](size_t index) const;

	t_vec2<T, N>& operator++();
	const t_vec2<T, N> operator++(int);
	t_vec2<T, N>& operator--();
	const t_vec2<T, N> operator--(int);

	t_vec2<T, N>& operator+=(T scalar);
	t_vec2<T, N>& operator+=(const t_vec2<T, N>& v);
	t_vec2<T, N>& operator-=(T scalar);
	t_vec2<T, N>& operator-=(const t_vec2<T, N>& v);
	t_vec2<T, N>& operator*=(T scalar);
	t_vec2<T, N>& operator*=(const t_vec2<T, N>& v);
	t_vec2<T, N>& operator/=(T scalar);
	t_vec2<T, N>& operator/=(const t_vec2<T, N>& v);

	template<typename T, size_t N>
	friend t_vec2<T, N> operator+(T scalar, const t_vec2<T, N>& v);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator+(const t_vec2<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator+(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator-(T scalar, const t_vec2<T, N>& v);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator-(const t_vec2<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator-(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator*(T scalar, const t_vec2<T, N>& v);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator*(const t_vec2<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator*(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator/(T scalar, const t_vec2<T, N>& v);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator/(const t_vec2<T, N>& v, T scalar);
	template<typename T, size_t N>
	friend t_vec2<T, N> operator/(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2);

	template<typename T, size_t N>
	friend bool operator==(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2);
	template<typename T, size_t N>
	friend bool operator!=(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2);

	template<typename T, size_t N>
	friend std::ostream& operator<<(std::ostream& ost, const t_vec2<T, N>& v);
};

#ifdef T_VEC2
#include "t_vec2.inl"
#endif