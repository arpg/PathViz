#include <pathviz/Context.h>
#include <pathviz/OpenGL.h>

using namespace pathviz;

void Context::Initialize()
{
  InitGlfw();
  InitWindow();
  InitGlew();
  InitOpenGL();
}

void Context::Destroy()
{
}

void Context::InitGlfw()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}

void Context::InitGlew()
{
  glewExperimental = GL_TRUE;
  glewInit();
  glGetError();
}

void Context::InitOpenGL()
{
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

void Context::InitWindow()
{
  glfwWindowHint(GLFW_VISIBLE, GL_FALSE);
  GLFWwindow* window = glfwCreateWindow(1, 1, "", NULL, NULL);
  glfwMakeContextCurrent(window);
}
