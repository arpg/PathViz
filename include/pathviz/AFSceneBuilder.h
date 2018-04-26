#ifndef PATHVIZ_AFSCENEBUILDER_H
#define PATHVIZ_AFSCENEBUILDER_H

#include <string>
#include <vector>
#include <Eigen/Geometry>
#include <pathviz/Path.h>
#include <pathviz/SceneBuilder.h>
#include <pathviz/Scene.h>
#include <pathviz/Types.h>

namespace pathviz
{

class AFSceneBuilder : public SceneBuilder
{
  public: AFSceneBuilder(calibu::RigPtr rig, PathPtr path,
      const std::string& file);

  public: AFSceneBuilder(const std::vector<calibu::RigPtr>& rigs, PathPtr path, const std::string& file);

  public: virtual ~AFSceneBuilder();

  public: ScenePtr Build() override;

  protected: Box CreateBlankBox(const Eigen::AlignedBox3f& bounds) const;

  protected: std::vector<Eigen::Vector3f> m_centroids;

  protected: std::string m_sceneFile;
};

} // namespace pathviz

#endif