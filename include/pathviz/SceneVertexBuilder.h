#ifndef PATHVIZ_SCENEVERTEXBUILDER_H
#define PATHVIZ_SCENEVERTEXBUILDER_H

#include <pathviz/Types.h>
#include <pathviz/Scene.h>
#include <pathviz/VertexArray.h>

namespace pathviz
{

class SceneVertexBuilder
{
  typedef Quad<SceneVertex> SceneQuad;

  public: SceneVertexBuilder();

  public: ~SceneVertexBuilder();

  public: SceneVertexArrayPtr Build(ScenePtr scene);

  protected: void AddBox(SceneVertexArrayPtr vertices, const Box& box);

  protected: void BuildQuad(SceneQuad& quad, const Box& box, uint face);

  protected: void AddPosition(SceneQuad& quad, const Box& box, uint face);

  protected: void AddTexture(SceneQuad& quad, const Box& box, uint face,
                 uint index);

  protected: void GetTextureIndices(uint* indices, uint rotation);
};

}

#endif