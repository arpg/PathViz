#include <pathviz/VertexArray.h>

using namespace pathviz;

template <typename Vertex>
VertexArray<Vertex>::VertexArray() :
  m_id(GenerateId()),
  m_vertId(GenerateBufferId()),
  m_idxId(GenerateBufferId()),
  m_updated(true)
{
}

template <typename Vertex>
VertexArray<Vertex>::~VertexArray()
{
  glDeleteVertexArrays(1, &m_id);
  glDeleteBuffers(1, &m_vertId);
  glDeleteBuffers(1, &m_idxId);
}

template <typename Vertex>
void VertexArray<Vertex>::SetActive()
{
  glBindVertexArray(m_id);
  glBindBuffer(GL_ARRAY_BUFFER, m_vertId);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_idxId);
}

template <typename Vertex>
void VertexArray<Vertex>::Add(const Quad<Vertex>& quad)
{
  uint offset = m_vertices.size();

  // add each vertex
  for (int i = 0; i < 4; ++i)
  {
    m_vertices.push_back(quad.vertices[i]);
  }

  // add indices for each triangle
  m_indices.push_back(Eigen::Vector3i(offset + 0, offset + 1, offset + 2));
  m_indices.push_back(Eigen::Vector3i(offset + 0, offset + 2, offset + 3));
  m_updated = true;
}

template <typename Vertex>
void VertexArray<Vertex>::Draw()
{
  PrepareDraw();
  const uint count = 3 * m_indices.size();
  glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);
}

template <typename Vertex>
void VertexArray<Vertex>::PrepareDraw()
{
  SetActive();

  // check if upload needed
  if (m_updated)
  {
    UploadVertices();
    UploadIndices();
    m_updated = false;
  }
}

template <typename Vertex>
void VertexArray<Vertex>::UploadVertices()
{
  void* data = (void*)&m_vertices[0];
  uint size = sizeof(Vertex) * m_vertices.size();
  glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

template <typename Vertex>
void VertexArray<Vertex>::UploadIndices()
{
  void* data = (void*)&m_indices[0];
  uint size = sizeof(Eigen::Vector3i) * m_indices.size();
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

template <typename Vertex>
GLuint VertexArray<Vertex>::GenerateId()
{
  GLuint id;
  glGenVertexArrays(1, &id);
  return id;
}

template <typename Vertex>
GLuint VertexArray<Vertex>::GenerateBufferId()
{
  GLuint id;
  glGenBuffers(1, &id);
  return id;
}

template class pathviz::VertexArray<TextureVertex>;
template class pathviz::VertexArray<SceneVertex>;
