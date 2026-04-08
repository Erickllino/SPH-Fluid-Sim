#pragma once

#include "Shader.h"
#include <glad/glad.h>

// class Renderer
// {
// private:
//     unsigned int vao;
//     unsigned int vbo;
//     int maxParticles;
//     Shader shader;

//     unsigned int axisVAO;
//     unsigned int axisVBO;
//     Shader axisShader;

//     Shader particleShader;

// public:
//     Renderer(int maxParticles, const char *shaderName);
//     ~Renderer(); // glDelete* calls go here

//     void update(const float *positions, int count); // CPU path
//     void draw(int count);
//     void draw_triangle(int count);
//     void drawAxes(const float *mvp);
//     void setMVP(const float *mat4);

//     // Sphere-impostor particle rendering
//     // view and proj are separate mat4 pointers; radius in world units; viewportH in pixels
//     void setViewProj(const float *view, const float *proj);
//     void drawParticles(int count, float radius, float viewportH);

//     unsigned int getVBO() const; // CUDA interop later
// };

class Renderer
{
protected:
    unsigned int vao;
    unsigned int vbo;
    Shader shader;

public:
    Renderer(const char *shaderName);
    ~Renderer(); // glDelete* calls go here

    void update(const float *positions, int count); // CPU path
    void draw(int count);
    void setMVP(const float *mat4);

    // Sphere-impostor particle rendering
    // view and proj are separate mat4 pointers; radius in world units; viewportH in pixels

    unsigned int getVBO() const; // CUDA interop later
};

class ParticleRenderer : public Renderer
{
private:
    int maxParticles;

public:
    ParticleRenderer(int maxParticles, const char *shaderName);
    ~ParticleRenderer();
    void drawParticles(int count, float radius, float viewportH);
    void setViewProj(const float *view, const float *proj);
};

class EnvRenderer : public Renderer
{
private:
public:
    EnvRenderer(const char *shaderName);
    ~EnvRenderer();
};

class DebugRenderer : public Renderer
{
private:
public:
    DebugRenderer(const char *shaderName);
    ~DebugRenderer();
    void drawAxes(const float *mvp);
};

class ShapeDebugRenderer : public Renderer
{
private:
public:
    ShapeDebugRenderer(const char *shaderName);
    ~ShapeDebugRenderer();
    void draw_triangle(int count);
};