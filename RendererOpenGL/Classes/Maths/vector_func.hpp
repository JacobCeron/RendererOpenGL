#pragma once
#define VECTOR_FUNC

#include "types.h"
#include <cmath>

template<typename T, template<typename, size_t> class vecType, size_t N>
T magnitud(const vecType<T, N>& x);

template<typename T, template<typename, size_t> class vecType, size_t N>
T distance(const vecType<T, N>& p0, const vecType<T, N>& p1);

template<typename T, template<typename, size_t> class vecType, size_t N>
T dot(const vecType<T, N>& x, const vecType<T, N>& y);

template<typename T, template<typename, size_t> class vecType, size_t N>
vecType<T, N> normalize(const vecType<T, N>& v);

template<typename T>
t_vec3<T, 3> cross(const t_vec3<T, 3>& x, const t_vec3<T, 3>& y);

template<typename T, template<typename, size_t> class vecType, size_t N>
vecType<T, N> reflect(const vecType<T, N>& I, const vecType<T, N>& N);

template<typename T, template<typename, size_t> class vecType, size_t N>
vecType<T, N> refract(const vecType<T, N>& I, const vecType<T, N>& N, float eta);

#ifdef VECTOR_FUNC
#include "vector_func.inl"
#endif