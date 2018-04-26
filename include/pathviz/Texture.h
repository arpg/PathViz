#ifndef PATHVIZ_TEXTURE_H
#define PATHVIZ_TEXTURE_H

#include <pathviz/OpenGL.h>
#include <pathviz/Types.h>

namespace pathviz
{

class Texture2DArray
{
  public: struct Options
    {
      GLuint levels = 1;
      GLuint width  = 1;
      GLuint height = 1;
      GLuint layers = 1;
      GLenum format = GL_RGBA32F;
      bool last_blank = false;
    };

  public: Texture2DArray(const Options& options);

  public: ~Texture2DArray();

  public: Options GetOptions() const;

  public: void SetParameter(GLenum param, GLint value);

  public: GLuint GetId() const;

  private: void Initialize();

  protected: static GLuint GenerateId();

  protected: const GLuint m_id;

  protected: Options m_options;
};

}

#endif