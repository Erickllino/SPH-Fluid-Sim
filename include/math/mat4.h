#pragma once

#include <cmath>
#include "vec3.h"

// ─────────────────────────────────────────────────────────────────────────────
// vec4 — vetor homogêneo, par natural do mat4
//
//   w = 1  →  ponto    (translação é aplicada)
//   w = 0  →  direção  (translação é ignorada)
// ─────────────────────────────────────────────────────────────────────────────
struct vec4
{
    float x, y, z, w;

    vec4() : x(0.f), y(0.f), z(0.f), w(0.f) {}
    vec4(float x, float y, float z, float w) : x(x), y(y), z(z), w(w) {}

    // Constrói a partir de vec3 explicitando w
    vec4(const vec3 &v, float w) : x(v.x), y(v.y), z(v.z), w(w) {}

    // Descarta w — útil após perspectiva (lembre de dividir por w antes)
    vec3 xyz() const { return {x, y, z}; }
};

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

    // mat4 × vec4  (multiplicação completa em coordenadas homogêneas)
    vec4 operator*(const vec4 &v) const
    {
        return {
            a[0][0]*v.x + a[0][1]*v.y + a[0][2]*v.z + a[0][3]*v.w,
            a[1][0]*v.x + a[1][1]*v.y + a[1][2]*v.z + a[1][3]*v.w,
            a[2][0]*v.x + a[2][1]*v.y + a[2][2]*v.z + a[2][3]*v.w,
            a[3][0]*v.x + a[3][1]*v.y + a[3][2]*v.z + a[3][3]*v.w,
        };
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

// ─────────────────────────────────────────────────────────────────────────────
// Helpers de conveniência: evitam lidar com vec4 e w na maioria dos casos
// ─────────────────────────────────────────────────────────────────────────────

// Transforma um PONTO (w=1): translação + rotação + escala são aplicadas.
// Se a matriz for projetiva, faz a divisão perspectiva por w.
inline vec3 transformPoint(const mat4 &m, const vec3 &p)
{
    vec4 r = m * vec4(p, 1.0f);
    if (r.w != 0.f && r.w != 1.f)
        return r.xyz() / r.w;   // divisão perspectiva
    return r.xyz();
}

// Transforma uma DIREÇÃO (w=0): translação é ignorada, só rotação/escala.
inline vec3 transformDir(const mat4 &m, const vec3 &d)
{
    return (m * vec4(d, 0.0f)).xyz();
}