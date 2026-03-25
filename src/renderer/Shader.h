#pragma once

#include <glad/glad.h>

class Shader
{
private:
    unsigned int ShaderID;

public:
    Shader(const char *filename);
    ~Shader();

    void setUniform(const char *name, const float *mat4) const;

    void use() const;
    // later: setUniform helpers for mat4, vec3, float
};
