#include <pathviz/ImageWriter.h>
#include <iomanip>
#include <sstream>
#include <opencv2/opencv.hpp>

using namespace pathviz;

ImageWriter::ImageWriter(const std::string& outDir) :
  m_outDir((outDir.empty()) ? "." : outDir)
{
}

ImageWriter::~ImageWriter()
{
}

void ImageWriter::Write(uint camera, uint frame, const Image& image)
{
  std::stringstream path;
  path << m_outDir << "/capture_";
  path << std::setfill('0') << std::setw(2) << camera << "_";
  path << std::setfill('0') << std::setw(5) << frame << ".png";
  WriteRaw(path.str(), image);
}

void ImageWriter::Write(const std::string& filename, const Image& image)
{
  std::stringstream path;
  path << m_outDir << "/" << filename;
  WriteRaw(path.str(), image);
}

void ImageWriter::WriteRaw(const std::string& path, const Image& image)
{
  cv::Mat mat(image.height(), image.width(), CV_8UC4, image.data());
  cv::cvtColor(mat, mat, CV_RGBA2GRAY);
  cv::flip(mat, mat, 0);
  cv::imwrite(path, mat);
}