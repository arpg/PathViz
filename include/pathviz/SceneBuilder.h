#ifndef PATHVIZ_SCENEBUILDER_H
#define PATHVIZ_SCENEBUILDER_H

#include <Eigen/Geometry>
#include <pathviz/Types.h>
#include <pathviz/Scene.h>
#include <pathviz/Path.h>

namespace pathviz
{

////////////////////////////////////////////////////////////////////////////////

class SceneGrid
{
  public: SceneGrid(const Eigen::AlignedBox3f& bounds, uint maxDim);

  public: ~SceneGrid();

  public: void TurnOff(const Eigen::AlignedBox3f& bounds);

  public: void TurnOnRemaining(float prob);

  public: uint GetCellCount() const;

  public: bool CellIsOn(uint index) const;

  public: Eigen::AlignedBox3f GetCellBounds(uint index) const;

  public: uint GetIndex(const Eigen::Vector3i indices) const;

  public: Eigen::Vector3i GetIndices(uint index) const;

  public: Eigen::Vector3i GetIndices(const Eigen::Vector3f point) const;

  private: void Initialize(const Eigen::AlignedBox3f& bounds, uint maxDim);

  protected: Eigen::Vector3i m_gridDim;

  protected: Eigen::Vector3f m_gridMin;

  protected: float m_cellSize;

  protected: uchar* m_grid;

  protected: static const uchar CELL_FREE = 0;

  protected: static const uchar CELL_OFF = 1;

  protected: static const uchar CELL_ON = 2;
};

////////////////////////////////////////////////////////////////////////////////

class SceneBuilder
{
  public: SceneBuilder(calibu::RigPtr rig, PathPtr path);

  public: ~SceneBuilder();

  public: void SetRandomSeed(uint seed);

  public: void SetTextureCount(uint count);

  public: ScenePtr Build();

  protected: void GetRigBounds(Eigen::AlignedBox3f& bounds, const Pose& pose,
                 float radius) const;

  protected: float GetRigRadius() const;

  protected: inline Box CreateBox(const Eigen::AlignedBox3f& bounds) const;

  protected: inline void CreateFace(Box& box, uint face) const;

  protected: inline void CreateFace(Box& box, uint face, uint texture) const;

  protected: calibu::RigPtr m_rig;

  protected: PathPtr m_path;

  protected: uint m_seed;

  protected: uint m_texCount;
};

////////////////////////////////////////////////////////////////////////////////

}

#endif
