#ifndef PATHVIZ_FRAMEBUFFER_H
#define PATHVIZ_FRAMEBUFFER_H

#include <pathviz/OpenGL.h>
#include <pathviz/Types.h>
#include <pathviz/Image.h>

namespace pathviz
{

////////////////////////////////////////////////////////////////////////////////

class FrameBuffer
{
  public: FrameBuffer(uint width, uint height);

  public: ~FrameBuffer();

  public: void SetActive();

  public: void Resize(uint width, uint height);

  public: void Read(Image& image);

  public: void Clear();

  protected: bool AllocationNeeded(uint width, uint height);

  protected: void Allocate(uint width, uint height);

  protected: static GLuint GenerateId();

  protected: static GLuint GenerateBufferId();

  protected: const GLuint m_id;

  protected: const GLuint m_colorId;

  protected: const GLuint m_depthId;

  protected: uint m_width;

  protected: uint m_height;
};

////////////////////////////////////////////////////////////////////////////////

class TextureFrameBuffer
{
  public: TextureFrameBuffer(Texture2DArrayPtr textures);

  public: ~TextureFrameBuffer();

  public: void SetActive();

  public: void SetLayer(uint layer);

  public: void Clear();

  public: void ClearDepth();

  protected: void Initialize();

  protected: static GLuint GenerateId();

  protected: static GLuint GenerateBufferId();

  protected: const GLuint m_id;

  protected: const GLuint m_depthId;

  protected: Texture2DArrayPtr m_textures;

  protected: uint m_layer;
};

////////////////////////////////////////////////////////////////////////////////

}

#endif
