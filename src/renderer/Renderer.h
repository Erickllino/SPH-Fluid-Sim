#pragma once

#include "Shader.h"
#include <glad/glad.h>

class Renderer
{
private:
    unsigned int vao;
    unsigned int vbo;
    int maxParticles;
    Shader shader; // or Shader* if you prefer pointer

public:
    Renderer(int maxParticles, const char *shaderName);
    ~Renderer(); // glDelete* calls go here

    void update(const float *positions, int count); // CPU path
    void draw(int count);
    void draw_triangle(int count);
    void setMVP(const float *mat4);

    unsigned int getVBO() const; // CUDA interop later
};