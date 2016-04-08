#include <pathviz/Shader.h>
#include <fstream>
#include <sstream>
#include <pathviz/Exception.h>

using namespace pathviz;

Shader::Shader(GLuint type, const std::string& filename) :
  m_id(glCreateShader(type)),
  m_type(type),
  m_filename(GetFullFilePath(filename))
{
}

Shader::~Shader()
{
  glDeleteShader(m_id);
}

void Shader::Compile()
{
  // read source from file
  std::string source = ReadSource();
  const GLchar* const sourcePtr = source.c_str();
  GLint sourceLen = source.length();

  // compile source code
  glShaderSource(m_id, 1, &sourcePtr, &sourceLen);
  glCompileShader(m_id);

  // validate
  CheckCompile();
}

GLuint Shader::GetId() const
{
  return m_id;
}

void Shader::CheckCompile()
{
  // get status
  GLint success = 0;
  glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);

  // check if failed
  if (success != GL_TRUE)
  {
    throw Exception(GetCompileError());
  }
}

std::string Shader::GetCompileError()
{
  // get message length
  GLint length = 0;
  glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);

  // get message text
  GLchar message[length];
  glGetShaderInfoLog(m_id, length, &length, message);

  // build error message
  std::stringstream error;
  error << "Error compiling shader: " << m_filename << std::endl << message;

  // return string
  return error.str();
}

std::string Shader::ReadSource()
{
  // open file and get size
  std::ifstream fin(m_filename, std::ios::binary | std::ios::ate);
  unsigned long fileLength = fin.tellg();
  std::string source;

  // check if non-empty file
  if (fileLength > 0)
  {
    // read entire file into string
    source.resize(fileLength + 1, 0);
    fin.seekg(0, std::ios::beg);
    fin.read(&source[0], fileLength);
  }

  fin.close();
  return source;
}

std::string Shader::GetFullFilePath(const std::string& filename)
{
  return PATHVIZ_SHADER_DIR + filename;
}