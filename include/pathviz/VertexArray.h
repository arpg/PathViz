#ifndef PATHVIZ_VERTEXARRAY_H
#define PATHVIZ_VERTEXARRAY_H

#include <vector>
#include <Eigen/Eigen>
#include <pathviz/OpenGL.h>

namespace pathviz
{

struct TextureVertex
{
  Eigen::Vector2f position;
};

struct SceneVertex
{
  Eigen::Vector3f position;
  Eigen::Vector3f texCoords[2];
};

template <typename Vertex>
struct Quad
{
  Vertex vertices[4];
};

template <typename Vertex>
class VertexArray
{
  public: VertexArray();

  public: ~VertexArray();

  public: void SetActive();

  public: void Add(const Quad<Vertex>& quad);

  public: void Draw();

  protected: void PrepareDraw();

  protected: void UploadVertices();

  protected: void UploadIndices();

  protected: static GLuint GenerateId();

  protected: static GLuint GenerateBufferId();

  protected: const GLuint m_id;

  protected: const GLuint m_vertId;

  protected: const GLuint m_idxId;

  protected: std::vector<Vertex> m_vertices;

  protected: std::vector<Eigen::Vector3i> m_indices;

  protected: bool m_updated;
};

}

#endif