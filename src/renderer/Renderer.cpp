#include "Renderer.h"
#include <glad/glad.h>
#include "vec3.h"

static const float Axis_Size = 4.0f;

// 3 eixos × 2 vértices × 6 floats (pos+cor) = 36 floats
static void buildAxisVerts(float out[3 * 2 * 6])
{
    float neg = -Axis_Size / 4.0f;

    // Cores dos vetores, RGB normalizado
    vec3 cX = {1, 0, 0}; // vermelho
    vec3 cY = {0, 1, 0}; // verde
    vec3 cZ = {0, 0, 1}; // azul

    // clang-format off
    float buf[] = {
        neg,       0,   0,  cX.x, cX.y, cX.z,  // X: neg → pos
        Axis_Size, 0,   0,  cX.x, cX.y, cX.z,
        0,   neg,        0, cY.x, cY.y, cY.z,  // Y: neg → pos
        0,   Axis_Size,  0, cY.x, cY.y, cY.z,
        0,   0,   neg,      cZ.x, cZ.y, cZ.z,  // Z: neg → pos
        0,   0,   Axis_Size,cZ.x, cZ.y, cZ.z,
    };
    // clang-format on
    for (int i = 0; i < 36; i++)
        out[i] = buf[i];
}

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
    float axisVerts[3 * 2 * 6];
    buildAxisVerts(axisVerts);

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