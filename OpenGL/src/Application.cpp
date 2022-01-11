#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) 
{
  if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    glfwSetWindowShouldClose(window, GL_TRUE);
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

  {
    //float positions[] = {
    //  // position   // color
    //  -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, // 0
    //   0.5f, -0.5f, 0.0f, 1.0f, 0.0f, // 1
    //   0.5f,  0.5f, 0.0f, 0.0f, 1.0f, // 2
    //  -0.5f,  0.5f, 1.0f, 1.0f, 1.0f  // 3
    //};
    float positions[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 1
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
       //0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3
      //-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
       //0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 7
      //-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 8
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 9
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 10
      //-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      //-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      //-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 11
       //0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 12
       //0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 13
       //0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      //-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 14
       //0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       //0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      //-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      //-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      //-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       //0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       //0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       //0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // 15
      //-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    unsigned int indices[] = {
      0,   1,  2,  2,  3,  0,
      4,   5,  6,  6,  7,  4,
      8,   9, 10, 10,  4,  8,
      11,  2, 12, 12, 13, 11,
      10, 14,  5,  5,  4, 10,
       3,  2, 11, 11, 15,  3
    };

    glEnable(GL_DEPTH_TEST);

    //glm::mat4 trans;
    // ͸��ͶӰ
    
    //trans = glm::rotate(trans, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));

    //trans = glm::scale(trans, glm::vec3(0.5f, 0.5f, 0.5f));

    // ��� https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/03%20Blending/
    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    VertexArray va;
    VertexBuffer vb(positions, 36 * 5 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(3);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 36);

    Shader shader("resource/shaders/Basic.shader");
    shader.Bind();
    //shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);
    //shader.SetUniformMat4f("u_Transform", trans);

    //float r = 0.0f;
    //float increment = 0.05f;

    Texture texture("resource/textures/container.jpg");
    Texture texture_other("resource/textures/ChernoLogo.png");
    texture.Bind();
    texture_other.Bind(1);
    shader.SetUniform1i("u_Texture", 0);
    shader.SetUniform1i("u_Texture_other", 1);

    va.UnBind();
    vb.UnBind();
    ib.UnBind();
    shader.UnBind();

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      renderer.Clear();

      shader.Bind();
      glm::mat4 trans = glm::perspective(45.0f, (640.0f / 480.0f), 0.1f, 100.0f);
      trans = glm::translate(trans, glm::vec3(0.0f, 0.0f, -3.0f));
      trans = glm::rotate(trans, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
      //shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
      trans = glm::rotate(trans, glm::radians((GLfloat)glfwGetTime() * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
      shader.SetUniformMat4f("u_Transform", trans);

      renderer.Draw(va, ib);

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

  }

  glfwTerminate();
  return 0;
}