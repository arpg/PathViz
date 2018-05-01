#include <pathviz/PathReader.h>
#include <fstream>
#include <sstream>
#include <pathviz/Exception.h>

using namespace pathviz;

PathReader::PathReader(const std::string& filename) :
  m_filename(filename),
  m_linesRead(0)
{
}

PathReader::~PathReader()
{
}

PathPtr PathReader::Read()
{
  // create empty path
  PathPtr path(new Path());

  Pose pose;

  // open file for reading
  std::ifstream stream(m_filename);
  std::string line;
  m_linesRead = 0;

  // read each line in file
  while (std::getline(stream, line))
  {
    ++m_linesRead;
    ParsePose(line, pose);
    path->AddPose(pose);
  }

  stream.close();
  return path;
}

void PathReader::ParsePose(const std::string& line, Pose& pose)
{
  double values[VALUES_PER_LINE];
  ParseValues(line, values);

  // parse time
  pose.time = 1e-9*values[0];

  // parse translation
  Eigen::Vector3f t(values[5], values[6], values[7]);

  // parse rotation
  Eigen::Quaternionf r(values[1], values[2], values[3], values[4]);

  // create final pose
  pose.pose = Sophus::SE3f(r, t);
}

void PathReader::ParseValues(const std::string& line, double* values)
{
  uint valuesRead = 0;
  std::stringstream tokens(line);
  std::string token;

  // read each value in line
  while (valuesRead < VALUES_PER_LINE && std::getline(tokens, token, ' '))
  {
    values[valuesRead++] = ParseValue(token);
  }

  // check if all values read
  if (valuesRead < VALUES_PER_LINE)
  {
    std::stringstream error;
    error << "Insufficient value count on line " << m_linesRead;
    error << " of pose file: " << m_filename;
    error << ". Expected " << VALUES_PER_LINE << " got " << valuesRead;
    throw Exception(error.str());
  }
}

double PathReader::ParseValue(const std::string& token)
{
  try
  {
    return stod(token);
  }
  catch (...)
  {
    std::stringstream error;
    error << "Invalid value on line " << m_linesRead;
    error << " of pose file: " << m_filename;
    throw Exception(error.str());
  }
}
