#include <iostream>
#include <fstream>
#include <vector>
#include <calibu/Calibu.h>
#include <pathviz/PathViz.h>

using namespace pathviz;

std::string rigListFile;
std::string interpFile;
std::string poseFile;
std::string outDir;

void ParseArgs(int argc, char** argv)
{
  // check if insufficent args given
  if (argc < 5)
  {
    std::cout << "usage: dynamic_rig_image_writer rig_list_file ";
    std::cout << "interp_file pose_file output_dir [seed]" << std::endl;
    std::exit(0);
  }

  std::cout << "-------" << std::endl;
  std::cout << "Parsing arguments..." << std::endl;
  rigListFile = std::string(argv[1]);
  interpFile = std::string(argv[2]);
  poseFile = std::string(argv[3]);
  outDir = std::string(argv[4]);

  std::cout << "Rig List File: " << rigListFile << std::endl;
  std::cout << "Interp File:   " << interpFile << std::endl;
  std::cout << "Pose File:     " << poseFile << std::endl;
  std::cout << "Output Dir:    " << outDir << std::endl;

  // check if seed given
  if (argc >= 6)
  {
    uint seed = atoi(argv[5]);
    std::cout << "Random Seed:   " << seed << std::endl;
    srand(seed);
  }

  std::cout << "-------" << std::endl;
}

calibu::RigPtr GetCalibuRig(const std::string& rigFile)
{
  std::cout << "Reading rig file: " << rigFile << "..." << std::endl;

  // read calibu rig from cameras.xml file
  calibu::RigPtr rig = calibu::ReadXmlRig(rigFile);
  uint cameraCount = rig->NumCams();

  std::cout << "Read rig with " << cameraCount << " camera(s)" << std::endl;
  std::cout << "-------" << std::endl;

  // check if empty rig
  if (cameraCount == 0)
  {
    std::flush(std::cout);
    std::cerr << "No cameras in rig. Exiting..." << std::endl;
    exit(0);
  }

  return rig;
}

void GetCalibuRigs(std::vector<calibu::RigPtr>& calibuRigs)
{
  std::cout << "Reading rig list file: " << rigListFile << "..." << std::endl;
  std::cout << "-------" << std::endl;
  std::cout << "-------" << std::endl;

  // open file for reading
  std::ifstream stream(rigListFile);
  std::string line;

  uint camCount = 0;

  // read each line in file
  while (std::getline(stream, line))
  {
    calibuRigs.push_back(GetCalibuRig(line));

    // check if first rig added
    if (calibuRigs.size() == 1)
    {
      camCount = calibuRigs.back()->NumCams();
    }
    // check if camera counts do not match
    else if (calibuRigs.size() > 1 && calibuRigs.back()->NumCams() != camCount)
    {
      std::cerr << "Camera counts in each rig file do not match" << std::endl;
      std::exit(0);
    }
  }

  std::cout << "-------" << std::endl;
  std::cout << "Read rig list file with " << calibuRigs.size();
  std::cout << " rigs(s)" << std::endl;
  std::cout << "-------" << std::endl;

  stream.close();
}

void GetInterpValues(std::vector<double>& interpValues)
{
  std::cout << "Reading interp file: " << interpFile << "..." << std::endl;

  // open file for reading
  std::ifstream stream(interpFile);
  std::string line;

  // read each line in file
  while (std::getline(stream, line))
  {
    interpValues.push_back(atof(line.c_str()));
  }

  std::cout << "Read interp file with " << interpValues.size();
  std::cout << " values(s)" << std::endl;
  std::cout << "-------" << std::endl;

  stream.close();
}

PathPtr GetPath()
{
  std::cout << "Reading pose file: " << poseFile << "..." << std::endl;

  // read path from pose file
  PathReader pathReader(poseFile);
  PathPtr path = pathReader.Read();
  uint poseCount = path->GetPoseCount();

  std::cout << "Read path with " << poseCount << " pose(s)" << std::endl;
  std::cout << "-------" << std::endl;

  // check if empty path
  if (poseCount == 0)
  {
    std::flush(std::cout);
    std::cerr << "No poses read. Exiting..." << std::endl;
    exit(0);
  }

  return path;
}

ScenePtr GetScene(const std::vector<calibu::RigPtr>& calibuRigs, PathPtr path)
{
  std::cout << "Building scene..." << std::endl;

  SceneBuilder builder(calibuRigs, path);
  builder.SetTextureCount(20);
  ScenePtr scene = builder.Build();
  uint boxCount = scene->GetBoxCount();

  std::cout << "Scene built with " << boxCount << " box(es)" << std::endl;
  std::cout << "-------" << std::endl;
  return scene;
}

RigPtr GetRig(calibu::RigPtr calibuRig, ScenePtr scene)
{
  std::cout << "Building rig..." << std::endl;

  // build rig from path & calibu-rig
  RigBuilder rigBuilder(calibuRig, scene);
  RigPtr rig = rigBuilder.Build();

  std::cout << "Rig built" << std::endl;
  std::cout << "-------" << std::endl;
  return rig;
}

void GetRigs(const std::vector<calibu::RigPtr>& calibuRigs, ScenePtr scene,
    std::vector<RigPtr>& rigs)
{
  std::cout << "Building rigs from calibu-rigs" << std::endl;
  std::cout << "-------" << std::endl;
  std::cout << "-------" << std::endl;

  // build rig from each calibu-rig
  for (calibu::RigPtr calibuRig : calibuRigs)
  {
    rigs.push_back(GetRig(calibuRig, scene));
  }

  std::cout << "-------" << std::endl;
  std::cout << "Built " << rigs.size() << " rigs(s)" << std::endl;
  std::cout << "-------" << std::endl;
}

