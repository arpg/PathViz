#include <pathviz/Rig.h>
#include <pathviz/SceneRenderer.h>

using namespace pathviz;

////////////////////////////////////////////////////////////////////////////////

RigCamera::RigCamera()
{
}

RigCamera::~RigCamera()
{
}

Sophus::SE3f RigCamera::GetWorldPose() const
{
  return m_camera.GetPose();
}

Sophus::SE3f RigCamera::GetRigPose() const
{
  return m_offset;
}

void RigCamera::SetRigPose(const Sophus::SE3f& pose)
{
  const Sophus::SE3f Twc = GetWorldPose();
  const Sophus::SE3f Tcr = m_offset.inverse();
  const Sophus::SE3f Twr = Twc * Tcr;
  m_camera.SetPose(Twr * pose);
  m_offset = pose;
}

uint RigCamera::GetImageWidth() const
{
  return m_camera.GetImageWidth();
}

uint RigCamera::GetImageHeight() const
{
  return m_camera.GetImageHeight();
}

RigCamera::Intrinsics RigCamera::GetIntrinsics() const
{
  return m_camera.GetIntrinsics();
}

void RigCamera::SetIntrinsics(const RigCamera::Intrinsics& intrinsics)
{
  m_camera.Configure(intrinsics);
}

void RigCamera::Capture(Image& image)
{
  m_renderer->SetCamera(m_camera);
  m_renderer->Render(image);
}

void RigCamera::SetPose(const Sophus::SE3f& pose)
{
  m_camera.SetPose(pose * m_offset);
}

////////////////////////////////////////////////////////////////////////////////

Rig::Rig(ScenePtr scene, bool last_blank) :
  m_renderer(new SceneRenderer(scene, last_blank))
{
}

Rig::~Rig()
{
}

void Rig::SetPose(const Sophus::SE3f& pose)
{
  m_pose = pose;

  // set pose for all cameras
  for (RigCameraPtr camera : m_cameras)
  {
    camera->SetPose(pose);
  }
}

void Rig::AddCamera(const Camera& camera, const Sophus::SE3f& offset)
{
  RigCameraPtr rigCamera(new RigCamera());
  rigCamera->m_renderer = m_renderer;
  rigCamera->m_camera = camera;
  rigCamera->m_offset = offset;
  m_cameras.push_back(rigCamera);
}

uint Rig::GetCameraCount() const
{
  return m_cameras.size();
}

RigCameraPtr Rig::GetCamera(uint index) const
{
  return m_cameras[index];
}

////////////////////////////////////////////////////////////////////////////////