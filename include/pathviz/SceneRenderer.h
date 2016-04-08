#ifndef PATHVIZ_SCENERENDERER_H
#define PATHVIZ_SCENERENDERER_H

#include <memory>
#include <pathviz/Types.h>
#include <pathviz/Camera.h>
#include <pathviz/FrameBuffer.h>
#include <pathviz/Image.h>
#include <pathviz/Program.h>
#include <pathviz/Scene.h>
#include <pathviz/VertexArray.h>

namespace pathviz
{

class SceneRenderer
{
  public: SceneRenderer(ScenePtr scene);

  public: ~SceneRenderer();

  public: void SetCamera(const Camera& camera);

  public: void Render(Image& image);

  private: void Initialize(ScenePtr scene);

  private: void InitFrameBuffer();

  private: void InitVertexArray(ScenePtr scene);

  private: void InitTextures(ScenePtr scene);

  private: void InitProgram();

  private: void CreateProgram();

  private: void InitProgramAttributes();

  private: void InitProgramUniforms();

  protected: FrameBufferPtr m_frameBuffer;

  protected: ProgramPtr m_program;

  protected: Texture2DArrayPtr m_textures;

  protected: SceneVertexArrayPtr m_vertices;

  protected: Camera m_camera;
};

}

#endif