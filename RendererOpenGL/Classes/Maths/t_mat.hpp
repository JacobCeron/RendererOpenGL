#pragma once
#define MATRIX

template<typename T, template<typename, size_t> class vecType, size_t N>
struct t_mat
{
	vecType<T, N> data[N];

	t_mat();

	vecType<T, N>& operator[](size_t index);
	const vecType<T, N>& operator[](size_t index) const;

	t_mat<T, vecType, N>& operator++();
	const t_mat<T, vecType, N> operator++(int);
	t_mat<T, vecType, N>& operator--();
	const t_mat<T, vecType, N> operator--(int);

	template<typename U>
	t_mat<T, vecType, N>& operator+=(U s);
	template<typename U>
	t_mat<T, vecType, N>& operator+=(const t_mat<U, vecType, N>& m);
	template<typename U>
	t_mat<T, vecType, N>& operator-=(U s);
	template<typename U>
	t_mat<T, vecType, N>& operator-=(const t_mat<U, vecType, N>& m);
	template<typename U>
	t_mat<T, vecType, N>& operator*=(U s);
	template<typename U>
	t_mat<T, vecType, N>& operator*=(const t_mat<U, vecType, N>& m);
	template<typename U>
	t_mat<T, vecType, N>& operator/=(U s);
	template<typename U>
	t_mat<T, vecType, N>& operator/=(const t_mat<U, vecType, N>& m);

	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator+(const t_mat<T, vecType, N>& m, const T& s);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator+(const T& s, const t_mat<T, vecType, N>& m);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator+(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator-(const t_mat<T, vecType, N>& m, const T& s);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator-(const T& s, const t_mat<T, vecType, N>& m);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator-(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator*(const t_mat<T, vecType, N>& m, const T& s);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator*(const T& s, const t_mat<T, vecType, N>& m);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend vecType<T, N> operator*(const vecType<T, N>& v, const t_mat<T, vecType, N>& m);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend vecType<T, N> operator*(const t_mat<T, vecType, N>& m, const vecType<T, N>& v);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator*(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator/(const t_mat<T, vecType, N>& m, const T& s);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator/(const T& s, const t_mat<T, vecType, N>& m);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend t_mat<T, vecType, N> operator/(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2);

	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend bool operator==(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2);
	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend bool operator!=(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2);

	template<typename T, template<typename, size_t> class vecType, size_t N>
	friend std::ostream& operator<<(std::ostream& ost, const t_mat<T, vecType, N>& m);
};



#ifdef MATRIX
#include "t_mat.inl"
#endif