#include <pathviz/SceneBuilder.h>
#include <pathviz/Util.h>

using namespace pathviz;

////////////////////////////////////////////////////////////////////////////////

const uchar SceneGrid::CELL_FREE;
const uchar SceneGrid::CELL_OFF;
const uchar SceneGrid::CELL_ON;

SceneGrid::SceneGrid(const Eigen::AlignedBox3f& bounds, uint maxDim) :
  m_gridDim(0, 0, 0),
  m_cellSize(0),
  m_grid(nullptr)
{
  Initialize(bounds, maxDim);
}

SceneGrid::~SceneGrid()
{
}

void SceneGrid::TurnOff(const Eigen::AlignedBox3f& bounds)
{
  Eigen::Vector3i start = GetIndices(bounds.min());
  Eigen::Vector3i stop = GetIndices(bounds.max());

  start[0] = fmax(0, start[0]);
  start[1] = fmax(0, start[1]);
  start[2] = fmax(0, start[2]);

  stop[0] = fmin(m_gridDim[0] - 1, stop[0]);
  stop[1] = fmin(m_gridDim[1] - 1, stop[1]);
  stop[2] = fmin(m_gridDim[2] - 1, stop[2]);

  for (int z = start[2]; z <= stop[2]; ++z)
  {
    for (int y = start[1]; y <= stop[1]; ++y)
    {
      for (int x = start[0]; x <= stop[0]; ++x)
      {
        uint index = GetIndex(Eigen::Vector3i(x, y, z));
        m_grid[index] = CELL_OFF;
      }
    }
  }
}

void SceneGrid::TurnOnRemaining(float prob)
{
  uint count = GetCellCount();

  // check all cells in grid
  for (uint i = 0; i < count; ++i)
  {
    // check if cell eligible & selected
    if (m_grid[i] == CELL_FREE && randf() < prob)
    {
      m_grid[i] = CELL_ON;
    }
  }
}

uint SceneGrid::GetCellCount() const
{
  return m_gridDim[0] * m_gridDim[1] * m_gridDim[2];
}

bool SceneGrid::CellIsOn(uint index) const
{
  return m_grid[index] == CELL_ON;
}

Eigen::AlignedBox3f SceneGrid::GetCellBounds(uint index) const
{
  Eigen::AlignedBox3f bounds;
  Eigen::Vector3i indices = GetIndices(index);
  Eigen::Vector3f offset(1, 1, 1);
  bounds.extend(m_gridMin + m_cellSize * indices.cast<float>());
  bounds.extend(m_gridMin + m_cellSize * (indices.cast<float>() + offset));
  return bounds;
}

uint SceneGrid::GetIndex(const Eigen::Vector3i indices) const
{
  uint xy = m_gridDim[0] * m_gridDim[1];
  uint x  = m_gridDim[0];
  return indices[2] * xy + indices[1] * x + indices[0];
}

Eigen::Vector3i SceneGrid::GetIndices(uint index) const
{
  uint xy = m_gridDim[0] * m_gridDim[1];
  uint x  = m_gridDim[0];

  Eigen::Vector3i indices;
  indices[0] = index % xy % x;
  indices[1] = index % xy / x;
  indices[2] = index / xy;
  return indices;
}

Eigen::Vector3i SceneGrid::GetIndices(const Eigen::Vector3f point) const
{
  return ((point - m_gridMin) / m_cellSize).cast<int>();
}

void SceneGrid::Initialize(const Eigen::AlignedBox3f& bounds, uint maxDim)
{
  // compute cell size
  Eigen::Vector3f center = bounds.center();
  Eigen::Vector3f sizes = bounds.sizes();
  float maxSize = sizes.maxCoeff();
  m_cellSize = maxSize / maxDim;

  // compute grid dimensions
  m_gridDim[0] = fmax(1, ceil(maxSize / m_cellSize));
  m_gridDim[1] = fmax(1, ceil(maxSize / m_cellSize));
  m_gridDim[2] = fmax(1, ceil(maxSize / m_cellSize));

  // compute grid minimum position
  m_gridMin = center - (m_cellSize * m_gridDim.cast<float>()) / 2;

  // allocate & initialize grid
  uint cellCount = GetCellCount();
  m_grid = new uchar[cellCount];
  std::fill(m_grid, m_grid + cellCount, CELL_FREE);
}

////////////////////////////////////////////////////////////////////////////////

SceneBuilder::SceneBuilder(calibu::RigPtr rig, PathPtr path) :
  m_path(path),
  m_seed(time(NULL)),
  m_texCount(10)
{
  m_rigs.push_back(rig);
}

SceneBuilder::SceneBuilder(const std::vector<calibu::RigPtr>& rigs,
    PathPtr path) :
  m_rigs(rigs),
  m_path(path),
  m_seed(time(NULL)),
  m_texCount(10)
{
}

SceneBuilder::~SceneBuilder()
{
}

void SceneBuilder::SetRandomSeed(uint seed)
{
  m_seed = seed;
}

void SceneBuilder::SetTextureCount(uint count)
{
  m_texCount = count;
}

