#ifndef PATHVIZ_FILEWRITER_H
#define PATHVIZ_FILEWRITER_H

#include <string>
#include <pathviz/Types.h>
#include <pathviz/Image.h>
#include <fstream>

namespace pathviz
{

class FileWriter
{
  public: FileWriter(const std::string& outDir);

  public: ~FileWriter();

  public: void Write(uint camera, double timestamp,
                     const Eigen::VectorXd &params);

  protected: std::string m_outDir;
};

}

#endif
