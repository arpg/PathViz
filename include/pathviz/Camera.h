#ifndef PATHVIZ_CAMERA_H
#define PATHVIZ_CAMERA_H

#include <sophus/se3.hpp>
#include <pathviz/Types.h>

namespace pathviz
{

class Camera
{
  public: struct Intrinsics
    {
      uint  w = 320;
      uint  h = 240;
      float fx = w / 2;
      float fy = w / 2;
      float cx = w / 2;
      float cy = h / 2;
      float near = 0.1;
      float far = 100.0;
    };

  public: Camera();

  public: Camera(const Intrinsics& intrinsics);

  public: ~Camera();

  public: void Configure(const Intrinsics& intrinsics);

  public: uint GetImageWidth() const;

  public: uint GetImageHeight() const;

  public: Sophus::SE3f GetPose() const;

  public: void SetPose(const Sophus::SE3f& pose);

  public: Eigen::Matrix4f GetProjectionMatrix() const;

  public: Eigen::Matrix4f GetModelViewMatrix() const;

  public: Eigen::Matrix4f GetMatrix() const;

  public: Intrinsics GetIntrinsics() const;

  protected: Eigen::Matrix4f ComputeProjectionMatrix() const;

  private: void InitTransform();

  protected: Intrinsics m_intrinsics;

  protected: Sophus::SE3f m_pose;

  protected: Sophus::SE3f m_transform;

  protected: Eigen::Matrix4f m_projMatrix;
};

}

#endif