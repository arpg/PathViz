#ifndef PATHVIZ_RIGBUILDER_H
#define PATHVIZ_RIGBUILDER_H

#include <pathviz/Types.h>
#include <pathviz/Camera.h>

namespace pathviz
{

class RigBuilder
{
  public: RigBuilder(calibu::RigPtr calibuRig, ScenePtr scene);

  public: ~RigBuilder();

  public: void SetLastBlank(bool blank);

  public: RigPtr Build();

  protected: void AddCameras(RigPtr rig);

  protected: void AddCamera(RigPtr rig, uint index);

  protected: void GetIntrinsics(Camera::Intrinsics& intrinsics, uint index);

  protected: calibu::RigPtr m_calibuRig;

  protected: ScenePtr m_scene;

  protected: bool m_last_blank;
};

}

#endif