#ifndef PATHVIZ_SCENE_H
#define PATHVIZ_SCENE_H

#include <vector>
#include <Eigen/Geometry>
#include <pathviz/Types.h>

namespace pathviz
{

struct Face
{
  uint textures[2];
  uchar rotations[2];
};

struct Box
{
  Box()
  {
  }

  inline void SetPosition(float x, float y, float z)
  {
    SetPosition(Eigen::Vector3f(x, y, z));
  }

  inline void SetPosition(const Eigen::Vector3f& position)
  {
    const Eigen::Vector3f& size = bounds.sizes();
    bounds.setEmpty();
    bounds.extend(position - size / 2);
    bounds.extend(position + size / 2);
  }

  inline void SetSize(float x, float y, float z)
  {
    SetSize(Eigen::Vector3f(x, y, z));
  }

  inline void SetSize(const Eigen::Vector3f& size)
  {
    const Eigen::Vector3f& position = bounds.center();
    bounds.setEmpty();
    bounds.extend(position - size / 2);
    bounds.extend(position + size / 2);
  }

  Eigen::AlignedBox3f bounds;
  Face faces[6];
  bool inverted;
  bool blank;
};

class Scene
{
  public: Scene();

  public: ~Scene();

  public: void Clear();

  public: void AddBox(const Box& box);

  public: uint GetBoxCount() const;

  public: Box GetBox(uint index) const;

  public: uint GetTextureCount() const;

  private: inline void UpdateTextureCount(const Box& box);

  private: inline void UpdateTextureCount(const Face& face);

  protected: std::vector<Box> m_boxes;

  protected: uint m_texCount;
};

}

#endif