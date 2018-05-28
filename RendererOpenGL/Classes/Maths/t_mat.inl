#include <cassert>

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N>::t_mat()
{
	for (size_t i{ 0 }; i < N; i++)
		data[i][i] = static_cast<T>(1);
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline vecType<T, N>& t_mat<T, vecType, N>::operator[](size_t index)
{
	assert(index >= 0 && index < N);
	return data[index];
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline const vecType<T, N>& t_mat<T, vecType, N>::operator[](size_t index) const
{
	assert(index >= 0 && index < N);
	return data[index];
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator++()
{
	for (size_t i{ 0 }; i < N; i++)
		++data[i];
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline const t_mat<T, vecType, N> t_mat<T, vecType, N>::operator++(int)
{
	t_mat<T, vecType, N> temp(*this);
	for (size_t i{ 0 }; i < N; i++)
		data[i]++;
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator--()
{
	for (size_t i{ 0 }; i < N; i++)
		--data[i];
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline const t_mat<T, vecType, N> t_mat<T, vecType, N>::operator--(int)
{
	t_mat<T, vecType, N> temp(*this);
	for (size_t i{ 0 }; i < N; i++)
		data[i]--;
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
template<typename U>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator+=(U s)
{
	for (size_t i{ 0 }; i < N; i++)
		data[i] += static_cast<T>(s);
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
template<typename U>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator+=(const t_mat<U, vecType, N>& m)
{
	for (size_t i{ 0 }; i < N; i++)
		data[i] += m[i];
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
template<typename U>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator-=(U s)
{
	for (size_t i{ 0 }; i < N; i++)
		data[i] -= static_cast<T>(s);
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
template<typename U>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator-=(const t_mat<U, vecType, N>& m)
{
	for (size_t i{ 0 }; i < N; i++)
		data[i] -= m[i];
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
template<typename U>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator*=(U s)
{
	for (size_t i{ 0 }; i < N; i++)
		data[i] *= static_cast<T>(s);
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
template<typename U>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator*=(const t_mat<U, vecType, N>& m)
{
	for (size_t i{ 0 }; i < N; i++)
		data[i] *= m[i];
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
template<typename U>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator/=(U s)
{
	for (size_t i{ 0 }; i < N; i++)
		data[i] /= static_cast<T>(s);
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
template<typename U>
inline t_mat<T, vecType, N>& t_mat<T, vecType, N>::operator/=(const t_mat<U, vecType, N>& m)
{
	for (size_t i{ 0 }; i < N; i++)
		data[i] /= m[i];
	return *this;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator+(const t_mat<T, vecType, N>& m, const T& s)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = m[i] + s;
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator+(const T& s, const t_mat<T, vecType, N>& m)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = s + m[i];
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator+(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = m1[i] + m2[i];
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator-(const t_mat<T, vecType, N>& m, const T& s)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = m[i] - s;
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator-(const T& s, const t_mat<T, vecType, N>& m)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = s - m[i];
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator-(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = m1[i] - m2[i];
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator*(const t_mat<T, vecType, N>& m, const T& s)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = m[i] * s;
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator*(const T& s, const t_mat<T, vecType, N>& m)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = s * m[i];
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline vecType<T, N> operator*(const vecType<T, N>& v, const t_mat<T, vecType, N>& m)
{
	vecType<T, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = dot(v, m[i]);
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline vecType<T, N> operator*(const t_mat<T, vecType, N>& m, const vecType<T, N>& v)
{
	vecType<T, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = dot(m[i], v);
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator*(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		for (size_t j{ 0 }; j < N; j++)
			temp[i][j] = dot(m1[i], m2[j]);
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator/(const t_mat<T, vecType, N>& m, const T& s)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = m[i] / s;
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator/(const T& s, const t_mat<T, vecType, N>& m)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = s / m[i];
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> operator/(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2)
{
	t_mat<T, vecType, N> temp;
	for (size_t i{ 0 }; i < N; i++)
		temp[i] = m1[i] / m2[i];
	return temp;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline bool operator==(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2)
{
	for (size_t i{ 0 }; i < N; i++)
		if (m1[i] != m2[i])
			return false;
	return true;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline bool operator!=(const t_mat<T, vecType, N>& m1, const t_mat<T, vecType, N>& m2)
{
	return !(m1 == m2);
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline std::ostream& operator<<(std::ostream& ost, const t_mat<T, vecType, N>& m)
{
	for (size_t i{ 0 }; i < N; i++)
		ost << m[i] << std::endl;
	return ost;
}