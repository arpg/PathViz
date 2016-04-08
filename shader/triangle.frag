#version 330

uniform sampler2DArray textures;

in vec3 vs_texCoord1;
in vec3 vs_texCoord2;

out vec4 color;

void main()
{
  float a = texture(textures, vs_texCoord1).x;
  float b = texture(textures, vs_texCoord2).x;
  float c = a + b;

  color = vec4(c, c, c, 1);
}