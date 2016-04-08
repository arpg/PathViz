#ifndef PATHVIZ_IMAGEWRITER_H
#define PATHVIZ_IMAGEWRITER_H

#include <string>
#include <pathviz/Types.h>
#include <pathviz/Image.h>

namespace pathviz
{

class ImageWriter
{
  public: ImageWriter(const std::string& outDir);

  public: ~ImageWriter();

  public: void Write(uint camera, uint frame, const Image& image);

  public: void Write(const std::string& filename, const Image& image);

  public: void WriteRaw(const std::string& path, const Image& image);

  protected: std::string m_outDir;
};

}

#endif