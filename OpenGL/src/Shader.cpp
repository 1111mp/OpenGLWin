#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

Shader::Shader(const std::string filepath)
  : m_FilePath(filepath), m_RendererID(0)
{
  ShaderSources sources = ParseShader(filepath);

  m_RendererID = CreateShader(sources.VertextSource, sources.FragmenttSource);
}

Shader::~Shader()
{
  GLCall(glDeleteProgram(m_RendererID));
}

void Shader::Bind() const
{
  GLCall(glUseProgram(m_RendererID));
}

void Shader::UnBind() const
{
  GLCall(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float x, float y, float z, float w)
{
  GLCall(glUniform4f(GetUniformLocation(name), x, y, z, w));
}

int Shader::GetUniformLocation(const std::string& name) const
{
  if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end())
    return m_UniformLocationCache[name];

  GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1)
    std::cout << "Warning: uniform '" << name << "' doesn't exist!\n";

  m_UniformLocationCache[name] = location;
  return location;
}

ShaderSources Shader::ParseShader(const std::string& filepath)
{
  std::ifstream stream(filepath);

  if (!stream.is_open())
    throw "File open failed.";

  enum class ShaderType
  {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string line;
  std::stringstream strs[2];
  ShaderType type = ShaderType::NONE;

  while (std::getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
        // set mode to vertex
        type = ShaderType::VERTEX;
      else if (line.find("fragment") != std::string::npos)
        // set mode to fragment
        type = ShaderType::FRAGMENT;
    }
    else
    {
      strs[static_cast<int>(type)] << line << '\n';
    }
  }

  stream.close();

  return { strs[0].str(), strs[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
  unsigned int id = glCreateShader(type);
  const char* src = source.c_str();

  glShaderSource(id, 1, &src, nullptr);
  glCompileShader(id);

  int result;
  glGetShaderiv(id, GL_COMPILE_STATUS, &result);

  if (result == GL_FALSE)
  {
    int length;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetShaderInfoLog(id, length, &length, message);

    std::cout << "ERROR::SHADER::" << (type == GL_VERTEX_SHADER ? "VERTEX" : "FRAGMENT") << "::COMPILATION_FAILED\n" << message << std::endl;

    glDeleteShader(id);

    return 0;
  }

  return id;
}

int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
{
  unsigned int program = glCreateProgram();

  unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexShader);
  unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

  glAttachShader(program, vertex);
  glAttachShader(program, fragment);
  glLinkProgram(program);
  glValidateProgram(program);

  glDeleteShader(vertex);
  glDeleteShader(fragment);

  int result;
  glGetProgramiv(program, GL_COMPILE_STATUS, &result);
  if (!result) {
    int length;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
    char* message = (char*)alloca(length * sizeof(char));
    glGetProgramInfoLog(program, length, &length, message);

    std::cout << "ERROR::SHADER::PROGRAM::COMPILATION_FAILED\n" << message << std::endl;

    glDeleteProgram(program);

    return 0;
  }

  return program;
}
