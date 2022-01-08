#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
}

struct ShaderSources
{
  std::string VertextSource;
  std::string FragmenttSource;
};

static ShaderSources ParseShader(const std::string& filepath)
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
      else if(line.find("fragment") != std::string::npos)
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

static unsigned int CompileShader(unsigned int type, const std::string& source) 
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

static int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) 
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

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1;


  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  //glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
    std::cout << "GLEW Error\n";

  std::cout << glGetString(GL_VERSION) << "\n";

  GLint nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

  /* key press event */
  glfwSetKeyCallback(window, key_callback);

  {
    float positions[] = {
      // position   // color
      -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0
       0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1
       0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // 2
      -0.5f,  0.5f, 1.0f, 1.0f, 1.0f  // 3
    };

    unsigned int indices[] = {
      0, 1, 2,
      0, 2, 3
    };

    VertexArray va;
    VertexBuffer vb(positions, 4 * 5 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(3);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    ShaderSources sources = ParseShader("resource/shaders/Basic.shader");

    unsigned int shader = CreateShader(sources.VertextSource, sources.FragmenttSource);

    //int location = glGetUniformLocation(shader, "u_Color");
    //glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f);

    //float r = 0.0f;
    //float increment = 0.05f;

    va.UnBind();

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT);

      glUseProgram(shader);
      va.Bind();
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
      va.UnBind();

      //if (r > 1.0f)
      //  increment = -0.05f;
      //else if (r < 0.0f)
      //  increment = 0.05f;

      //r += increment;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }

    glDeleteProgram(shader);
  }

  glfwTerminate();
  return 0;
}