#ifndef __OPENGL_TUTORIAL_SHADER_H__
#define __OPENGL_TUTORIAL_SHADER_H__

#include "glad/glad.h"
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>

class Shader
{
public:
  // The program ID.
  unsigned int ID;

  // Constructor reads and builds the shader.
  Shader(const std::string& vertexPath, const std::string& fragmentPath);

  // Use/activate the shader.
  void use();

  // Utility uniform functions.
  void setBool(const std::string& name, bool value) const;
  void setInt(const std::string& name, int value) const;
  void setFloat(const std::string& name, float value) const;
};

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
{
  // 1. Retrieve the vertex and fragment shader source code from the files.
  std::string vertexCode, fragmentCode;
  std::ifstream vShaderFile, fShaderFile;
  vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
  fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

  try
  {
    vShaderFile.open(vertexPath);
    fShaderFile.open(fragmentPath);

    std::stringstream vShaderStream, fShaderStream;

    vShaderStream << vShaderFile.rdbuf();
    fShaderStream << fShaderFile.rdbuf();

    vShaderFile.close();
    fShaderFile.close();

    vertexCode = vShaderStream.str();
    fragmentCode = fShaderStream.str();
  }
  catch (std::ifstream::failure e)
  {
    std::cout << "Failed to read shader file: " << e.what() << std::endl;
    throw;
  }

  const char* vShaderCode = vertexCode.c_str();
  const char* fShaderCode = fragmentCode.c_str();

  // 2. Compile the shaders (and check for errors).
  unsigned int vertex, fragment;
  int success;
  char infoLog[512];

  vertex = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex, 1, &vShaderCode, NULL);
  glCompileShader(vertex);

  glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(vertex, 512, NULL, infoLog);
    std::cout << "Vertex shader compilation failed: " << infoLog << std::endl;
    throw std::exception();
  }

  fragment = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment, 1, &fShaderCode, NULL);
  glCompileShader(fragment);

  glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    glGetShaderInfoLog(fragment, 512, NULL, infoLog);
    std::cout << "Fragment shader compilation failed: " << infoLog << std::endl;
    throw std::exception();
  }

  // Finally compile the shader program.
  ID = glCreateProgram();
  glAttachShader(ID, vertex);
  glAttachShader(ID, fragment);
  glLinkProgram(ID);

  // Check for linking errors.
  glGetProgramiv(ID, GL_LINK_STATUS, &success);
  if (!success)
  {
    glGetProgramInfoLog(ID, 512, NULL, infoLog);
    std::cout << "Program linking failed: " << infoLog << std::endl;
    throw std::exception();
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
}

void Shader::use()
{
  glUseProgram(ID);
}

void Shader::setBool(const std::string& name, bool value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), int(value));
}

void Shader::setInt(const std::string& name, int value) const
{
  glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::setFloat(const std::string& name, float value) const
{
  glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

#endif // __OPENGL_TUTORIAL_SHADER_H__
