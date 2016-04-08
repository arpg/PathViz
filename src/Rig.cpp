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

Sophus::SE3f RigCamera::GetPose() const
{
  return m_camera.GetPose();
}

uint RigCamera::GetImageWidth() const
{
  return m_camera.GetImageWidth();
}

uint RigCamera::GetImageHeight() const
{
  return m_camera.GetImageHeight();
}

void RigCamera::Capture(Image& image)
{
  m_renderer->SetCamera(m_camera);
  m_renderer->Render(image);
}

void RigCamera::SetPose(const Sophus::SE3f& pose)
{
  m_camera.SetPose(m_offset * pose);
}

////////////////////////////////////////////////////////////////////////////////

Rig::Rig(ScenePtr scene) :
  m_renderer(new SceneRenderer(scene))
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