void InterpExtrinsics(const Sophus::SE3f& a, const Sophus::SE3f& b,
    double interp, Sophus::SE3f& output)
{
  const double alpha = 1 - interp;
  const double beta  = interp;
  output.translation() = alpha * a.translation() + beta * b.translation();

  Eigen::Quaternionf quat_a(a.rotationMatrix());
  Eigen::Quaternionf quat_b(b.rotationMatrix());
  output.setQuaternion(quat_a.slerp(beta, quat_b));
}

void InterpIntrinsics(const RigCamera::Intrinsics& a,
    const RigCamera::Intrinsics& b, double interp,
    RigCamera::Intrinsics& output)
{
  if (a.w != b.w || a.h != b.h)
  {
    std::cerr << "Camera resolution mismatch: ";
    std::cerr << a.w << "x" << a.h << " and ";
    std::cerr << b.w << "x" << b.h << std::endl;
    std::exit(0);
  }

  output.w = a.w;
  output.h = a.h;

  const double alpha = 1 - interp;
  const double beta  = interp;
  output.fx   = alpha * a.fx + beta * b.fx;
  output.fy   = alpha * a.fy + beta * b.fy;
  output.cx   = alpha * a.cx + beta * b.cx;
  output.cy   = alpha * a.cy + beta * b.cy;
  output.near = alpha * a.near + beta * b.near;
  output.far  = alpha * a.far + beta * b.far;
}

void InterpRig(RigPtr a, RigPtr b, double interp, RigPtr output)
{
  const uint camCount = a->GetCameraCount();

  for (uint i = 0; i < camCount; ++i)
  {
    RigCameraPtr camera_a = a->GetCamera(i);
    RigCameraPtr camera_b = b->GetCamera(i);
    const Sophus::SE3f ex_a = camera_a->GetRigPose();
    const Sophus::SE3f ex_b = camera_b->GetRigPose();
    const RigCamera::Intrinsics in_a = camera_a->GetIntrinsics();
    const RigCamera::Intrinsics in_b = camera_b->GetIntrinsics();

    Sophus::SE3f ex;
    RigCamera::Intrinsics in;
    InterpExtrinsics(ex_a, ex_b, interp, ex);
    InterpIntrinsics(in_a, in_b, interp, in);

    RigCameraPtr camera = output->GetCamera(i);
    camera->SetIntrinsics(in);
    camera->SetRigPose(ex);
  }
}

void InterpRig(const std::vector<RigPtr>& rigs, double interp, RigPtr output)
{
  int startIndex = floor(interp);

  if (startIndex < 0 || startIndex > rigs.size() - 1)
  {
    std::cerr << "out-of-bounds interp value: " << interp << std::endl;
    std::exit(0);
  }

  int endIndex = startIndex + ((startIndex == rigs.size() - 1) ? 0 : 1);

  RigPtr a = rigs[startIndex];
  RigPtr b = rigs[endIndex];
  double diff = interp - floor(interp);
  InterpRig(a, b, diff, output);
}

void RenderImages(const std::vector<RigPtr>& rigs,
  const std::vector<double>& interpValues, ScenePtr scene, PathPtr path)
{
  std::cout << "Rendering images to " << outDir << "..." << std::endl;
  clock_t startTime = clock();

  // prepare render loop
  uint poseCount = path->GetPoseCount();
  uint cameraCount = rigs[0]->GetCameraCount();
  ImageWriter writer(outDir);
  Image image;

  // create rig for interp output
  RigPtr rig = std::make_shared<Rig>(scene);

  for (uint i = 0; i < cameraCount; ++i)
  {
    rig->AddCamera(Camera(), Sophus::SE3f());
  }

  // rendering each pose in path
  for (uint i = 0; i < poseCount; ++i)
  {
    InterpRig(rigs, interpValues[i], rig);
    Pose pose = path->GetPose(i);
    rig->SetPose(pose.pose);

    // render image for each camera in rig
    for (uint j = 0; j < cameraCount; ++j)
    {
      RigCameraPtr camera = rig->GetCamera(j);
      camera->Capture(image);
      writer.Write(j, i, image);
    }

    // print progress
    if (i % 10 == 0)
    {
      float percent = 100 * float(i) / poseCount;
      std::cout << "\rRendered:            ";
      std::cout << "\rRendered: " << percent << "%" << std::flush;
    }
  }

  std::cout << "\rRendered:            ";
  std::cout << "\rRendered: 100.0%" << std::endl;

  clock_t stopTime = clock();
  double elapsedTime = float(stopTime - startTime) / CLOCKS_PER_SEC;
  std::cout << "Elapsed time: " << elapsedTime << "s" << std::endl;
  std::cout << "-------" << std::endl;
}

int main(int argc, char** argv)
{
  std::vector<calibu::RigPtr> calibuRigs;
  std::vector<double> interpValues;
  std::vector<RigPtr> rigs;

  ParseArgs(argc, argv);
  Context::Initialize();
  GetCalibuRigs(calibuRigs);
  GetInterpValues(interpValues);
  PathPtr path = GetPath();

  // check for interp-pose count mismatch
  if (interpValues.size() != path->GetPoseCount())
  {
    std::cerr << "interp value count does not match pose count" << std::endl;
    std::exit(0);
  }

  ScenePtr scene = GetScene(calibuRigs, path);
  GetRigs(calibuRigs, scene, rigs);
  RenderImages(rigs, interpValues, scene, path);

  std::cout << "Success" << std::endl;
  return 0;
}