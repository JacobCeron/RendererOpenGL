#include <cassert>

template<typename T, size_t N>
inline const size_t t_vec2<T, N>::length()
{
	return N;
}

template<typename T, size_t N>
inline t_vec2<T, N>::t_vec2()
	: x(static_cast<T>(0)), y(static_cast<T>(0))
{
}

template<typename T, size_t N>
inline t_vec2<T, N>::t_vec2(T _x)
	: x(_x), y(_x)
{
}

template<typename T, size_t N>
inline t_vec2<T, N>::t_vec2(T _x, T _y)
	: x(_x), y(_y)
{
}

template<typename T, size_t N>
inline t_vec2<T, N>::t_vec2(const t_vec3<T, 3>& v)
	: x(v.x), y(v.y)
{
}

template<typename T, size_t N>
inline t_vec2<T, N>::t_vec2(const t_vec4<T, 4>& v)
	: x(v.x), y(v.y)
{
}

template<typename T, size_t N>
inline T& t_vec2<T, N>::operator[](size_t index)
{
	assert(index >= 0 && index < length());
	return *(&x + index);
}

template<typename T, size_t N>
inline const T& t_vec2<T, N>::operator[](size_t index) const
{
	assert(index >= 0 && index < length());
	return *(&x + index);
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator++()
{
	x++;
	y++;
	return *this;
}

template<typename T, size_t N>
inline const t_vec2<T, N> t_vec2<T, N>::operator++(int)
{
	t_vec2<T, N> temp(*this);
	x++;
	y++;
	return temp;
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator--()
{
	x--;
	y--;
	return *this;
}

template<typename T, size_t N>
inline const t_vec2<T, N> t_vec2<T, N>::operator--(int)
{
	t_vec2<T, N> temp(*this);
	x--;
	y--;
	return temp;
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator+=(T scalar)
{
	x += scalar;
	y += scalar;
	return *this;
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator+=(const t_vec2<T, N>& v)
{
	x += v.x;
	y += v.y;
	return *this;
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator-=(T scalar)
{
	x -= scalar;
	y -= scalar;
	return *this;
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator-=(const t_vec2<T, N>& v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator*=(T scalar)
{
	x *= scalar;
	y *= scalar;
	return *this;
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator*=(const t_vec2<T, N>& v)
{
	x *= v.x;
	y *= v.y;
	return *this;
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator/=(T scalar)
{
	x /= scalar;
	y /= scalar;
	return *this;
}

template<typename T, size_t N>
inline t_vec2<T, N>& t_vec2<T, N>::operator/=(const t_vec2<T, N>& v)
{
	x /= v.x;
	y /= v.y;
	return *this;
}

template<typename T, size_t N>
inline t_vec2<T, N> operator+(T scalar, const t_vec2<T, N>& v)
{
	return t_vec2<T, N>
		(
			scalar + v.x,
			scalar + v.y
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator+(const t_vec2<T, N>& v, T scalar)
{
	return t_vec2<T, N>
		(
			v.x + scalar,
			v.y + scalar
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator+(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2)
{
	return t_vec2<T, N>
		(
			v1.x + v2.x,
			v1.y + v2.y
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator-(T scalar, const t_vec2<T, N>& v)
{
	return t_vec2<T, N>
		(
			scalar - v.x,
			scalar - v.y
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator-(const t_vec2<T, N>& v, T scalar)
{
	return t_vec2<T, N>
		(
			v.x - scalar,
			v.y - scalar
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator-(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2)
{
	return t_vec2<T, N>
		(
			v1.x - v2.x,
			v1.y - v2.y
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator*(T scalar, const t_vec2<T, N>& v)
{
	return t_vec2<T, N>
		(
			scalar * v.x,
			scalar * v.y
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator*(const t_vec2<T, N>& v, T scalar)
{
	return t_vec2<T, N>
		(
			v.x * scalar,
			v.y * scalar
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator*(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2)
{
	return t_vec2<T, N>
		(
			v1.x * v2.x,
			v1.y * v2.y
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator/(T scalar, const t_vec2<T, N>& v)
{
	return t_vec2<T, N>
		(
			scalar / v.x,
			scalar / v.y
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator/(const t_vec2<T, N>& v, T scalar)
{
	return t_vec2<T, N>
		(
			v.x / scalar,
			v.y / scalar
		);
}

template<typename T, size_t N>
inline t_vec2<T, N> operator/(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2)
{
	return t_vec2<T, N>
		(
			v1.x / v2.x,
			v1.y / v2.y
		);
}

template<typename T, size_t N>
inline bool operator==(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2)
{
	return (v1.x == v2.x && v1.y == v2.y);
}

template<typename T, size_t N>
inline bool operator!=(const t_vec2<T, N>& v1, const t_vec2<T, N>& v2)
{
	return !(v1 == v2);
}

template<typename T, size_t N>
inline std::ostream& operator<<(std::ostream& ost, const t_vec2<T, N>& v)
{
	ost << v.x << " " << v.y;
	return ost;
}