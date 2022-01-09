#pragma once

#include <string>
#include <unordered_map>

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

  // set uniform
  void SetUniform4f(const std::string& name, float x, float y, float z, float w);
 
private:
  ShaderSources ParseShader(const std::string& filepath);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);

  int GetUniformLocation(const std::string& name) const;
};