#include <iostream>

template<typename T>
inline t_mat<T, t_vec4, 4> translate(const t_mat<T, t_vec4, 4>& m, const t_vec3<T, 3>& v)
{
	t_mat<T, t_vec4, 4> r(m);
	r[3] = v[0] * m[0] + v[1] * m[1] + v[2] * m[2] + static_cast<T>(1) * m[3];
	return r;
}

template<typename T>
inline t_mat<T, t_vec4, 4> scale(const t_mat<T, t_vec4, 4>& m, const t_vec3<T, 3>& v)
{
	t_mat<T, t_vec4, 4> r(m);
	r[0] = v[0] * m[0];
	r[1] = v[1] * m[1];
	r[2] = v[2] * m[2];
	r[3] = m[3];
	return r;
}

template<typename T>
inline t_mat<T, t_vec4, 4> rotate(const t_mat<T, t_vec4, 4>& m, T angle, const t_vec3<T, 3>& axis)
{
	t_mat<T, t_vec4, 4> rot;
	vec3 u = normalize(axis);
	float cosA = cos(angle);
	float sinA = sin(angle);

	rot[0][0] = cosA + u.x * u.x * (1 - cosA);
	rot[1][0] = u.x * u.y * (1 - cosA) - u.z * sinA;
	rot[2][0] = u.x * u.z * (1 - cosA) + u.y * sinA;

	rot[0][1] = u.y * u.x * (1 - cosA) + u.z * sinA;
	rot[1][1] = cosA + u.y * u.y * (1 - cosA);
	rot[2][1] = u.y * u.z * (1 - cosA) - u.x * sinA;

	rot[0][2] = u.z * u.x * (1 - cosA) - u.y * sinA;
	rot[1][2] = u.z * u.y * (1 - cosA) + u.x * sinA;
	rot[2][2] = cosA + u.z * u.z * (1 - cosA);

	t_mat<T, t_vec4, 4> r;
	r[0] = m[0] * rot[0][0] + m[1] * rot[0][1] + m[2] * rot[0][2];
	r[1] = m[0] * rot[1][0] + m[1] * rot[1][1] + m[2] * rot[1][2];
	r[2] = m[0] * rot[2][0] + m[1] * rot[2][1] + m[2] * rot[2][2];
	r[3] = m[3];

	return r;
}

template<typename T>
inline t_mat<T, t_vec4, 4> lookAt(const t_vec3<T, 3>& eye, const t_vec3<T, 3>& target, const t_vec3<T, 3>& up)
{
	t_vec3<T, 3> zAxis(normalize(eye - target));
	t_vec3<T, 3> xAxis(normalize(cross(up, zAxis)));
	t_vec3<T, 3> yAxis(cross(zAxis, xAxis));

	t_mat<T, t_vec4, 4> r;
	r[0][0] = xAxis.x;
	r[0][1] = xAxis.y;
	r[0][2] = xAxis.z;
	r[1][0] = yAxis.x;
	r[1][1] = yAxis.y;
	r[1][2] = yAxis.z;
	r[2][0] = zAxis.x;
	r[2][1] = zAxis.y;
	r[2][2] = zAxis.z;

	t_mat<T, t_vec4, 4> translate;
	translate[3][0] = -eye.x;
	translate[3][1] = -eye.y;
	translate[3][2] = -eye.z;

	return translate * r;
}

template<typename T>
inline t_mat<T, t_vec4, 4> ortho(T left, T right, T bottom, T top, T near, T far)
{
	t_mat<T, t_vec4, 4> r;
	
	r[0][0] = static_cast<T>(2) / (right - left);
	r[1][1] = static_cast<T>(2) / (top - bottom);
	r[2][2] = static_cast<T>(2) / (far - near);

	r[3][0] = -(right + left) / (right - left);
	r[3][1] = -(top + bottom) / (top - bottom);
	r[3][2] = -(far + near) / (far - near);

	return r;
}

template<typename T>
inline t_mat<T, t_vec4, 4> perspective(T fovy, T aspect, T near, T far)
{
	t_mat<T, t_vec4, 4> r;
	r[0][0] = 1.0f / (aspect * tan(fovy * 0.5f));
	r[1][1] = 1.0f / tan(fovy * 0.5f);
	r[2][2] = -(far + near) / (far - near);
	r[2][3] = -1.0f;
	r[3][2] = -(2.0f * far * near) / (far - near);
	r[3][3] = 0.0f;

	return r;
}