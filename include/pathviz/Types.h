#ifndef PATHVIZ_TYPES_H
#define PATHVIZ_TYPES_H

#include <memory>
#include <calibu/Calibu.h>

namespace calibu
{
  typedef std::shared_ptr<Rigd> RigPtr;
  typedef std::shared_ptr<CameraInterfaced> CameraPtr;
}

namespace pathviz
{

class SceneVertex;
class TextureVertex;

template <typename Vertex>
class VertexArray;

typedef VertexArray<SceneVertex>   SceneVertexArray;
typedef VertexArray<TextureVertex> TextureVertexArray;

class FrameBuffer;
class Path;
class Program;
class Rig;
class RigCamera;
class Scene;
class SceneRenderer;
class Texture2DArray;
class TextureFrameBuffer;

typedef std::shared_ptr<FrameBuffer>        FrameBufferPtr;
typedef std::shared_ptr<Path>               PathPtr;
typedef std::shared_ptr<Program>            ProgramPtr;
typedef std::shared_ptr<Rig>                RigPtr;
typedef std::shared_ptr<RigCamera>          RigCameraPtr;
typedef std::shared_ptr<Scene>              ScenePtr;
typedef std::shared_ptr<SceneRenderer>      SceneRendererPtr;
typedef std::shared_ptr<SceneVertexArray>   SceneVertexArrayPtr;
typedef std::shared_ptr<Texture2DArray>     Texture2DArrayPtr;
typedef std::shared_ptr<TextureVertexArray> TextureVertexArrayPtr;
typedef std::shared_ptr<TextureFrameBuffer> TextureFrameBufferPtr;

typedef unsigned int uint;
typedef unsigned char uchar;

}

#endif