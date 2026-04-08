#include "Renderer.h"
#include <glad/glad.h>
#include "vec3.h"

static const float Axis_Size = 4.0f;

static void buildAxisVerts(float out[3 * 2 * 6])
{
    float neg = -Axis_Size / 4.0f;

    vec3 cX = {1, 0, 0};
    vec3 cY = {0, 1, 0};
    vec3 cZ = {0, 0, 1};

    // clang-format off
    float buf[] = {
        neg,       0,         0,  cX.x, cX.y, cX.z,
        Axis_Size, 0,         0,  cX.x, cX.y, cX.z,
        0,         neg,       0,  cY.x, cY.y, cY.z,
        0,         Axis_Size, 0,  cY.x, cY.y, cY.z,
        0,         0,         neg,         cZ.x, cZ.y, cZ.z,
        0,         0,         Axis_Size,   cZ.x, cZ.y, cZ.z,
    };
    // clang-format on
    for (int i = 0; i < 36; i++)
        out[i] = buf[i];
}

// ─── Base ────────────────────────────────────────────────────────────────────

Renderer::Renderer(const char *shaderName) : shader(shaderName), vao(0), vbo(0) {}

Renderer::~Renderer()
{
    if (vbo) glDeleteBuffers(1, &vbo);
    if (vao) glDeleteVertexArrays(1, &vao);
}

void Renderer::update(const float *positions, int count)
{
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferSubData(GL_ARRAY_BUFFER, 0, count * 3 * sizeof(float), positions);
}

void Renderer::draw(int count)
{
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, count);
}

void Renderer::setMVP(const float *mat4)
{
    shader.use();
    shader.setUniform("uMVP", mat4);
}

unsigned int Renderer::getVBO() const { return vbo; }

// ─── ParticleRenderer ────────────────────────────────────────────────────────

ParticleRenderer::ParticleRenderer(int maxParticles, const char *shaderName)
    : Renderer(shaderName), maxParticles(maxParticles)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, maxParticles * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

ParticleRenderer::~ParticleRenderer() {}

void ParticleRenderer::setViewProj(const float *view, const float *proj)
{
    shader.use();
    shader.setUniform("uView", view);
    shader.setUniform("uProj", proj);
}

void ParticleRenderer::drawParticles(int count, float radius, float viewportH)
{
    shader.use();
    shader.setUniformf("uRadius", radius);
    shader.setUniformf("uViewportH", viewportH);
    glBindVertexArray(vao);
    glDrawArrays(GL_POINTS, 0, count);
    glBindVertexArray(0);
}

// ─── DebugRenderer ───────────────────────────────────────────────────────────

DebugRenderer::DebugRenderer(const char *shaderName) : Renderer(shaderName)
{
    float axisVerts[3 * 2 * 6];
    buildAxisVerts(axisVerts);

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(axisVerts), axisVerts, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glBindVertexArray(0);
}

DebugRenderer::~DebugRenderer() {}

void DebugRenderer::drawAxes(const float *mvp)
{
    shader.use();
    shader.setUniform("uMVP", mvp);
    glBindVertexArray(vao);
    glDrawArrays(GL_LINES, 0, 6);
    glBindVertexArray(0);
}

// ─── ShapeDebugRenderer ──────────────────────────────────────────────────────

ShapeDebugRenderer::ShapeDebugRenderer(const char *shaderName) : Renderer(shaderName)
{
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 1000 * 3 * sizeof(float), nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glBindVertexArray(0);
}

ShapeDebugRenderer::~ShapeDebugRenderer() {}

void ShapeDebugRenderer::draw_triangle(int count)
{
    shader.use();
    glBindVertexArray(vao);
    glDrawArrays(GL_TRIANGLES, 0, count);
    glBindVertexArray(0);
}

// ─── EnvRenderer ─────────────────────────────────────────────────────────────

EnvRenderer::EnvRenderer(const char *shaderName) : Renderer(shaderName) {}

EnvRenderer::~EnvRenderer() {}
