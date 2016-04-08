#include <pathviz/RigBuilder.h>
#include <pathviz/Rig.h>
#include <pathviz/Scene.h>

using namespace pathviz;

RigBuilder::RigBuilder(calibu::RigPtr calibuRig, ScenePtr scene) :
  m_calibuRig(calibuRig),
  m_scene(scene)
{
}

RigBuilder::~RigBuilder()
{
}

RigPtr RigBuilder::Build()
{
  RigPtr rig(new Rig(m_scene));
  AddCameras(rig);
  return rig;
}

void RigBuilder::AddCameras(RigPtr rig)
{
  uint count = m_calibuRig->NumCams();

  // add each camera in calibu rig
  for (uint i = 0; i < count; ++i)
  {
    AddCamera(rig, i);
  }
}

void RigBuilder::AddCamera(RigPtr rig, uint index)
{
  // get camera offset
  calibu::CameraPtr calibuCamera = m_calibuRig->cameras_[index];
  Sophus::SE3f offset = calibuCamera->Pose().cast<float>();

  // get camera intrinsics
  Camera::Intrinsics intrinsics;
  GetIntrinsics(intrinsics, index);
  Camera camera(intrinsics);

  rig->AddCamera(camera, offset);
}

void RigBuilder::GetIntrinsics(Camera::Intrinsics& intrinsics, uint index)
{
  calibu::CameraPtr camera = m_calibuRig->cameras_[index];
  Eigen::Matrix3f K = camera->K().cast<float>();
  intrinsics.w  = camera->Width();
  intrinsics.h  = camera->Height();
  intrinsics.fx = K(0, 0);
  intrinsics.fy = K(1, 1);
  intrinsics.cx = K(0, 2);
  intrinsics.cy = K(1, 2);
  intrinsics.far = 2000.0;
  intrinsics.near = 0.01;
}