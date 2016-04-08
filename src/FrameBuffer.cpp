#include <pathviz/FrameBuffer.h>
#include <pathviz/Texture.h>

using namespace pathviz;

////////////////////////////////////////////////////////////////////////////////

FrameBuffer::FrameBuffer(uint width, uint height) :
  m_id(GenerateId()),
  m_colorId(GenerateBufferId()),
  m_depthId(GenerateBufferId())
{
  Resize(width, height);
}

FrameBuffer::~FrameBuffer()
{
  glDeleteFramebuffers(1, &m_id);
  glDeleteRenderbuffers(1, &m_colorId);
  glDeleteRenderbuffers(1, &m_depthId);
}

void FrameBuffer::SetActive()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
  glViewport(0, 0, m_width, m_height);
}

void FrameBuffer::Resize(uint width, uint height)
{
  // check if larger allocation needed
  if (AllocationNeeded(width, height))
  {
    Allocate(width, height);
  }

  // set viewport regardless
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
  glViewport(0, 0, width, height);
  m_width = width;
  m_height = height;
}

void FrameBuffer::Read(Image& image)
{
  SetActive();
  image.Resize(m_width, m_height);
  glReadPixels(0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, image.data());
}

void FrameBuffer::Clear()
{
  SetActive();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

bool FrameBuffer::AllocationNeeded(uint width, uint height)
{
  GLint w = 0;
  GLint h = 0;
  glBindRenderbuffer(GL_RENDERBUFFER, m_colorId);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &w);
  glGetRenderbufferParameteriv(GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &h);
  return (width > w || height > h);
}

void FrameBuffer::Allocate(uint width, uint height)
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);

  // allocate color render buffer
  glBindRenderbuffer(GL_RENDERBUFFER, m_colorId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_RGBA8, width, height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      GL_RENDERBUFFER, m_colorId);

  // allocate depth render buffer
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthId);
  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_RENDERBUFFER, m_depthId);
}

GLuint FrameBuffer::GenerateId()
{
  GLuint id;
  glGenFramebuffers(1, &id);
  return id;
}

GLuint FrameBuffer::GenerateBufferId()
{
  GLuint id;
  glGenRenderbuffers(1, &id);
  return id;
}

////////////////////////////////////////////////////////////////////////////////

TextureFrameBuffer::TextureFrameBuffer(Texture2DArrayPtr textures) :
  m_id(GenerateId()),
  m_depthId(GenerateBufferId()),
  m_textures(textures),
  m_layer(0)
{
  Initialize();
}

TextureFrameBuffer::~TextureFrameBuffer()
{
  glDeleteFramebuffers(1, &m_id);
  glDeleteRenderbuffers(1, &m_depthId);
}

void TextureFrameBuffer::SetActive()
{
  glBindFramebuffer(GL_FRAMEBUFFER, m_id);

  glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      m_textures->GetId(), 0, m_layer);

  const Texture2DArray::Options& options = m_textures->GetOptions();
  glViewport(0, 0, options.width, options.height);
}

void TextureFrameBuffer::SetLayer(uint layer)
{
  SetActive();
  m_layer = layer;

  glFramebufferTextureLayer(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
      m_textures->GetId(), 0, m_layer);
}

void TextureFrameBuffer::Clear()
{
  SetActive();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void TextureFrameBuffer::ClearDepth()
{
  SetActive();
  glClear(GL_DEPTH_BUFFER_BIT);
}

void TextureFrameBuffer::Initialize()
{
  const Texture2DArray::Options& options = m_textures->GetOptions();

  glBindFramebuffer(GL_FRAMEBUFFER, m_id);
  glBindRenderbuffer(GL_RENDERBUFFER, m_depthId);

  glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, options.width,
      options.height);

  glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
      GL_RENDERBUFFER, m_depthId);

  GLenum drawBuffers[1] = { GL_COLOR_ATTACHMENT0 };
  glDrawBuffers(1, drawBuffers);

  SetLayer(m_layer);
}

GLuint TextureFrameBuffer::GenerateId()
{
  GLuint id;
  glGenFramebuffers(1, &id);
  return id;
}

GLuint TextureFrameBuffer::GenerateBufferId()
{
  GLuint id;
  glGenRenderbuffers(1, &id);
  return id;
}