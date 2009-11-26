#ifndef SCENE_FACTORY_H
#define SCENE_FACTORY_H

#include <osg/Geode>
#include <osg/Vec3f>
#include <osg/Node>
#include "avreader.h"
#include "scenefactory.h"

namespace av {
    /* A class with pointers to the relevant transforms in the scene
     */
    class SceneTransforms
    {
    public:
      osg::ref_ptr<osg::Group> root;
      osg::ref_ptr<osg::Group> ground;
      std::map< std::string, osg::ref_ptr<osg::MatrixTransform> > matrices;
      std::map< std::string, osg::ref_ptr<osg::MatrixTransform> > translates;
      std::map< std::string, osg::ref_ptr<osg::MatrixTransform> > wrenches;
    };

    class SceneFactory
    {
    public:
      SceneFactory();
      SceneTransforms initScene(const av::FrameData fData) const;
      //SceneTransforms initScene(const FrameData fData, const std::string filename) const;

    private:
      static osg::ref_ptr<osg::Geode> createLine();
      static osg::ref_ptr<osg::Geode> createBasis();
      static osg::MatrixTransform* createMatrixTransform(const std::string name, osg::Node* child);
      osg::Node* createGrid() const;
      osg::Group* createGround() const;
      osg::MatrixTransform* createMarker(const std::string name) const;
      osg::MatrixTransform* createForce(const std::string name) const;
      //osg::MatrixTransform* createTorque(const std::string name);
      osg::ref_ptr<osg::Geode> markerSphere;
      osg::ref_ptr<osg::Geode> forceLine;
      osg::ref_ptr<osg::Geode> torqueLine;
      osg::ref_ptr<osg::Geode> basisLines;
      static const float RADIUS = .005f;
      static const float GRID_SIZE = 200;
    };
}
#endif
