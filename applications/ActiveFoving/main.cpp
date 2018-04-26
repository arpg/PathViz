#include <ctime>
#include <iostream>
#include <string>
#include <calibu/Calibu.h>
#include <pathviz/PathViz.h>

using namespace pathviz;

std::string rigFile;
std::string poseFile;
std::string sceneFile;
std::string outDir;
int rand_seed;

void ParseArgs(int argc, char** argv)
{
  // check if insufficent args given
  if (argc < 5)
  {
    std::cout << "usage: pathviz rig_file pose_file scene_file ";
    std::cout << "output_dir [rand_seed]" << std::endl;
    exit(0);
  }

  std::cout << "-------" << std::endl;
  std::cout << "Parsing arguments..." << std::endl;
  rigFile = std::string(argv[1]);
  poseFile = std::string(argv[2]);
  sceneFile = std::string(argv[3]);
  outDir = std::string(argv[4]);

  std::cout << "Rig File:    " << rigFile << std::endl;
  std::cout << "Pose File:   " << poseFile << std::endl;
  std::cout << "Scene File:   " << sceneFile << std::endl;
  std::cout << "Output Dir:  " << outDir << std::endl;

  // check if seed given
  if (argc >= 6)
  {
    rand_seed = atoi(argv[5]);
    std::cout << "Random Seed: " << rand_seed << std::endl;
    srand(rand_seed);
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

  AFSceneBuilder builder(calibuRig, path, sceneFile);
  if (rand_seed >= 0) builder.SetRandomSeed(rand_seed);
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
  rigBuilder.SetLastBlank(true);
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
  double timestamp;

  // rendering each pose in path
  for (uint frame = 0; frame < poseCount; ++frame)
  {
    Pose pose = path->GetPose(frame);
    rig->SetPose(pose.pose);
    timestamp = pose.time;

    // render image for each camera in rig
    for (uint cam_idx = 0; cam_idx < cameraCount; ++cam_idx)
    {
      RigCameraPtr camera = rig->GetCamera(cam_idx);
      camera->Capture(image);
      writer.Write(cam_idx, timestamp, image);
    }

    // print progress
    if (frame % 10 == 0)
    {
      float percent = 100 * float(frame) / poseCount;
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
