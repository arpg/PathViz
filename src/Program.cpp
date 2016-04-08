#include <pathviz/Program.h>
#include <iostream>
#include <sstream>
#include <pathviz/Exception.h>
#include <pathviz/Shader.h>

using namespace pathviz;

Program::Program() :
  m_id(glCreateProgram())
{
}

Program::~Program()
{
  glDeleteProgram(m_id);
}

void Program::Attach(const Shader& shader)
{
  glAttachShader(m_id, shader.GetId());
}

void Program::Detach(const Shader& shader)
{
  glDetachShader(m_id, shader.GetId());
}

void Program::Link()
{
  glLinkProgram(m_id);
  CheckLink();
}

void Program::SetActive()
{
  glUseProgram(m_id);
}

GLuint Program::GetId() const
{
  return m_id;
}

Uniform Program::GetUniform(const std::string& name)
{
  GLint uid = glGetUniformLocation(m_id, name.c_str());

  if (uid < 0)
  {
    std::cerr << "Uniform variable '" << name;
    std::cerr << "' not found in program" << std::endl;
  }

  return Uniform(uid, m_id);
}

Attribute Program::GetAttribute(const std::string& name)
{
  GLint aid = glGetAttribLocation(m_id, name.c_str());

  if (aid < 0)
  {
    std::cerr << "Attribute variable '" << name;
    std::cerr << "' not found in program" << std::endl;
  }

  return Attribute(aid, m_id);
}

void Program::CheckLink()
{
  // get status
  GLint success = 0;
  glGetProgramiv(m_id, GL_LINK_STATUS, &success);

  // check if failed
  if (success != GL_TRUE)
  {
    throw Exception(GetLinkError());
  }
}

std::string Program::GetLinkError()
{
  // get message length
  GLint length = 0;
  glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);

  // get message
  GLchar message[length];
  glGetProgramInfoLog(m_id, length, &length, message);

  // build error message
  std::stringstream error;
  error << "Error linking program:" << std::endl << message;

  // return string
  return error.str();
}