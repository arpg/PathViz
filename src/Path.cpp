#include <pathviz/Path.h>

using namespace pathviz;

Path::Path()
{
}

Path::~Path()
{
}

void Path::AddPose(const pathviz::Pose& pose)
{
  m_poses.push_back(pose);
  m_bounds.extend(pose.pose.translation());
}

uint Path::GetPoseCount() const
{
  return m_poses.size();
}

Pose Path::GetPose(uint index) const
{
  return m_poses[index];
}

Eigen::AlignedBox3f Path::GetBounds() const
{
  return m_bounds;
}