#include "TestTexture2D.h"

#include "imgui/imgui.h"

namespace test {

  TestTexture2D::TestTexture2D()
    : m_Translation(200.0f, 200.0f, 0.0f), m_Proj(glm::ortho(0.0f, 960.0f, 0.0f, 600.0f, -1.0f, 1.0f))
  {
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

    GLCall(glEnable(GL_BLEND));
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

    m_Shader = std::make_unique<Shader>("resource/shaders/Basic.shader");
    m_VArray = std::make_unique<VertexArray>();
    m_VBuffer = std::make_unique<VertexBuffer>(positions, 4 * 4 * sizeof(float));
    VertexBufferLayout layout;

    layout.Push<float>(2);
    layout.Push<float>(2);

    m_VArray->AddBuffer(*m_VBuffer, layout);

    m_IBuffer = std::make_unique<IndexBuffer>(indices, 6);

    m_Shader->Bind();

    m_Texture = std::make_unique<Texture>("resource/textures/ChernoLogo.png");
    m_Texture->Bind();

    m_Shader->SetUniform1i("u_Texture", 0);
  }

  TestTexture2D::~TestTexture2D()
  {
  }

  void TestTexture2D::OnUpdate(float deltaTime)
  {
  }

  void TestTexture2D::OnRender()
  {
    GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    Renderer renderer;

    m_Texture->Bind();

    {
      glm::mat4 model = glm::translate(glm::mat4(1.0f), m_Translation);
      glm::mat4 trans = m_Proj * model;

      m_Shader->Bind();
      m_Shader->SetUniformMat4f("u_Transform", trans);

      renderer.Draw(*m_VArray, *m_IBuffer);
    }

  }

  void TestTexture2D::OnImGuiRender()
  {
    ImGui::SliderFloat3("Translation", &m_Translation.x, 0.0f, 960.0f);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
  }
}