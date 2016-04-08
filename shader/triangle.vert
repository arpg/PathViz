#version 330

uniform mat4 M;

layout(location = 0) in vec3 in_position;
layout(location = 1) in vec3 in_texCoord1;
layout(location = 2) in vec3 in_texCoord2;

out vec3 vs_texCoord1;
out vec3 vs_texCoord2;

void main()
{
  vec4 position = vec4(in_position, 1);
  gl_Position = M * position;
  vs_texCoord1 = in_texCoord1;
  vs_texCoord2 = in_texCoord2;
}