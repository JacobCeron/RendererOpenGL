template<typename T, template<typename, size_t> class vecType, size_t N>
inline t_mat<T, vecType, N> transpose(const t_mat<T, vecType, N>& m)
{
	return t_mat<T, vecType, N>();
}

template<typename T>
inline t_mat<T, t_vec4, 4> inverse(const t_mat<T, t_vec4, 4>& m)
{
	return t_mat<T, t_vec4, 4>();
}

template<typename T>
inline t_mat<T, t_vec3, 3> inverse(const t_mat<T, t_vec3, 3>& m)
{
	return t_mat<T, t_vec3, 3>();
}

template<typename T>
inline t_mat<T, t_vec2, 2> inverse(const t_mat<T, t_vec2, 2>& m)
{
	return t_mat<T, t_vec2, 2>();
}