#pragma once

#include "t_vec2.hpp"
#include "t_vec3.hpp"
#include "t_vec4.hpp"
#include "t_mat.hpp"

typedef t_vec2<bool, 2> bvec2;
typedef t_vec2<unsigned int, 2> uivec2;
typedef t_vec2<int, 2> ivec2;
typedef t_vec2<float, 2> vec2;
typedef t_vec2<double, 2> dvec2;

typedef t_vec3<bool, 3> bvec3;
typedef t_vec3<unsigned int, 3> uivec3;
typedef t_vec3<int, 3> ivec3;
typedef t_vec3<float, 3> vec3;
typedef t_vec3<double, 3> dvec3;

typedef t_vec4<bool, 4> bvec4;
typedef t_vec4<unsigned int, 4> uivec4;
typedef t_vec4<int, 4> ivec4;
typedef t_vec4<float, 4> vec4;
typedef t_vec4<double, 4> dvec4;

typedef t_mat<bool, t_vec2, 2> bmat2;
typedef t_mat<unsigned int, t_vec2, 2> uimat2;
typedef t_mat<int, t_vec2, 2> imat2;
typedef t_mat<float, t_vec2, 2> mat2;
typedef t_mat<double, t_vec2, 2> dmat2;

typedef t_mat<bool, t_vec3, 3> bmat3;
typedef t_mat<unsigned int, t_vec3, 3> uimat3;
typedef t_mat<int, t_vec3, 3> imat3;
typedef t_mat<float, t_vec3, 3> mat3;
typedef t_mat<double, t_vec3, 3> dmat3;

typedef t_mat<bool, t_vec4, 4> bmat4;
typedef t_mat<unsigned int, t_vec4, 4> uimat4;
typedef t_mat<int, t_vec4, 4> imat4;
typedef t_mat<float, t_vec4, 4> mat4;
typedef t_mat<double, t_vec4, 4> dmat4;