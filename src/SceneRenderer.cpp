#include <pathviz/SceneRenderer.h>
#include <pathviz/SceneVertexBuilder.h>
#include <pathviz/Texture.h>
#include <pathviz/TextureGenerator.h>
#include <pathviz/VertexArray.h>

using namespace pathviz;

SceneRenderer::SceneRenderer(ScenePtr scene)
{
  Initialize(scene);
}

SceneRenderer::~SceneRenderer()
{
}

void SceneRenderer::SetCamera(const Camera& camera)
{
  // resize frame buffer
  uint width = camera.GetImageWidth();
  uint height = camera.GetImageHeight();
  m_frameBuffer->Resize(width, height);

  // update camera matrix
  Uniform M = m_program->GetUniform("M");
  M.SetMat4(camera.GetMatrix());
  m_camera = camera;
}

void SceneRenderer::Render(Image& image)
{
  m_frameBuffer->SetActive();
  m_frameBuffer->Clear();
  m_program->SetActive();
  m_vertices->Draw();
  m_frameBuffer->Read(image);
}

void SceneRenderer::Initialize(ScenePtr scene)
{
  InitFrameBuffer();
  InitVertexArray(scene);
  InitTextures(scene);
  InitProgram();
}

void SceneRenderer::InitFrameBuffer()
{
  FrameBuffer* frameBuffer = new FrameBuffer(1024, 1024);
  m_frameBuffer = FrameBufferPtr(frameBuffer);
  m_frameBuffer->SetActive();
}

void SceneRenderer::InitVertexArray(ScenePtr scene)
{
  SceneVertexBuilder builder;
  m_vertices = builder.Build(scene);
}

void SceneRenderer::InitTextures(ScenePtr scene)
{
  Texture2DArray::Options options;
  options.levels = 1;
  options.width  = 256;
  options.height = 256;
  options.layers = scene->GetTextureCount();

  TextureGenerator generator(options);
  m_textures = generator.Generate();
}

void SceneRenderer::InitProgram()
{
  CreateProgram();
  InitProgramAttributes();
  InitProgramUniforms();
}

void SceneRenderer::CreateProgram()
{
  // compile vertex shader
  VertexShader vertShader("triangle.vert");
  vertShader.Compile();

  // compile fragment shader
  FragmentShader fragShader("triangle.frag");
  fragShader.Compile();

  // link program
  m_program = ProgramPtr(new Program());
  m_program->Attach(vertShader);
  m_program->Attach(fragShader);
  m_program->Link();
  m_program->Detach(vertShader);
  m_program->Detach(fragShader);
}

void SceneRenderer::InitProgramAttributes()
{
  // configure attribute options
  Attribute::Options options;
  options.size = 3;
  options.type = GL_FLOAT;
  options.stride = sizeof(SceneVertex);
  options.normalized = false;
  options.pointer = 0;
  uint offset = 0;

  // get attribute locations
  Attribute attributes[] =
  {
    m_program->GetAttribute("in_position"),
    m_program->GetAttribute("in_texCoord1"),
    m_program->GetAttribute("in_texCoord2")
  };

  m_vertices->SetActive();

  // enable & configure each attribute
  for (int i = 0; i < 3; ++i)
  {
    attributes[i].Enable();
    options.pointer = (void*)(uintptr_t)offset;
    attributes[i].Configure(options);
    offset += sizeof(Eigen::Vector3f);
  }
}

void SceneRenderer::InitProgramUniforms()
{
  Uniform textures = m_program->GetUniform("textures");
  textures.SetInt(m_textures->GetId());
}