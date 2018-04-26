#ifndef PATHVIZ_TEXTUREGENERATOR_H
#define PATHVIZ_TEXTUREGENERATOR_H

#include <memory>
#include <vector>
#include <pathviz/Types.h>
#include <pathviz/Program.h>
#include <pathviz/Texture.h>

namespace pathviz
{

////////////////////////////////////////////////////////////////////////////////

class TexturePainter
{
  public: struct Options
    {
      uint iterations = 1;
      float maxAmp = 0.5;
      float minAmp = 0.5;
      float maxSig = 0.5;
      float minSig = 0.5;
      bool last_blank = false;
    };

  public: struct Gaussian
    {
      float amp = 1.0;
      float sx  = 1.0;
      float sy  = 1.0;
      float cx  = 0.0;
      float cy  = 0.0;
      float th  = 0.0;
    };

  public: TexturePainter(Texture2DArrayPtr textures);

  public: ~TexturePainter();

  public: void AddConfiguration(const Options& options);

  public: void Paint();

  public: void Paint(uint layer);

  public: void PaintBlank(uint layer);

  protected: void Paint(const Options& options, uint layer);

  protected: void GetGaussian(const Options& options, Gaussian& gaussian);

  protected: void AddGaussian(const Gaussian& gaussian, uint layer);

  private: void Initialize();

  private: void InitVertices();

  private: void InitFrameBuffer();

  private: void InitProgram();

  private: void CreateProgram();

  private: void AssignAttributes();

  private: void AssignUniforms();

  protected: Texture2DArrayPtr m_textures;

  protected: TextureVertexArrayPtr m_vertices;

  protected: TextureFrameBufferPtr m_frameBuffer;

  protected: ProgramPtr m_program;

  protected: std::vector<Options> m_options;
};

////////////////////////////////////////////////////////////////////////////////

class TextureGenerator
{
  public: TextureGenerator(const Texture2DArray::Options& options);

  public: ~TextureGenerator();

  public: Texture2DArrayPtr Generate();

  protected: void AddConfigurations(TexturePainter& painter);

  protected: Texture2DArray::Options m_options;
};

////////////////////////////////////////////////////////////////////////////////

}

#endif
