#ifndef PATHVIZ_PATH_H
#define PATHVIZ_PATH_H

#include <vector>
#include <sophus/se3.hpp>
#include <Eigen/Geometry>

namespace pathviz
{

struct Pose
{
  double time;
  Sophus::SE3f pose;
};

class Path
{
  public: Path();

  public: ~Path();

  public: void AddPose(const Pose& pose);

  public: uint GetPoseCount() const;

  public: Pose GetPose(uint index) const;

  public: Eigen::AlignedBox3f GetBounds() const;

  protected: std::vector<Pose> m_poses;

  protected: Eigen::AlignedBox3f m_bounds;
};

}

#endif