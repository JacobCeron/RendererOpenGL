#pragma once

#define MATRIX_TRANSFORMS

#include "types.h"
#include <cmath>

#undef near
#undef far

template<typename T>
t_mat<T, t_vec4, 4> translate(const t_mat<T, t_vec4, 4>& m, const t_vec3<T, 3>& v);

template<typename T>
t_mat<T, t_vec4, 4> scale(const t_mat<T, t_vec4, 4>& m, const t_vec3<T, 3>& v);

template<typename T>
t_mat<T, t_vec4, 4> rotate(const t_mat<T, t_vec4, 4>& m, T angle, const t_vec3<T, 3>& axis);

template<typename T>
t_mat<T, t_vec4, 4> lookAt(const t_vec3<T, 3>& eye, const t_vec3<T, 3>& target, const t_vec3<T, 3>& up);

template<typename T>
t_mat<T, t_vec4, 4> ortho(T left, T right, T bottom, T top, T near, T far);

template<typename T>
t_mat<T, t_vec4, 4> perspective(T fovy, T aspect, T near, T far);


#ifdef MATRIX_TRANSFORMS
#include "matrix_transform.inl"
#endif