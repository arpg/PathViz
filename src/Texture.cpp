#include <pathviz/Texture.h>

using namespace pathviz;

Texture2DArray::Texture2DArray(const Options& options) :
  m_id(GenerateId()),
  m_options(options)
{
  Initialize();
}

Texture2DArray::~Texture2DArray()
{
  glDeleteTextures(1, &m_id);
}

Texture2DArray::Options Texture2DArray::GetOptions() const
{
  return m_options;
}

void Texture2DArray::SetParameter(GLenum param, GLint value)
{
  glActiveTexture(GL_TEXTURE0 + m_id);
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);
  glTexParameteri(GL_TEXTURE_2D_ARRAY, param, value);
}

GLuint Texture2DArray::GetId() const
{
  return m_id;
}

void Texture2DArray::Initialize()
{
  glActiveTexture(GL_TEXTURE0 + m_id);
  glBindTexture(GL_TEXTURE_2D_ARRAY, m_id);

  glTexStorage3D(GL_TEXTURE_2D_ARRAY, m_options.levels, m_options.format,
      m_options.width, m_options.height, m_options.layers);
}

GLuint Texture2DArray::GenerateId()
{
  GLuint id;
  glGenTextures(1, &id);
  return id;
}