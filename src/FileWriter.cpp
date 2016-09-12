#include <pathviz/FileWriter.h>
#include <iomanip>
#include <sstream>

using namespace pathviz;

static Eigen::IOFormat kCleanFmt(4, 0, ", ", ";\n", "", "");
static Eigen::IOFormat kLongFmt(Eigen::FullPrecision,
                                0, ", ", ";\n", "", "");
static Eigen::IOFormat kLongCsvFmt(Eigen::FullPrecision,
                                   0, ", ", "\n", "", "");

FileWriter::FileWriter(const std::string& outDir) :
  m_outDir((outDir.empty()) ? "." : outDir)
{
}

FileWriter::~FileWriter()
{
}

void FileWriter::Write(uint camera, double timestamp,
                       const Eigen::VectorXd& params)
{
  std::stringstream path;
  path << m_outDir << "/cam_params_";
  path << std::setfill('0') << std::setw(2) << camera << "_";
  path << ".csv";

  std::ofstream(path.str(), std::ios_base::app) <<
                 timestamp << ", " <<
                 params.transpose().format(kLongCsvFmt)
              << std::endl;
}
