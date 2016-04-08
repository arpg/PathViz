#version 330

uniform sampler2DArray current;
uniform int index;

uniform vec2 size;
uniform vec2 center;
uniform vec2 sig;
uniform float amp;
uniform mat2 R;

layout(location = 0) out float value;

float gaussian(vec2 xy)
{
  vec2 uv = R * (xy - center);
  float a = uv.x * uv.x / (2 * sig.x, sig.x);
  float b = uv.y * uv.y / (2 * sig.y, sig.y);
  return amp * exp(-(a + b));
}

void main()
{
  vec2 uv = vec2(gl_FragCoord);
  vec3 uvw = vec3(uv / size, index);
  float a = texture(current, uvw).x;
  float b = gaussian(uv);
  value = clamp(a + b, 0, 1);
}