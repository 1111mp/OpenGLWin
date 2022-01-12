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

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "test/TestClearColor.h"

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
	window = glfwCreateWindow(960, 600, "Hello World", NULL, NULL);
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
		// 立方体
		//float positions[] = {
		//	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // 0
		//	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // 1
		//	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 2
  //    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // 3
		//	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 4
		//	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // 5
		//	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // 6
		//	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // 7
		//	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 8
		//	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // 9
		//	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 10
		//	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // 11
		//	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // 12
		//	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // 13
		//	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // 14
		//	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // 15
		//};

	/*	unsigned int indices[] = {
			0,   1,  2,  2,  3,  0,
			4,   5,  6,  6,  7,  4,
			8,   9, 10, 10,  4,  8,
			11,  2, 12, 12, 13, 11,
			10, 14,  5,  5,  4, 10,
			 3,  2, 11, 11, 15,  3
		};*/

		float positions[] = {
			// position   // texture
				0.0f,		0.0f, 0.0f, 0.0f,
      100.0f,		0.0f, 1.0f, 0.0f,
			100.0f, 100.0f, 1.0f, 1.0f,
				0.0f, 100.0f, 0.0f, 1.0f
		};

		unsigned int indices[] = {
			0, 1, 2,
			0, 2, 3
    };

		glEnable(GL_DEPTH_TEST);

		// 混合 https://learnopengl-cn.readthedocs.io/zh/latest/04%20Advanced%20OpenGL/03%20Blending/
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		VertexArray va;
		VertexBuffer vb(positions, 4 * 4 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		Shader shader("resource/shaders/Basic.shader");
		shader.Bind();
		
		Texture texture("resource/textures/ChernoLogo.png");
    //Texture texture_other("resource/textures/container.jpg");
		texture.Bind();
		//texture_other.Bind(1);
		shader.SetUniform1i("u_Texture", 0);
		//shader.SetUniform1i("u_Texture_other", 1);

		va.UnBind();
		vb.UnBind();
		ib.UnBind();
		shader.UnBind();

		Renderer renderer;

		const char* glsl_version = "#version 130";

		IMGUI_CHECKVERSION();
		ImGui::CreateContext();

		ImGui::StyleColorsDark();

		ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

		glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 600.0f, -1.0f, 1.0f);
    //glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-100.0f, -100.0f, 0.0f));

    glm::vec3 translationA(200.0f, 200.0f, 0.0f);
    glm::vec3 translationB(0.0f, 0.0f, 0.0f);

		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			//glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			renderer.Clear();

			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			{
				glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA);
				glm::mat4 trans = proj * model;

				shader.Bind();
				shader.SetUniformMat4f("u_Transform", trans);

        renderer.Draw(va, ib);
			}

			{
				glm::mat4 model_other = glm::translate(glm::mat4(1.0f), translationB);
				glm::mat4 trans_other = proj * model_other;

				shader.Bind();
				shader.SetUniformMat4f("u_Transform", trans_other);

        renderer.Draw(va, ib);
			}

      {
        ImGui::Begin("Debug");
				ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::SliderFloat3("Translation A", &translationA.x, 0.0f, 960.0f);
        ImGui::SliderFloat3("Translation B", &translationB.x, 0.0f, 960.0f);
				ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
				ImGui::End();
			}

			ImGui::Render();
      ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);

			/* Poll for and process events */
			glfwPollEvents();
		}

	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwTerminate();
	return 0;
}