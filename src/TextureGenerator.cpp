#include <pathviz/TextureGenerator.h>
#include <cmath>
#include <pathviz/Util.h>
#include <pathviz/FrameBuffer.h>
#include <pathviz/VertexArray.h>

using namespace pathviz;

////////////////////////////////////////////////////////////////////////////////

TexturePainter::TexturePainter(Texture2DArrayPtr textures) :
  m_textures(textures),
  m_vertices(nullptr),
  m_program(nullptr)
{
  Initialize();
}

TexturePainter::~TexturePainter()
{
}

void TexturePainter::AddConfiguration(const TexturePainter::Options& options)
{
  m_options.push_back(options);
}

void TexturePainter::Paint()
{
  Texture2DArray::Options options = m_textures->GetOptions();

  // paint each layer in texture array
  for (uint i = 0; i < options.layers; ++i)
  {
    Paint(i);
  }
}

void TexturePainter::Paint(uint layer)
{
  m_frameBuffer->SetLayer(layer);
  m_frameBuffer->Clear();

  Uniform index = m_program->GetUniform("index");
  index.SetInt(layer);

  // paint with each configuration
  for (const Options& options : m_options)
  {
    Paint(options, layer);
  }
}

void TexturePainter::Paint(const Options& options, uint layer)
{
  Gaussian gaussian;

  // paint each iterations
  for (uint i = 0; i < options.iterations; ++i)
  {
    GetGaussian(options, gaussian);
    AddGaussian(gaussian, layer);
  }
}

void TexturePainter::GetGaussian(const Options& options, Gaussian& gaussian)
{
  const Texture2DArray::Options& texOptions = m_textures->GetOptions();
  gaussian.amp = rand(options.minAmp, options.maxAmp);
  gaussian.sx  = rand(options.minSig, options.maxSig);
  gaussian.sy  = rand(options.minSig, options.maxSig);
  gaussian.cx  = rand(-0.5f * texOptions.width,  1.5f * texOptions.width);
  gaussian.cy  = rand(-0.5f * texOptions.height, 1.5f * texOptions.height);
  gaussian.th  = rand(0.0f, M_PI);
}

void TexturePainter::AddGaussian(const Gaussian& gaussian, uint layer)
{
  // assign gaussian center
  Uniform center = m_program->GetUniform("center");
  center.SetFloat2(gaussian.cx, gaussian.cy);

  // assign gaussian sigmas
  Uniform sig = m_program->GetUniform("sig");
  sig.SetFloat2(gaussian.sx, gaussian.sy);

  // assign gaussian amplitude
  Uniform amp = m_program->GetUniform("amp");
  amp.SetFloat(gaussian.amp);

  // construct rotation matrix
  Eigen::Matrix2f mat = Eigen::Matrix2f::Zero();
  mat(0, 0) =  cos(gaussian.th);
  mat(1, 0) =  sin(gaussian.th);
  mat(1, 1) =  mat(0, 0);
  mat(0, 1) = -mat(1, 0);

  // assign gaussian rotation
  Uniform R = m_program->GetUniform("R");
  R.SetMat2(mat);

  // render gaussian to texture
  m_frameBuffer->ClearDepth();
  m_program->SetActive();
  m_vertices->Draw();
}

void TexturePainter::Initialize()
{
  InitVertices();
  InitFrameBuffer();
  InitProgram();
}

void TexturePainter::InitVertices()
{
  m_vertices = TextureVertexArrayPtr(new TextureVertexArray());

  Quad<TextureVertex> quad;
  quad.vertices[0].position = Eigen::Vector2f( 1, -1);
  quad.vertices[1].position = Eigen::Vector2f( 1,  1);
  quad.vertices[2].position = Eigen::Vector2f(-1,  1);
  quad.vertices[3].position = Eigen::Vector2f(-1, -1);
  m_vertices->Add(quad);
}

void TexturePainter::InitFrameBuffer()
{
  m_frameBuffer = std::make_shared<TextureFrameBuffer>(m_textures);
}

void TexturePainter::InitProgram()
{
  CreateProgram();
  AssignAttributes();
  AssignUniforms();
}

