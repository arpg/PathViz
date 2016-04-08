#ifndef PATHVIZ_PATHREADER_H
#define PATHVIZ_PATHREADER_H

#include <string>
#include <pathviz/Types.h>
#include <pathviz/Path.h>

namespace pathviz
{

class PathReader
{
  public: PathReader(const std::string& filename);

  public: ~PathReader();

  public: PathPtr Read();

  protected: inline void ParsePose(const std::string& line, Pose& pose);

  protected: inline void ParseValues(const std::string& line, double* values);

  protected: inline double ParseValue(const std::string& token);

  protected: std::string m_filename;

  protected: uint m_linesRead;

  protected: static const uint VALUES_PER_LINE = 8;
};

}

#endif