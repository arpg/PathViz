#ifndef PATHVIZ_PROGRAM_H
#define PATHVIZ_PROGRAM_H

#include <string>
#include <Eigen/Eigen>
#include <pathviz/OpenGL.h>
#include <pathviz/Types.h>
#include <pathviz/Shader.h>

#include <iostream>

namespace pathviz
{

////////////////////////////////////////////////////////////////////////////////

struct Uniform
{
  Uniform(GLint id, GLuint program) :
    id(id),
    program(program)
  {
  }

  inline void SetInt(int a)
  {
    glUseProgram(program);
    glUniform1i(id, a);
  }

  inline void SetFloat(float a)
  {
    glUseProgram(program);
    glUniform1f(id, a);
  }

  inline void SetFloat2(float a, float b)
  {
    glUseProgram(program);
    glUniform2f(id, a, b);
  }

  inline void SetMat2(const Eigen::Matrix2f& a)
  {
    glUseProgram(program);
    glUniformMatrix2fv(id, 1, false, a.data());
  }

  inline void SetMat4(const Eigen::Matrix4f& a)
  {
    glUseProgram(program);
    glUniformMatrix4fv(id, 1, false, a.data());
  }

  const GLint id;
  const GLuint program;
};

////////////////////////////////////////////////////////////////////////////////

struct Attribute
{
  struct Options
  {
    GLint size = 4;
    GLenum type = GL_FLOAT;
    GLboolean normalized = GL_FALSE;
    GLsizei stride = 0;
    void* pointer = 0;
  };

  Attribute(GLint id, GLuint program) :
    id(id),
    program(program)
  {
  }

  inline void Enable()
  {
    glUseProgram(program);
    glEnableVertexAttribArray(id);
  }

  inline void Disable()
  {
    glUseProgram(program);
    glDisableVertexAttribArray(id);
  }

  inline void Configure(const Options& options)
  {
    glUseProgram(program);

    glVertexAttribPointer(id, options.size, options.type, options.normalized,
        options.stride, options.pointer);
  }

  const GLint id;
  const GLuint program;
};

////////////////////////////////////////////////////////////////////////////////

class Program
{
  public: Program();

  public: ~Program();

  public: void Attach(const Shader& shader);

  public: void Detach(const Shader& shader);

  public: void Link();

  public: void SetActive();

  public: GLuint GetId() const;

  public: Uniform GetUniform(const std::string& name);

  public: Attribute GetAttribute(const std::string& name);

  protected: inline void CheckLink();

  protected: inline std::string GetLinkError();

  protected: const GLuint m_id;
};

////////////////////////////////////////////////////////////////////////////////

}

#endif
