#pragma once

#include "Test.h"

#include "Renderer.h"
#include "Shader.h"
#include "Texture.h"
#include "VertexBufferLayout.h"

#include <memory>

namespace test {
  class TestTexture2D : public Test
  {
  public:
    TestTexture2D();
    ~TestTexture2D();

    void OnUpdate(float deltaTime) override;
    void OnRender() override;
    void OnImGuiRender() override;

  private:
    glm::vec3 m_Translation;
    glm::mat4 m_Proj;

    std::unique_ptr<VertexArray> m_VArray;
    std::unique_ptr<VertexBuffer> m_VBuffer;
    std::unique_ptr<IndexBuffer> m_IBuffer;
    std::unique_ptr<Texture> m_Texture;
    std::unique_ptr<Shader> m_Shader;
  };
}