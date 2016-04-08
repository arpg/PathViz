#ifndef PATHVIZ_SHADER_H
#define PATHVIZ_SHADER_H

#include <string>
#include <pathviz/OpenGL.h>
#include <pathviz/Types.h>

namespace pathviz
{

////////////////////////////////////////////////////////////////////////////////

class Shader
{
  protected: Shader(GLuint type, const std::string& filename);

  public: ~Shader();

  public: void Compile();

  public: GLuint GetId() const;

  protected: inline void CheckCompile();

  protected: inline std::string GetCompileError();

  protected: inline std::string ReadSource();

  protected: inline std::string GetFullFilePath(const std::string& filename);

  protected: const GLuint m_id;

  protected: const GLuint m_type;

  protected: const std::string m_filename;
};

////////////////////////////////////////////////////////////////////////////////

class FragmentShader : public Shader
{
  public: FragmentShader(const std::string& filename) :
      Shader(GL_FRAGMENT_SHADER, filename)
    {
    }

  public: ~FragmentShader()
    {
    }
};

////////////////////////////////////////////////////////////////////////////////

class VertexShader : public Shader
{
  public: VertexShader(const std::string& filename) :
      Shader(GL_VERTEX_SHADER, filename)
    {
    }

  public: ~VertexShader()
    {
    }
};

////////////////////////////////////////////////////////////////////////////////

}

#endif
