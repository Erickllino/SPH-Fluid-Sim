#pragma once

#ifdef CUDA_AVAILABLE
#include <cuda_runtime.h>
#define HOST_DEVICE __host__ __device__
#else
#define HOST_DEVICE // vira nada quando não tem CUDA
#endif

#include <cmath>

// ─────────────────────────────────────────────────────────────────────────────
// vec3 — CUDA-compatible float3 wrapper
//
// All methods annotated HOST_DEVICE so they work in both CPU code
// (SPHSystem init, boundary checks) and GPU kernels (density, forces, integrate).
//
// Design notes:
//   - Wraps float3 so it stays ABI-compatible with CUDA built-ins.
//   - No virtual functions, no heap allocation — safe inside CUDA structs.
//   - Includes the SPH-specific helpers (length2, normalize_safe) you will
//     need frequently in the Poly6 / Spiky / Viscosity kernels.
// ─────────────────────────────────────────────────────────────────────────────

struct vec3
{
    float x, y, z;

    // ── Constructors ──────────────────────────────────────────────────────────
    HOST_DEVICE vec3() : x(0.f), y(0.f), z(0.f) {}
    HOST_DEVICE explicit vec3(float s) : x(s), y(s), z(s) {}
    HOST_DEVICE vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    // Interop with CUDA built-in float3
    HOST_DEVICE vec3(float3 v) : x(v.x), y(v.y), z(v.z) {}
    HOST_DEVICE operator float3() const { return make_float3(x, y, z); }

    // ── Arithmetic (vector OP vector) ─────────────────────────────────────────
    HOST_DEVICE vec3 operator+(const vec3 &b) const { return {x + b.x, y + b.y, z + b.z}; }
    HOST_DEVICE vec3 operator-(const vec3 &b) const { return {x - b.x, y - b.y, z - b.z}; }
    HOST_DEVICE vec3 operator*(const vec3 &b) const { return {x * b.x, y * b.y, z * b.z}; }
    HOST_DEVICE vec3 operator/(const vec3 &b) const { return {x / b.x, y / b.y, z / b.z}; }

    // ── Arithmetic (vector OP scalar) ─────────────────────────────────────────
    HOST_DEVICE vec3 operator+(float s) const { return {x + s, y + s, z + s}; }
    HOST_DEVICE vec3 operator-(float s) const { return {x - s, y - s, z - s}; }
    HOST_DEVICE vec3 operator*(float s) const { return {x * s, y * s, z * s}; }
    HOST_DEVICE vec3 operator/(float s) const
    {
        float inv = 1.f / s;
        return {x * inv, y * inv, z * inv};
    }
    HOST_DEVICE vec3 operator-() const { return {-x, -y, -z}; }

    // ── Compound assignment ───────────────────────────────────────────────────
    HOST_DEVICE vec3 &operator+=(const vec3 &b)
    {
        x += b.x;
        y += b.y;
        z += b.z;
        return *this;
    }
    HOST_DEVICE vec3 &operator-=(const vec3 &b)
    {
        x -= b.x;
        y -= b.y;
        z -= b.z;
        return *this;
    }
    HOST_DEVICE vec3 &operator*=(float s)
    {
        x *= s;
        y *= s;
        z *= s;
        return *this;
    }
    HOST_DEVICE vec3 &operator/=(float s)
    {
        float inv = 1.f / s;
        x *= inv;
        y *= inv;
        z *= inv;
        return *this;
    }

    // ── Length / distance ─────────────────────────────────────────────────────
    HOST_DEVICE float length2() const { return x * x + y * y + z * z; }
    HOST_DEVICE float length() const { return sqrtf(length2()); }

    // ── Normalize ─────────────────────────────────────────────────────────────
    HOST_DEVICE vec3 normalized() const
    {
        float len = length();
        return (len > 1e-8f) ? (*this / len) : vec3(0.f);
    }

    // Safe normalize — returns fallback if near-zero (avoids NaN in kernels)
    HOST_DEVICE vec3 normalized_safe(vec3 fallback = vec3(0.f)) const
    {
        float len = length();
        return (len > 1e-8f) ? (*this / len) : fallback;
    }
};

// ── Free functions (scalar OP vector, dot, cross, etc.) ───────────────────────

HOST_DEVICE inline vec3 operator*(float s, const vec3 &v) { return v * s; }
HOST_DEVICE inline vec3 operator+(float s, const vec3 &v) { return v + s; }

HOST_DEVICE inline float dot(const vec3 &a, const vec3 &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

HOST_DEVICE inline vec3 cross(const vec3 &a, const vec3 &b)
{
    return {a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x};
}

HOST_DEVICE inline float distance(const vec3 &a, const vec3 &b)
{
    return (a - b).length();
}

HOST_DEVICE inline float distance2(const vec3 &a, const vec3 &b)
{
    return (a - b).length2();
}

HOST_DEVICE inline vec3 normalize(const vec3 &v)
{
    return v.normalized();
}

// Component-wise clamp — used in AABB boundary response
HOST_DEVICE inline vec3 clamp(const vec3 &v, const vec3 &lo, const vec3 &hi)
{
    return {fminf(fmaxf(v.x, lo.x), hi.x),
            fminf(fmaxf(v.y, lo.y), hi.y),
            fminf(fmaxf(v.z, lo.z), hi.z)};
}

// Component-wise abs — occasionally useful in boundary / debug code
HOST_DEVICE inline vec3 abs3(const vec3 &v)
{
    return {fabsf(v.x), fabsf(v.y), fabsf(v.z)};
}

// Linear interpolation
HOST_DEVICE inline vec3 lerp(const vec3 &a, const vec3 &b, float t)
{
    return a + (b - a) * t;
}

// Reflect v about normal n (for boundary bounce)
// n must be normalized
HOST_DEVICE inline vec3 reflect(const vec3 &v, const vec3 &n)
{
    return v - 2.f * dot(v, n) * n;
}