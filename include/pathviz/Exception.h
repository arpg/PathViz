#ifndef PATHVIZ_EXCEPTION_H
#define PATHVIZ_EXCEPTION_H

#include <exception>
#include <string>

namespace pathviz
{

class Exception : public std::exception
{
  public: Exception(const char* message) :
      m_message(message)
    {
    }

  public: Exception(const std::string& message) :
      m_message(message)
    {
    }

  public: const char* what() const throw()
    {
      return m_message.c_str();
    }

  protected: const std::string m_message;
};

}

#endif