#ifndef PATHVIZ_RIG_H
#define PATHVIZ_RIG_H

#include <vector>
#include <sophus/se3.hpp>
#include <pathviz/Types.h>
#include <pathviz/Camera.h>
#include <pathviz/Image.h>
#include <pathviz/Scene.h>

namespace pathviz
{

////////////////////////////////////////////////////////////////////////////////

class RigCamera
{
  protected: RigCamera();

  public: ~RigCamera();

  public: Sophus::SE3f GetPose() const;

  public: uint GetImageWidth() const;

  public: uint GetImageHeight() const;

  public: void Capture(Image& image);

  protected: void SetPose(const Sophus::SE3f& pose);

  protected: Camera m_camera;

  protected: Sophus::SE3f m_offset;

  protected: SceneRendererPtr m_renderer;

  friend class Rig;
};

////////////////////////////////////////////////////////////////////////////////

class Rig
{
  public: Rig(ScenePtr scene);

  public: ~Rig();

  public: Sophus::SE3f GetPose() const;

  public: void SetPose(const Sophus::SE3f& pose);

  public: void AddCamera(const Camera& camera, const Sophus::SE3f& offset);

  public: uint GetCameraCount() const;

  public: RigCameraPtr GetCamera(uint index) const;

  protected: SceneRendererPtr m_renderer;

  protected: std::vector<RigCameraPtr> m_cameras;

  protected: Sophus::SE3f m_pose;
};

////////////////////////////////////////////////////////////////////////////////

}

#endif