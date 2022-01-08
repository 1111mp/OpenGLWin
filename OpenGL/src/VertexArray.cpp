#include "VertexArray.h"

#include "Renderer.h"

VertexArray::VertexArray()
{
  GLCall(glGenVertexArrays(1, &m_RendererID));
  GLCall(glBindVertexArray(m_RendererID));
}

VertexArray::~VertexArray()
{
  GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
  vb.Bind();

  const auto& elements = layout.GetElements();
  unsigned int offset = 0;

  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto& element = elements[i];

    GLCall(glEnableVertexAttribArray(i));
    GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)offset));

    offset += element.count * VertexBufferElement::GetSizeofType(element.type);
  }

  //glEnableVertexAttribArray(0);
  //glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

  //glEnableVertexAttribArray(1);
  //glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(2 * sizeof(float)));
}

void VertexArray::Bind() const
{
  GLCall(glBindVertexArray(m_RendererID));
}

void VertexArray::UnBind() const
{
  GLCall(glBindVertexArray(0));
}
