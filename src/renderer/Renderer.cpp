#include "Renderer.h"
#include <glad/glad.h>

Renderer::Renderer(int Particles, const char *shaderName) : maxParticles(Particles), shader(shaderName)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(float), nullptr,
                 GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
};

Renderer::~Renderer()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
};

void Renderer::update(const float *positions, int count)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * 3 * sizeof(float), positions);
};

void Renderer::draw(int count)
{
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, count);
}

void Renderer::draw_triangle(int count)
{
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
}

unsigned int Renderer::getVBO() const { return vbo; }

void Renderer::setMVP(const float *mat4)
{
    shader.use();
    shader.setUniform("uMVP", mat4);
}