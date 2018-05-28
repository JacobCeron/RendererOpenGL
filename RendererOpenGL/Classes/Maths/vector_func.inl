template<typename T, template<typename, size_t> class vecType, size_t N>
inline T magnitud(const vecType<T, N>& x)
{
	return static_cast<T>(sqrtf(dot(x, x)));
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline T distance(const vecType<T, N>& p0, const vecType<T, N>& p1)
{
	return magnitud(p0 - p1);
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline T dot(const vecType<T, N>& x, const vecType<T, N>& y)
{
	vecType<T, N> temp(x * y);
	T r{ static_cast<T>(0) };
	for (size_t i{ 0 }; i < temp.length(); i++)
		r += temp[i];
	return r;
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline vecType<T, N> normalize(const vecType<T, N>& v)
{
	return v / magnitud(v);
}

template<typename T>
inline t_vec3<T, 3> cross(const t_vec3<T, 3>& x, const t_vec3<T, 3>& y)
{
	return t_vec3<T, 3>
		(
			x.y * y.z - x.z * y.y,
			x.z * y.x - x.x * y.z,
			x.x * y.y - x.y * y.x
		);
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline vecType<T, N> reflect(const vecType<T, N>& I, const vecType<T, N>& N)
{
	return vecType<T, N>(I - static_cast<T>(2) * dot(N, I) * N);
}

template<typename T, template<typename, size_t> class vecType, size_t N>
inline vecType<T, N> refract(const vecType<T, N>& I, const vecType<T, N>& N, float eta)
{
	T k{ 1.0f - eta * eta * (1.0f - dot(N, I) * dot(N, I)) };
	if (k < static_cast<T>(0))
		return vecType<T, N>(static_cast<T>(0));
	else
		return vecType<T, N>(eta * I - (eta * dot(N, I) + static_cast<T>(sqrtf(k))) + N);
}