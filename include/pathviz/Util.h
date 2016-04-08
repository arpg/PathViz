#ifndef PATHVIZ_UTIL_H
#define PATHVIZ_UTIL_H

#include <cstdlib>

namespace pathviz
{

inline float randf()
{
  return float(std::rand()) / RAND_MAX;
}

inline int rand(int min, int max)
{
  return std::rand() % (max - min) + min;
}

inline float rand(float min, float max)
{
  return (max - min) * randf() + min;
}

}

#endif