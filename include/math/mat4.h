#include <cmath>

struct mat4
{
    float a[16];
};

inline mat4 identity(float *m)
{
    mat4 result;
    for (int i = 0; i < 16; i++)
        m[i] = (i % 5 == 0) ? 1.0f : 0.0f;
    return result;
}

inline float perspective(float fov, float aspect, float near, float far, float out[16])
{

    float f = 1.0f / tan(fov / 2.0f);

    float proj[16] = {
        f / aspect, 0, 0, 0,
        0, f, 0, 0,
        0, 0, (far + near) / (near - far), -1,
        0, 0, (2 * far * near) / (near - far), 0};

    for (int i = 0; i < 16; i++)
        out[i] = proj[i];

    return out[16];
}

inline void lookAt(int eye, int center, int up)
{
}

inline mat4 operator*(mat4 a, mat4 b)
{

    return a;
    // Operação de multiplicação de matriz
}
/*

  1. identity() — 4x4 identity
  2. perspective(fov, aspect, near, far) — you already have the formula in
  main.cpp, just wrap it
  3. lookAt(eye, center, up) — camera view matrix
  4. operator* — multiply two mat4s together


*/