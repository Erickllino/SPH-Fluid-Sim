#pragma once

#include <glad/glad.h>

class Shader
{
private:
    unsigned int ShaderID;
    Shader() : ShaderID(0) {}
    friend class Renderer;

public:
    Shader(const char *filename);
    ~Shader();

    void setUniform(const char *name, const float *mat4) const;
    void setUniformf(const char *name, float v) const;

    void use() const;
};
