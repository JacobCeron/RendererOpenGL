#pragma once
#define MATRIX_FUNC

#include "types.h"
#include <cmath>

template<typename T, template<typename, size_t> class vecType, size_t N>
t_mat<T, vecType, N> transpose(const t_mat<T, vecType, N>& m);

template<typename T>
t_mat<T, t_vec4, 4> inverse(const t_mat<T, t_vec4, 4>& m);
template<typename T, template<typename, size_t> class vecType>
t_mat<T, t_vec3, 3> inverse(const t_mat<T, t_vec3, 3>& m);
template<typename T, template<typename, size_t> class vecType>
t_mat<T, t_vec2, 3> inverse(const t_mat<T, t_vec2, 3>& m);

#ifdef MATRIX_FUNC
#include "matrix_func.inl"
#endif