void TexturePainter::CreateProgram()
{
  // compile vertex shader
  VertexShader vertShader("texture.vert");
  vertShader.Compile();

  // compile fragment shader
  FragmentShader fragShader("texture.frag");
  fragShader.Compile();

  // link program
  m_program = ProgramPtr(new Program());
  m_program->Attach(vertShader);
  m_program->Attach(fragShader);
  m_program->Link();
  m_program->Detach(vertShader);
  m_program->Detach(fragShader);
}

void TexturePainter::AssignAttributes()
{
  m_vertices->SetActive();

  Attribute::Options attrOpts;
  attrOpts.size = 2;
  attrOpts.type = GL_FLOAT;
  attrOpts.normalized = GL_FALSE;
  attrOpts.stride = sizeof(TextureVertex);
  attrOpts.pointer = 0;

  Attribute in_position = m_program->GetAttribute("in_position");
  in_position.Enable();
  in_position.Configure(attrOpts);
}

void TexturePainter::AssignUniforms()
{
  const Texture2DArray::Options& options = m_textures->GetOptions();

  Uniform current = m_program->GetUniform("current");
  current.SetInt(m_textures->GetId());

  Uniform size = m_program->GetUniform("size");
  size.SetFloat2(options.width, options.height);
}

////////////////////////////////////////////////////////////////////////////////

TextureGenerator::TextureGenerator(const Texture2DArray::Options& options) :
  m_options(options)
{
}

TextureGenerator::~TextureGenerator()
{
}

Texture2DArrayPtr TextureGenerator::Generate()
{
  Texture2DArrayPtr textures(new Texture2DArray(m_options));
  textures->SetParameter(GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  textures->SetParameter(GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  textures->SetParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  textures->SetParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

  TexturePainter painter(textures);
  AddConfigurations(painter);
  painter.Paint();

  textures->SetParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  textures->SetParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  return textures;
}

void TextureGenerator::AddConfigurations(TexturePainter& painter)
{
  TexturePainter::Options painterOptions;
  float maxDim = fmax(m_options.width, m_options.height);
  float maxDimSq = maxDim * maxDim;

  painterOptions.iterations = 20;
  painterOptions.maxAmp = 0.25;
  painterOptions.minAmp = 0.10;
  painterOptions.maxSig = 0.050 * maxDimSq;
  painterOptions.minSig = 0.025 * maxDimSq;
  painter.AddConfiguration(painterOptions);

  painterOptions.iterations = 20;
  painterOptions.maxAmp = 0.50;
  painterOptions.minAmp = 0.25;
  painterOptions.maxSig = 0.025 * maxDimSq;
  painterOptions.minSig = 0.010 * maxDimSq;
  painter.AddConfiguration(painterOptions);

  painterOptions.iterations = 50;
  painterOptions.maxAmp = 0.75;
  painterOptions.minAmp = 0.50;
  painterOptions.maxSig = 0.0005 * maxDimSq;
  painterOptions.minSig = 0.0001 * maxDimSq;
  painter.AddConfiguration(painterOptions);

  painterOptions.iterations = 50;
  painterOptions.maxAmp = 0.75;
  painterOptions.minAmp = 0.50;
  painterOptions.maxSig = 0.00010 * maxDimSq;
  painterOptions.minSig = 0.00005 * maxDimSq;
  painter.AddConfiguration(painterOptions);

  painterOptions.iterations = 50;
  painterOptions.maxAmp = -0.75;
  painterOptions.minAmp = -1.00;
  painterOptions.maxSig = 0.0005 * maxDimSq;
  painterOptions.minSig = 0.0001 * maxDimSq;
  painter.AddConfiguration(painterOptions);

  painterOptions.iterations = 50;
  painterOptions.maxAmp = -0.75;
  painterOptions.minAmp = -1.00;
  painterOptions.maxSig = 0.00010 * maxDimSq;
  painterOptions.minSig = 0.00005 * maxDimSq;
  painter.AddConfiguration(painterOptions);
}

////////////////////////////////////////////////////////////////////////////////
