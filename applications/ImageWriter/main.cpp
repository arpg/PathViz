#include <ctime>
#include <iostream>
#include <string>
#include <calibu/Calibu.h>
#include <pathviz/Types.h>
#include <pathviz/Context.h>
#include <pathviz/PathReader.h>
#include <pathviz/ImageWriter.h>
#include <pathviz/Path.h>
#include <pathviz/Rig.h>
#include <pathviz/RigBuilder.h>
#include <pathviz/Scene.h>
#include <pathviz/SceneBuilder.h>

using namespace pathviz;

std::string rigFile;
std::string poseFile;
std::string outDir;

void ParseArgs(int argc, char** argv)
{
  // check if insufficent args given
  if (argc < 4)
  {
    std::cout << "usage: pathviz rig_file pose_file output_dir [rand_seed]";
    std::cout << std::endl;
    exit(0);
  }

  std::cout << "-------" << std::endl;
  std::cout << "Parsing arguments..." << std::endl;
  rigFile = std::string(argv[1]);
  poseFile = std::string(argv[2]);
  outDir = std::string(argv[3]);

  std::cout << "Rig File:    " << rigFile << std::endl;
  std::cout << "Pose File:   " << poseFile << std::endl;
  std::cout << "Output Dir:  " << outDir << std::endl;

  // check if seed given
  if (argc >= 5)
  {
    uint seed = atoi(argv[4]);
    std::cout << "Random Seed: " << seed << std::endl;
    srand(seed);
  }

  std::cout << "-------" << std::endl;
}

calibu::RigPtr GetCalibuRig()
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

ScenePtr GetScene(calibu::RigPtr calibuRig, PathPtr path)
{
  std::cout << "Building scene..." << std::endl;

  SceneBuilder builder(calibuRig, path);
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

void RenderImages(RigPtr rig, PathPtr path)
{
  std::cout << "Rendering images to " << outDir << "..." << std::endl;
  clock_t startTime = clock();

  // prepare render loop
  uint poseCount = path->GetPoseCount();
  uint cameraCount = rig->GetCameraCount();
  ImageWriter writer(outDir);
  Image image;

  // rendering each pose in path
  for (uint i = 0; i < poseCount; ++i)
  {
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
  ParseArgs(argc, argv);
  Context::Initialize();

  calibu::RigPtr calibuRig = GetCalibuRig();
  PathPtr path = GetPath();
  ScenePtr scene = GetScene(calibuRig, path);
  RigPtr rig = GetRig(calibuRig, scene);
  RenderImages(rig, path);

  Context::Destroy();
  return 0;
}