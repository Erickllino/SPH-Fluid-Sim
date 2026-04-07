#include "Renderer.h"
#include <glad/glad.h>

// pos(xyz) + cor(rgb) para cada extremidade dos 3 eixos
static const float axisVerts[] = {
    // X - vermelho
    0,0,0,  1,0,0,
    1,0,0,  1,0,0,
    // Y - verde
    0,0,0,  0,1,0,
    0,1,0,  0,1,0,
    // Z - azul
    0,0,0,  0,0,1,
    0,0,1,  0,0,1,
};

Renderer::Renderer(int Particles, const char *shaderName)
    : maxParticles(Particles), shader(shaderName), axisShader("axis")
{
    // VAO principal
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // VAO dos eixos
    glGenVertexArrays(1, &axisVAO);
    glBindVertexArray(axisVAO);
    glGenBuffers(1, &axisVBO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVerts), axisVerts, GL_STATIC_DRAW);
    // attrib 0: posição
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // attrib 1: cor
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
};

Renderer::~Renderer()
{
    glDeleteBuffers(1, &vbo);
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &axisVBO);
    glDeleteVertexArrays(1, &axisVAO);
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

void Renderer::drawAxes(const float *mvp)
{
    axisShader.use();
    axisShader.setUniform("uMVP", mvp);
    glBindVertexArray(axisVAO);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
}

void Renderer::setMVP(const float *mat4)
{
    shader.use();
    shader.setUniform("uMVP", mat4);
}