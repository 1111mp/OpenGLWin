#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

struct ShaderSources
{
  std::string VertextSource;
  std::string FragmenttSource;
};

class Shader
{
private:
  std::string m_FilePath;
  unsigned int m_RendererID;
  mutable std::unordered_map<std::string, int> m_UniformLocationCache;

public:
  Shader(const std::string filepath);
  ~Shader();

  void Bind() const;
  void UnBind() const;

  void SetUniform1i(const std::string& name, int value);
  void SetUniformia(const std::string& name, int value[2]);
  void SetUniform1f(const std::string& name, float value);
  // set uniform
  void SetUniform4f(const std::string& name, float x, float y, float z, float w);
  void SetUniformMat4f(const std::string& name, const glm::mat4& matrix);
 
private:
  ShaderSources ParseShader(const std::string& filepath);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

  int GetUniformLocation(const std::string& name) const;
};