ScenePtr SceneBuilder::Build()
{
  srand(m_seed);
  ScenePtr scene(new Scene());

  // get path properties
  Eigen::AlignedBox3f pathBounds = m_path->GetBounds();
  Eigen::Vector3f pathCenter = pathBounds.center();
  Eigen::Vector3f pathSizes = pathBounds.sizes();

  // create small grid
  Eigen::AlignedBox3f smallGridBounds;
  smallGridBounds.extend(pathCenter - 1.2 * pathSizes);
  smallGridBounds.extend(pathCenter + 1.2 * pathSizes);
  //SceneGrid smallGrid(smallGridBounds, 20);
  SceneGrid smallGrid(smallGridBounds, 40);

  // create large grid
  Eigen::AlignedBox3f largeGridBounds;
  largeGridBounds.extend(pathCenter - 8.0 * pathSizes);
  largeGridBounds.extend(pathCenter + 8.0 * pathSizes);
  SceneGrid largeGrid(largeGridBounds, 20);

  float rigRadius = 0.5 + GetMaxRigRadius();
  uint poseCount = m_path->GetPoseCount();
  Eigen::AlignedBox3f poseBounds;

  // process each pose in path
  for (uint i = 0; i < poseCount; ++i)
  {
    // turn off cells that overlap pose
    const Pose& pose = m_path->GetPose(i);
    GetRigBounds(poseBounds, pose, rigRadius);
    smallGrid.TurnOff(poseBounds);
    largeGrid.TurnOff(poseBounds);
  }

  //smallGrid.TurnOnRemaining(0.05);
  // smallGrid.TurnOnRemaining(0.5);
  smallGrid.TurnOnRemaining(1.0);
  uint cellCount = smallGrid.GetCellCount();

  // process each cell in small grid
  for (uint i = 0; i < cellCount; ++i)
  {
    // check if cell is on
    if (smallGrid.CellIsOn(i))
    {
      Eigen::AlignedBox3f cellBounds = smallGrid.GetCellBounds(i);
      largeGrid.TurnOff(cellBounds);
      Box box = CreateBox(cellBounds);
      scene->AddBox(box);
    }
  }

  largeGrid.TurnOnRemaining(0.25);
  cellCount = largeGrid.GetCellCount();

  // process each cell in large grid
//  for (uint i = 0; i < cellCount; ++i)
//  {
//    // check if cell is on
//    if (largeGrid.CellIsOn(i))
//    {
//      Eigen::AlignedBox3f cellBounds = largeGrid.GetCellBounds(i);
//      Box box = CreateBox(cellBounds);
//      scene->AddBox(box);
//    }
//  }

  // create skybox
  Eigen::AlignedBox3f skyBoxBounds;
  float maxSize = pathSizes.maxCoeff();
  Eigen::Vector3f offset(maxSize, maxSize, maxSize);
  skyBoxBounds.extend(pathCenter - 10.0 * offset);
  skyBoxBounds.extend(pathCenter + 10.0 * offset);
  Box box = CreateBox(skyBoxBounds);
  box.inverted = true;
  scene->AddBox(box);

  return scene;
}

void SceneBuilder::GetRigBounds(Eigen::AlignedBox3f& bounds, const Pose& pose,
    float radius) const
{
  bounds.setEmpty();
  Eigen::Vector3f center = pose.pose.translation();
  Eigen::Vector3f offset(radius, radius, radius);
  bounds.extend(center - offset);
  bounds.extend(center + offset);
}

float SceneBuilder::GetMaxRigRadius() const
{
  float maxRadius = 0;

  for (calibu::RigPtr rig : m_rigs)
  {
    float radius = GetRigRadius(rig);

    if (radius > maxRadius)
    {
      maxRadius = radius;
    }
  }

  return maxRadius;
}

float SceneBuilder::GetRigRadius(calibu::RigPtr rig) const
{
  uint count = rig->NumCams();
  double maxDist = 0;

  // process all cameras in rig
  for (uint i = 0; i < count; ++i)
  {
    calibu::CameraPtr camera = rig->cameras_[i];
    Eigen::Vector3d pos = camera->Pose().translation();
    double dist = pos.norm();

    // check if new max
    if (dist > maxDist)
    {
      maxDist = dist;
    }
  }

  return float(maxDist);
}

Box SceneBuilder::CreateBox(const Eigen::AlignedBox3f& bounds) const
{
  Box box;
  box.bounds = bounds;
  box.inverted = false;
  box.blank = false;

  // create each face on box
  for (uint i = 0; i < 6; ++i)
  {
    CreateFace(box, i);
  }

  return box;
}

void SceneBuilder::CreateFace(Box& box, uint face) const
{
  // crate each face texture
  for (uint i = 0; i < 2; ++i)
  {
    CreateFace(box, face, i);
  }
}

void SceneBuilder::CreateFace(Box& box, uint face, uint texture) const
{
  box.faces[face].textures[texture] = rand(0, m_texCount);
  box.faces[face].rotations[texture] = rand(0, 4);
}

////////////////////////////////////////////////////////////////////////////////
