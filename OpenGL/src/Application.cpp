#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"

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
      // position   // texture
      -0.5f, -0.5f, 0.0f, 0.0f,
       0.5f, -0.5f, 1.0f, 0.0f,
       0.5f,  0.5f, 1.0f, 1.0f,
      -0.5f,  0.5f, 0.0f, 1.0f
    };

    unsigned int indices[] = {
      0, 1, 2,
      0, 2, 3
    };

    VertexArray va;
    VertexBuffer vb(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;
    layout.Push<float>(2);
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 6);

    Shader shader("resource/shaders/Basic.shader");
    shader.Bind();
    shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

    float r = 0.0f;
    float increment = 0.05f;

    Texture texture("resource/textures/container.jpg");
    //Texture texture_other("resource/textures/ChernoLogo.png");
    texture.Bind();
    //texture_other.Bind(1);
    shader.SetUniform1i("u_Texture", 0);
    //shader.SetUniform1i("u_Texture_other", 1);

    va.UnBind();
    vb.UnBind();
    ib.UnBind();
    shader.UnBind();

    Renderer renderer;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      //glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
      renderer.Clear();

      shader.Bind();
      shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);

      renderer.Draw(va, ib);

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

  }

  glfwTerminate();
  return 0;
}