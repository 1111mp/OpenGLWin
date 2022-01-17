#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec2 texCoord;
layout(location = 3) in float texIndex;

out vec4 v_color;
out vec2 v_TextCoord;
out float v_TexIndex;

uniform mat4 u_Transform;

void main()
{
  gl_Position = u_Transform * position;
  v_color = color;
  v_TextCoord = texCoord;
  v_TexIndex = texIndex;
}

#shader fragment
#version 330 core

in vec4 v_color;
in vec2 v_TextCoord;
in float v_TexIndex;

out vec4 color;

uniform sampler2D u_Textures[2];

void main()
{
  int index = int(v_TexIndex);
  color = texture(u_Textures[index], v_TextCoord);
}