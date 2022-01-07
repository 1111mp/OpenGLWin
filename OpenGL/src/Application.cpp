#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

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
      strs[(int)type] << line << '\n';
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

  glfwSwapInterval(1);

  if (glewInit() != GLEW_OK)
    std::cout << "GLEW Error\n";

  std::cout << glGetString(GL_VERSION) << "\n";

  GLint nrAttributes;
  glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
  std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;

  /* key press event */
  glfwSetKeyCallback(window, key_callback);

  float positions[] = {
    -0.5f, -0.5f, // 0
     0.5f, -0.5f, // 1
     0.5f,  0.5f, // 2
    -0.5f,  0.5f, // 3
  };

  unsigned int indices[] = {
    0, 1, 2,
    0, 2, 3
  };

  unsigned int buffer; // id of buffer
  glGenBuffers(1, &buffer); // generate memory in Video RAM
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  /* glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数 */
  glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  unsigned int ibo; // id of ibo
  glGenBuffers(1, &ibo); // generate memory in Video RAM
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

  ShaderSources sources = ParseShader("resource/shaders/Basic.shader");

  unsigned int shader = CreateShader(sources.VertextSource, sources.FragmenttSource);
  glUseProgram(shader);

  int location = glGetUniformLocation(shader, "u_Color");

  float r = 0.0f;
  float increment = 0.05f;

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    glUniform4f(location, r, 0.3f, 0.8f, 1.0f);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    if (r > 1.0f)
      increment = -0.05f;
    else if (r < 0.0f)
      increment = 0.05f;

    r += increment;

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteProgram(shader);

  glfwTerminate();
  return 0;
}