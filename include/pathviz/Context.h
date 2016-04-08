#ifndef PATHVIZ_CONTEXT_H
#define PATHVIZ_CONTEXT_H

namespace pathviz
{

class Context
{
  public: static void Initialize();

  public: static void Destroy();

  protected: static void InitGlfw();

  protected: static void InitGlew();

  protected: static void InitOpenGL();

  protected: static void InitWindow();
};

}

#endif