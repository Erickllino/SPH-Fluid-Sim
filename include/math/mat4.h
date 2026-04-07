#include <cmath>
#include "vec3.h"

struct mat4
{
    float a[4][4];

    operator const float *() const { return &a[0][0]; }
    operator float *() { return &a[0][0]; }

    mat4 &operator=(const mat4 &b)
    {
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
                a[i][j] = b.a[i][j];
        return *this;
    }

    mat4 operator*(const mat4 &b) const
    {
        mat4 result;
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 4; j++)
            {
                result.a[i][j] = 0.0f;
                for (int k = 0; k < 4; k++)
                    result.a[i][j] += a[i][k] * b.a[k][j];
            }
        return result;
    }
};

inline mat4 identity(mat4 &m)
{
    mat4 result;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            result.a[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
    return m = result;
}

inline mat4 perspective(float fov, float aspect, float near, float far, mat4 out)
{

    float f = 1.0f / tan(fov / 2.0f);

    mat4 proj = {{{f / aspect, 0, 0, 0},
                  {0, f, 0, 0},
                  {0, 0, (far + near) / (near - far), -1},
                  {0, 0, (2 * far * near) / (near - far), 0}}};

    out = proj;
    return out;
}

inline mat4 lookAt(vec3 eye, vec3 center, vec3 up)
{
    vec3 f = (center - eye).normalized(); // frente
    vec3 r = cross(f, up).normalized();   // direita
    vec3 u = cross(r, f);                 // up real

    mat4 result = {{{r.x,            u.x,           -f.x,          0},
                    {r.y,            u.y,           -f.y,          0},
                    {r.z,            u.z,           -f.z,          0},
                    {-dot(r, eye),  -dot(u, eye),   dot(f, eye),   1}}};
    return result;
}

/*

  1. identity() — 4x4 identity
  2. perspective(fov, aspect, near, far) — you already have the formula in
  main.cpp, just wrap it
  3. lookAt(eye, center, up) — camera view matrix
  4. operator* — multiply two mat4s together


*/