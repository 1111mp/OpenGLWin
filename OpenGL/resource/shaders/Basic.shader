#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_Textcoord;

void main()
{
  gl_Position = position;
  v_Textcoord = texCoord;
};

#shader fragment
#version 330 core

in vec2 v_Textcoord;

out vec4 color;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform sampler2D u_Texture_other;

void main()
{
  color = texture(u_Texture, v_Textcoord) * u_Color;
};