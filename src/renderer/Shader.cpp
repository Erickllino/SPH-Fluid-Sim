/*
The .cpp implementation does:
  1. Read the file at vertPath into a string
  2. glCreateShader(GL_VERTEX_SHADER) → glShaderSource → glCompileShader
  3. Same for fragment
  4. glCreateProgram() → glAttachShader x2 → glLinkProgram
  5. Check errors at each step with glGetShaderiv(shader, GL_COMPILE_STATUS,
  &ok)
  6. Store the result in programID

  use() is just glUseProgram(programID).

Do not skip error checking — shader compile errors are silent unless you
  explicitly query and print them with glGetShaderInfoLog.




*/

#include "Shader.h"
#include <fstream>
#include <sstream>
#include <cstdio>

Shader::Shader(const char *filename)
{

  std::string vertPath = std::string("shaders/") + filename + "/" + filename + ".vert";
  std::string fragPath = std::string("shaders/") + filename + "/" + filename + ".frag";

  std::ifstream vert(vertPath);
  std::ifstream frag(fragPath);
  std::stringstream vertBuffer;

  vertBuffer << vert.rdbuf();
  std::string vertCode = vertBuffer.str();
  const char *vt = vertCode.c_str();

  std::stringstream fragBuffer;
  fragBuffer << frag.rdbuf();
  std::string fragCode = fragBuffer.str();
  const char *fg = fragCode.c_str();

  unsigned int vertShader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertShader, 1, &vt, nullptr);
  glCompileShader(vertShader);

  unsigned int fragShader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragShader, 1, &fg, nullptr);
  glCompileShader(fragShader);

  int vert_ok, frag_ok;
  glGetShaderiv(vertShader, GL_COMPILE_STATUS, &vert_ok);
  glGetShaderiv(fragShader, GL_COMPILE_STATUS, &frag_ok);
  if (!vert_ok || !frag_ok)
  {
    char log[512];
    glGetShaderInfoLog(vertShader, 512, nullptr, log);
    printf("Erro no shader: %s\n", log);
  }

  unsigned int program = glCreateProgram();
  glAttachShader(program, vertShader);
  glAttachShader(program, fragShader);
  glLinkProgram(program);
  ShaderID = program;
  // shaders individuais não são mais necessários
  glDeleteShader(vertShader);
  glDeleteShader(fragShader);
};

/*
Shader::Shader(const char *filename, ShaderType type)
{
  // Open shader file
  std::string fileData = "";
  std::fstream stream(filename, std::ios::in);
  if (!stream.is_open())
  {
    printf("ERROR: Can't open shader file '%s'\n", filename);
    ShaderID = 0;
    return;
  }

  // Read shader file
  std::string line = "";
  while (getline(stream, line))
  {
    fileData += "\n" + line;
  }

  // Close file
  stream.close();

  // Append header & create shader
  std::string shaderSource;
  switch (type)
  {
  case eGeometry:
    shaderSource = "#define GEOMETRY_SHADER\n" + fileData;
    ShaderID = glCreateShader(GL_GEOMETRY_SHADER);
    break;
  case eVertex:
    shaderSource = "#define VERTEX_SHADER\n" + fileData;
    ShaderID = glCreateShader(GL_VERTEX_SHADER);
    break;
  case eFragment:
    shaderSource = "#define FRAGMENT_SHADER\n" + fileData;
    ShaderID = glCreateShader(GL_FRAGMENT_SHADER);
    break;
  case eCompute:
    shaderSource = "#define COMPUTE_SHADER\n" + fileData;
    ShaderID = glCreateShader(GL_COMPUTE_SHADER);
    break;
  }
  shaderSource = "#version 430\n" + shaderSource;

  // Compile shader
  const char *rawShaderSource = shaderSource.c_str();
  glShaderSource(ShaderID, 1, &rawShaderSource, NULL);
  glCompileShader(ShaderID);

  // Verify compile worked
  GLint isCompiled = 0;
  glGetShaderiv(ShaderID, GL_COMPILE_STATUS, &isCompiled);
  if (isCompiled == GL_FALSE)
  {
    printf("ERROR: Can't compile shader '%s'\n", filename);

    // Print error message
    GLint maxLength = 0;
    glGetShaderiv(ShaderID, GL_INFO_LOG_LENGTH, &maxLength);
    std::vector<GLchar> errorLog(maxLength);
    glGetShaderInfoLog(ShaderID, maxLength, &maxLength, &errorLog[0]);
    printf("GL ERRORS:\n%s\n", &errorLog[0]);

    // Delete shader
    glDeleteShader(ShaderID);
    ShaderID = 0;
  }
}
*/

Shader::~Shader()
{
  glDeleteProgram(ShaderID);
}

void Shader::setUniform(const char *name, const float *mat4) const
{
  // 1. acha onde está "uMVP" no shader
  int location = glGetUniformLocation(ShaderID, name);

  // 2. manda a matriz (16 floats)
  glUniformMatrix4fv(location, 1, GL_FALSE, mat4);
};

void Shader::use() const
{
  glUseProgram(ShaderID);
};