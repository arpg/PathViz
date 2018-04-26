#include <pathviz/AFSceneBuilder.h>
#include <fstream>
#include <pathviz/Exception.h>
#include <pathviz/Util.h>

namespace pathviz
{

AFSceneBuilder::AFSceneBuilder(calibu::RigPtr rig, PathPtr path, const std::string& file) :
  SceneBuilder(rig, path),
  m_sceneFile(file)
{
}

AFSceneBuilder::AFSceneBuilder(const std::vector<calibu::RigPtr>& rigs,
    PathPtr path, const std::string& file) :
  SceneBuilder(rigs, path),
  m_sceneFile(file)
{
}

AFSceneBuilder::~AFSceneBuilder()
{
}

ScenePtr AFSceneBuilder::Build()
{
  ScenePtr scene(new Scene());
  std::ifstream stream(m_sceneFile);
  if (!stream.is_open()) throw Exception("unable to open scene file");
  Eigen::AlignedBox3f bounds;
  std::string line;

  while (stream.good())
  {
    if (std::getline(stream, line))
    {
      Eigen::Vector3f center;
      Eigen::Vector3f size;
      int textured;
      char delim;

      std::stringstream tokenizer(line);

      tokenizer >> center[0];
      tokenizer >> delim;
      tokenizer >> center[1];
      tokenizer >> delim;
      tokenizer >> center[2];
      tokenizer >> delim;

      tokenizer >> size[0];
      tokenizer >> delim;
      tokenizer >> size[1];
      tokenizer >> delim;
      tokenizer >> size[2];
      tokenizer >> delim;

      tokenizer >> textured;

      bounds.setEmpty();
      bounds.extend(center - size / 2);
      bounds.extend(center + size / 2);

      const Box box = textured ? CreateBox(bounds) : CreateBlankBox(bounds);
      scene->AddBox(box);
    }
    else
    {
      break;
    }
  }

  stream.close();
  return scene;
}

Box AFSceneBuilder::CreateBlankBox(const Eigen::AlignedBox3f& bounds) const
{
  Box box;
  box.bounds = bounds;
  box.inverted = false;
  box.blank = true;

  for (uint i = 0; i < 6; ++i)
  {
    for (uint j = 0; j < 2; ++j)
    {
      box.faces[i].textures[j] = m_texCount - 1;
      box.faces[i].rotations[j] = 0;
    }
  }

  return box;
}

} // namespace pathviz