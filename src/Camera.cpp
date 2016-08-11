#include <pathviz/Camera.h>

using namespace pathviz;

Camera::Camera() :
  m_projMatrix(ComputeProjectionMatrix())
{
  InitTransform();
}

Camera::Camera(const Intrinsics &intrinsics) :
  m_intrinsics(intrinsics),
  m_projMatrix(ComputeProjectionMatrix())
{
  InitTransform();
}

Camera::~Camera()
{
}

void Camera::Configure(const Camera::Intrinsics& intrinsics)
{
  m_intrinsics = intrinsics;
  m_projMatrix = ComputeProjectionMatrix();
}

uint Camera::GetImageWidth() const
{
  return m_intrinsics.w;
}

uint Camera::GetImageHeight() const
{
  return m_intrinsics.h;
}

Sophus::SE3f Camera::GetPose() const
{
  return m_pose;
}

void Camera::SetPose(const Sophus::SE3f& pose)
{
  m_pose = pose;
}

Eigen::Matrix4f Camera::GetProjectionMatrix() const
{
  return m_projMatrix;
}

Eigen::Matrix4f Camera::GetModelViewMatrix() const
{
  return (m_pose * m_transform).inverse().matrix();
}

Eigen::Matrix4f Camera::GetMatrix() const
{
  return GetProjectionMatrix() * GetModelViewMatrix();
}

Camera::Intrinsics Camera::GetIntrinsics() const
{
  return m_intrinsics;
}

Eigen::Matrix4f Camera::ComputeProjectionMatrix() const
{
  const Intrinsics& i = m_intrinsics;
  float l = -i.cx * i.near / i.fx;
  float t =  i.cy * i.near / i.fy;
  float r = (i.w  - i.cx) * i.near / i.fx;
  float b = (i.cy - i.h)  * i.near / i.fy;

  Eigen::Matrix4f matrix = Eigen::Matrix4f::Zero();
  matrix(0, 0) = 2 * i.near / (r - l);
  matrix(1, 1) = 2 * i.near / (t - b);
  matrix(2, 2) = -(i.far + i.near) / (i.far - i.near);
  matrix(2, 3) = -2 * (i.far * i.near) / (i.far - i.near);
  matrix(0, 2) = (r + l) / (r - l);
  matrix(1, 2) = (t + b) / (t - b);
  matrix(3, 2) = -1.0;
  return matrix;
}

void Camera::InitTransform()
{
  Eigen::Matrix3f R;

  R <<  0,  0, -1,
        1,  0,  0,
        0, -1,  0;

  Eigen::Vector3f t(0, 0, 0);

  m_transform = Sophus::SE3f(R, t);
}
