#include <osg/Notify>
#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/PolygonMode>

//#include <string>
#include "scenefactory.h"

//Matrix::operator osg::Matrix () const {
//    return osg::Matrix(data);
//}

namespace av {
SceneFactory::SceneFactory()
{
        /* Create a sphere  that will be used for all markers*/
        markerSphere = new osg::Geode();
        markerSphere->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0f,0.0f,0.0f), RADIUS)));
        torqueLine = createLine();
        forceLine = createLine();
        basisLines = createBasis();
}

/**
 * Create a square grid.
 */
osg::Node* SceneFactory::createGrid() const
{
  int numTilesX = 8;
  int numTilesY = 8;
  float width = GRID_SIZE * RADIUS;
  osg::Vec3 center(0.0f,0.0f,0.0f);
  osg::Vec3 v000(center - osg::Vec3(width*0.5f,width*0.5f,0.0f));
  osg::Vec3 dx(osg::Vec3(width/((float)numTilesX),0.0,0.0f));
  osg::Vec3 dy(osg::Vec3(0.0f,width/((float)numTilesY),0.0f));

  // fill in vertices for grid, note numTilesX+1 * numTilesY+1...
  osg::Vec3Array* coords = new osg::Vec3Array;
  int iy;
  for(iy=0;iy<=numTilesY;++iy)
  {
       for(int ix=0;ix<=numTilesX;++ix)
       {
           coords->push_back(v000+dx*(float)ix+dy*(float)iy);
       }
  }

  int numIndicesPerRow=numTilesX+1;
  osg::UByteArray* coordIndices = new osg::UByteArray;
  for(iy=0;iy<numTilesY;++iy)
  {
      for(int ix=0;ix<numTilesX;++ix)
      {
          // four vertices per quad.
          coordIndices->push_back(ix    +(iy+1)*numIndicesPerRow);
          coordIndices->push_back(ix    +iy*numIndicesPerRow);
          coordIndices->push_back((ix+1)+iy*numIndicesPerRow);
          coordIndices->push_back((ix+1)+(iy+1)*numIndicesPerRow);
      }
  }

  // set up a single normal
  osg::Vec3Array* normals = new osg::Vec3Array;
  normals->push_back(osg::Vec3(0.0f,0.0f,1.0f));

  //geometry
  osg::Geometry* geom = new osg::Geometry;
  geom->setVertexArray(coords);
  geom->setVertexIndices(coordIndices);
  geom->setNormalArray(normals);
  geom->setNormalBinding(osg::Geometry::BIND_OVERALL);
  geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,coordIndices->size()));

  osg::Geode* geode = new osg::Geode;
  geode->addDrawable(geom);

  // set the polygon mode for the geode (make it draw lines)
  osg::StateSet* pStateSet = geode->getOrCreateStateSet();
  pStateSet->setAttribute( new osg::PolygonMode(osg::PolygonMode::FRONT_AND_BACK,osg::PolygonMode::LINE) );

  return geode;
}

osg::MatrixTransform* SceneFactory::createForce(const std::string name) const
{
  // Scale the force vector
  osg::MatrixTransform* transform = new osg::MatrixTransform;
  transform->setName("force_norm");
  transform->setDataVariance(osg::Object::DYNAMIC);
  transform->setMatrix(osg::Matrix::identity());
  transform->addChild(forceLine);

}

/**
 * Add a marker to the scene. It will be drawn as a sphere.
 */
osg::MatrixTransform* SceneFactory::createMarker(const std::string name) const
{
    osg::MatrixTransform* transform = createMatrixTransform(name, basisLines);
    return transform;
}

osg::MatrixTransform* SceneFactory::createMatrixTransform(const std::string name, osg::Node* child)
 {
    osg::MatrixTransform* transform = new osg::MatrixTransform;
    transform->setName(name);
    transform->setDataVariance(osg::Object::DYNAMIC);
    transform->setMatrix(osg::Matrix::identity());
    transform->addChild(child);
    return transform;
 }

/**
 * Create a line.
 */
osg::ref_ptr<osg::Geode> SceneFactory::createLine()
{
  // inspired from the osggeometry example.
  osg::ref_ptr<osg::Geode> geode = new osg::Geode();

  osg::Geometry* linesGeom = new osg::Geometry();

  osg::Vec3Array* vertices = new osg::Vec3Array(2);
  (*vertices)[0].set(0, 0, 0);
  (*vertices)[1].set(0, 0, 1);

  linesGeom->setVertexArray(vertices);

  // set the colors.
  osg::Vec4Array* colors = new osg::Vec4Array;
  colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
  linesGeom->setColorArray(colors);
  linesGeom->setColorBinding(osg::Geometry::BIND_OVERALL);

  // set the normal in the same way color.
  osg::Vec3Array* normals = new osg::Vec3Array;
  normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
  linesGeom->setNormalArray(normals);
  linesGeom->setNormalBinding(osg::Geometry::BIND_OVERALL);

  linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,2));
  geode->addDrawable(linesGeom);
  return geode.get();
}

/**
 * Create a frame as 3 lines.
 */
osg::ref_ptr<osg::Geode> SceneFactory::createBasis()
{
        // inspired from the osganimationsolid example
    osg::ref_ptr<osg::Geode> geode (new osg::Geode());
    osg::ref_ptr<osg::Geometry> geometry (new osg::Geometry());

    osg::ref_ptr<osg::Vec3Array> vertices (new osg::Vec3Array());
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 1.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 1.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 0.0));
    vertices->push_back (osg::Vec3 ( 0.0, 0.0, 1.0));
    geometry->setVertexArray (vertices.get());

    osg::ref_ptr<osg::Vec4Array> colors (new osg::Vec4Array());
    colors->push_back (osg::Vec4 (1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (1.0f, 0.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 1.0f, 0.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 0.0f, 1.0f, 1.0f));
    colors->push_back (osg::Vec4 (0.0f, 0.0f, 1.0f, 1.0f));
    geometry->setColorArray (colors.get());

    geometry->setColorBinding (osg::Geometry::BIND_PER_VERTEX);
    geometry->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,6));

    geode->addDrawable( geometry.get() );
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING, false);
    return geode.get();
}

SceneTransforms SceneFactory::initScene(const FrameData fData) const
{
    SceneTransforms sc;
    /* The root node of everything */
    sc.root = new osg::Group;

    /* The root node of the scene */
    sc.ground = new osg::Group;
    sc.ground->setName("ground");
    sc.root->addChild(sc.ground);
    SceneFactory factory;
    osg::Node* grid = factory.createGrid();
    sc.ground->addChild(grid);
    for (std::map< std::string, Matrix > ::const_iterator im(fData.matrices.begin());
         im != fData.matrices.end();
         im++) {
        sc.matrices[im->first] = createMarker(im->first);
        sc.matrices[im->first]->setName(im->first);
        sc.ground->addChild(sc.matrices[im->first]);
    }
    for (std::map< std::string, Translate > ::const_iterator it(fData.translates.begin());
         it != fData.translates.end();
         it++) {
        sc.translates[it->first] = createMarker(it->first);
        sc.translates[it->first]->setName(it->first);
        sc.ground->addChild(sc.translates[it->first]);
    }
    for (std::map< std::string, Wrench > ::const_iterator iw(fData.wrenches.begin());
         iw != fData.wrenches.end();
         iw++) {
        //TODO
    }
    return sc;
}
}
//OsgScene initScene(const std::string& filename) const;
//{
    //osg::ref_ptr<osg::Node> osgDB::readNodeFile(filename);

//}
