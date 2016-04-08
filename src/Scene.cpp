#include <pathviz/Scene.h>

using namespace pathviz;

Scene::Scene() :
  m_texCount(0)
{
}

Scene::~Scene()
{
}

void Scene::Clear()
{
  m_boxes.clear();
  m_texCount = 0;
}

void Scene::AddBox(const Box& box)
{
  m_boxes.push_back(box);
  UpdateTextureCount(box);
}

uint Scene::GetBoxCount() const
{
  return m_boxes.size();
}

Box Scene::GetBox(uint index) const
{
  return m_boxes[index];
}

uint Scene::GetTextureCount() const
{
  return m_texCount;
}

void Scene::UpdateTextureCount(const Box& box)
{
  // process each face
  for (uint j = 0; j < 6; ++j)
  {
    UpdateTextureCount(box.faces[j]);
  }
}

void Scene::UpdateTextureCount(const Face& face)
{
  // process each texture
  for (uint k = 0; k < 2; ++k)
  {
    uint count = face.textures[k] + 1;

    // check if new max found
    if (count > m_texCount)
    {
      m_texCount = count;
    }
  }
}