#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
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

  if (glewInit() != GLEW_OK)
    std::cout << "GLEW Error\n";

  std::cout << glGetString(GL_VERSION) << "\n";

  /* key press event */
  glfwSetKeyCallback(window, key_callback);

  float positions[6] = {
    -0.5f, -0.5f,
     0.0f,  0.5f,
     0.5f, -0.5f
  };

  unsigned int buffer; // id of buffer
  glGenBuffers(1, &buffer); // generate memory in Video RAM
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  /* glBufferData是一个专门用来把用户定义的数据复制到当前绑定缓冲的函数 */
  glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

  std::string vertexShader =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) in vec4 position;\n"
    "\n"
    "void mian()\n"
    "{\n"
    "  gl_Position = position;\n"
    "}\n";

  std::string fragmentShader =
    "#version 330 core\n"
    "\n"
    "layout(location = 0) out vec4 color;\n"
    "\n"
    "void mian()\n"
    "{\n"
    "  color = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n";

  unsigned int shader = CreateShader(vertexShader, fragmentShader);
  glUseProgram(shader);

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glDrawArrays(GL_TRIANGLES, 0, 3);

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  glDeleteProgram(shader);

  glfwTerminate();
  return 0;
}