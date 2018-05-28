#include <cassert>

template<typename T, size_t N>
inline const size_t t_vec4<T, N>::length()
{
	return N;
}

template<typename T, size_t N>
inline t_vec4<T, N>::t_vec4()
	: x(static_cast<T>(0)), y(static_cast<T>(0)), z(static_cast<T>(0)), w(static_cast<T>(0))
{
}

template<typename T, size_t N>
inline t_vec4<T, N>::t_vec4(T _x)
	: x(_x), y(_x), z(_x), w(_x)
{
}

template<typename T, size_t N>
inline t_vec4<T, N>::t_vec4(T _x, T _y, T _z, T _w)
	: x(_x), y(_y), z(_z), w(_w)
{
}

template<typename T, size_t N>
inline t_vec4<T, N>::t_vec4(const t_vec2<T, 2>& v, T _z, T _w)
	: x(v.x), y(v.y), z(_z), w(_w)
{
}

template<typename T, size_t N>
inline t_vec4<T, N>::t_vec4(const t_vec3<T, 3>& v, T _w)
	: x(v.x), y(v.y), z(v.z), w(_w)
{
}

template<typename T, size_t N>
inline T& t_vec4<T, N>::operator[](size_t index)
{
	assert(index >= 0 && index < length());
	return *(&x + index);
}

template<typename T, size_t N>
inline const T& t_vec4<T, N>::operator[](size_t index) const
{
	assert(index >= 0 && index < length());
	return *(&x + index);
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator++()
{
	x++;
	y++;
	z++;
	w++;
	return *this;
}

template<typename T, size_t N>
inline const t_vec4<T, N> t_vec4<T, N>::operator++(int)
{
	t_vec4<T, N> temp(*this);
	x++;
	y++;
	z++;
	w++;
	return temp;
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator--()
{
	x--;
	y--;
	z--;
	w--;
	return *this;
}

template<typename T, size_t N>
inline const t_vec4<T, N> t_vec4<T, N>::operator--(int)
{
	t_vec4<T, N> temp(*this);
	x--;
	y--;
	z--;
	w--;
	return temp;
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator+=(T scalar)
{
	x += scalar;
	y += scalar;
	z += scalar;
	w += scalar;
	return *this;
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator+=(const t_vec4<T, N>& v)
{
	x += v.x;
	y += v.y;
	z += v.z;
	w += v.w;
	return *this;
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator-=(T scalar)
{
	x -= scalar;
	y -= scalar;
	z -= scalar;
	w -= scalar;
	return *this;
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator-=(const t_vec4<T, N>& v)
{
	x -= v.x;
	y -= v.y;
	z -= v.z;
	w -= v.w;
	return *this;
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator*=(T scalar)
{
	x *= scalar;
	y *= scalar;
	z *= scalar;
	w *= scalar;
	return *this;
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator*=(const t_vec4<T, N>& v)
{
	x *= v.x;
	y *= v.y;
	z *= v.z;
	w *= v.w;
	return *this;
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator/=(T scalar)
{
	x /= scalar;
	y /= scalar;
	z /= scalar;
	w /= scalar;
	return *this;
}

template<typename T, size_t N>
inline t_vec4<T, N>& t_vec4<T, N>::operator/=(const t_vec4<T, N>& v)
{
	x /= v.x;
	y /= v.y;
	z /= v.z;
	w /= v.w;
	return *this;
}

template<typename T, size_t N>
inline t_vec4<T, N> operator+(T scalar, const t_vec4<T, N>& v)
{
	return t_vec4<T, N>
		(
			scalar + v.x,
			scalar + v.y,
			scalar + v.z,
			scalar + v.w
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator+(const t_vec4<T, N>& v, T scalar)
{
	return t_vec4<T, N>
		(
			v.x + scalar,
			v.y + scalar,
			v.z + scalar,
			v.w + scalar
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator+(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2)
{
	return t_vec4<T, N>
		(
			v1.x + v2.x,
			v1.y + v2.y,
			v1.z + v2.z,
			v1.w + v2.w
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator-(T scalar, const t_vec4<T, N>& v)
{
	return t_vec4<T, N>
		(
			scalar - v.x,
			scalar - v.y,
			scalar - v.z,
			scalar - v.w
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator-(const t_vec4<T, N>& v, T scalar)
{
	return t_vec4<T, N>
		(
			v.x - scalar,
			v.y - scalar,
			v.z - scalar,
			v.w - scalar
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator-(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2)
{
	return t_vec4<T, N>
		(
			v1.x - v2.x,
			v1.y - v2.y,
			v1.z - v2.z,
			v1.w - v2.w
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator*(T scalar, const t_vec4<T, N>& v)
{
	return t_vec4<T, N>
		(
			scalar * v.x,
			scalar * v.y,
			scalar * v.z,
			scalar * v.w
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator*(const t_vec4<T, N>& v, T scalar)
{
	return t_vec4<T, N>
		(
			v.x * scalar,
			v.y * scalar,
			v.z * scalar,
			v.w * scalar
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator*(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2)
{
	return t_vec4<T, N>
		(
			v1.x * v2.x,
			v1.y * v2.y,
			v1.z * v2.z,
			v1.w * v2.w
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator/(T scalar, const t_vec4<T, N>& v)
{
	return t_vec4<T, N>
		(
			scalar / v.x,
			scalar / v.y,
			scalar / v.z,
			scalar / v.w
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator/(const t_vec4<T, N>& v, T scalar)
{
	return t_vec4<T, N>
		(
			v.x / scalar,
			v.y / scalar,
			v.z / scalar,
			v.w / scalar
		);
}

template<typename T, size_t N>
inline t_vec4<T, N> operator/(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2)
{
	return t_vec4<T, N>
		(
			v1.x / v2.x,
			v1.y / v2.y,
			v1.z / v2.z,
			v1.w / v2.w
		);
}

template<typename T, size_t N>
inline bool operator==(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2)
{
	return (v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w);
}

template<typename T, size_t N>
inline bool operator!=(const t_vec4<T, N>& v1, const t_vec4<T, N>& v2)
{
	return !(v1 == v2);
}

template<typename T, size_t N>
inline std::ostream& operator<<(std::ostream& ost, const t_vec4<T, N>& v)
{
	ost << v.x << " " << v.y << " " << v.z << " " << v.w;
	return ost;
}