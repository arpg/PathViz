#include <pathviz/SceneVertexBuilder.h>

using namespace pathviz;

SceneVertexBuilder::SceneVertexBuilder()
{
}

SceneVertexBuilder::~SceneVertexBuilder()
{
}

SceneVertexArrayPtr SceneVertexBuilder::Build(ScenePtr scene)
{
  SceneVertexArrayPtr vertices(new SceneVertexArray());
  uint count = scene->GetBoxCount();

  // add vertices for each box
  for (uint i = 0; i < count; ++i)
  {
    AddBox(vertices, scene->GetBox(i));
  }

  return vertices;
}

void SceneVertexBuilder::AddBox(SceneVertexArrayPtr vertices, const Box& box)
{
  SceneQuad quad;

  // add vertices for each face
  for (uint i = 0; i < 6; ++i)
  {
    BuildQuad(quad, box, i);
    vertices->Add(quad);
  }
}

void SceneVertexBuilder::BuildQuad(SceneQuad& quad, const Box& box, uint face)
{
  AddPosition(quad, box, face);
  AddTexture(quad, box, face, 0);
  AddTexture(quad, box, face, 1);
}

void SceneVertexBuilder::AddPosition(SceneQuad& quad, const Box& box, uint face)
{
  const Eigen::Vector3f min = box.bounds.min();
  const Eigen::Vector3f max = box.bounds.max();

  switch (face)
  {
    case 0:
      quad.vertices[0].position = Eigen::Vector3f(max[0], min[1], max[2]);
      quad.vertices[1].position = Eigen::Vector3f(max[0], max[1], max[2]);
      quad.vertices[2].position = Eigen::Vector3f(min[0], max[1], max[2]);
      quad.vertices[3].position = Eigen::Vector3f(min[0], min[1], max[2]);
      break;

    case 1:
      quad.vertices[0].position = Eigen::Vector3f(min[0], min[1], min[2]);
      quad.vertices[1].position = Eigen::Vector3f(min[0], max[1], min[2]);
      quad.vertices[2].position = Eigen::Vector3f(max[0], max[1], min[2]);
      quad.vertices[3].position = Eigen::Vector3f(max[0], min[1], min[2]);
      break;

    case 2:
      quad.vertices[0].position = Eigen::Vector3f(max[0], max[1], max[2]);
      quad.vertices[1].position = Eigen::Vector3f(max[0], max[1], min[2]);
      quad.vertices[2].position = Eigen::Vector3f(min[0], max[1], min[2]);
      quad.vertices[3].position = Eigen::Vector3f(min[0], max[1], max[2]);
      break;

    case 3:
      quad.vertices[0].position = Eigen::Vector3f(max[0], min[1], min[2]);
      quad.vertices[1].position = Eigen::Vector3f(max[0], min[1], max[2]);
      quad.vertices[2].position = Eigen::Vector3f(min[0], min[1], max[2]);
      quad.vertices[3].position = Eigen::Vector3f(min[0], min[1], min[2]);
      break;

    case 4:
      quad.vertices[0].position = Eigen::Vector3f(max[0], min[1], min[2]);
      quad.vertices[1].position = Eigen::Vector3f(max[0], max[1], min[2]);
      quad.vertices[2].position = Eigen::Vector3f(max[0], max[1], max[2]);
      quad.vertices[3].position = Eigen::Vector3f(max[0], min[1], max[2]);
      break;

    case 5:
      quad.vertices[0].position = Eigen::Vector3f(min[0], min[1], max[2]);
      quad.vertices[1].position = Eigen::Vector3f(min[0], max[1], max[2]);
      quad.vertices[2].position = Eigen::Vector3f(min[0], max[1], min[2]);
      quad.vertices[3].position = Eigen::Vector3f(min[0], min[1], min[2]);
      break;
  }

  if (box.inverted)
  {
    Eigen::Vector3f temp;

    temp = quad.vertices[0].position;
    quad.vertices[0].position = quad.vertices[3].position;
    quad.vertices[3].position = temp;

    temp = quad.vertices[1].position;
    quad.vertices[1].position = quad.vertices[2].position;
    quad.vertices[2].position = temp;
  }
}

void SceneVertexBuilder::AddTexture(SceneQuad& quad, const Box& box, uint face,
    uint index)
{
  uint i[4];
  uchar texture = box.faces[face].textures[index];
  GetTextureIndices(i, box.faces[face].rotations[index]);
  quad.vertices[i[0]].texCoords[index] = Eigen::Vector3f(1.0, 0.0, texture);
  quad.vertices[i[1]].texCoords[index] = Eigen::Vector3f(1.0, 1.0, texture);
  quad.vertices[i[2]].texCoords[index] = Eigen::Vector3f(0.0, 1.0, texture);
  quad.vertices[i[3]].texCoords[index] = Eigen::Vector3f(0.0, 0.0, texture);
}

void SceneVertexBuilder::GetTextureIndices(uint* indices, uint rotation)
{
  // populate rotated indices
  for (int i = 0; i < 4; ++i)
  {
    indices[i] = (i + rotation) % 4;
  }
}
