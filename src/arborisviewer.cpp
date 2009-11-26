// RTNet C++ includes

#include <osg/MatrixTransform>
#include <osg/PositionAttitudeTransform>
#include <osg/Geometry>
#include <osg/ShapeDrawable>
#include <osg/StateSet>
#include <osg/PolygonMode>
#include <osg/Group>

#include <osgGA/TrackballManipulator>

#include <osgSim/OverlayNode>
#include <osgDB/WriteFile>
#include <osgViewer/Viewer>

#include <fstream>
#include <iostream>
//#include <cstdlib>
#include <ctime>
#include <string>
#include <boost/tr1/memory.hpp>
#include "avreaderh5.h"
#include "scenefactory.h"
#include <boost/program_options.hpp>
const double POS_FACTOR = 0.1;

using namespace av;
namespace po = boost::program_options;        

class MarkerCallback : public osg::NodeCallback
{
private:
    std::tr1::shared_ptr<LastReader> reader;
    SceneTransforms transforms;
public:
    MarkerCallback(std::tr1::shared_ptr<LastReader> _reader,
                   SceneTransforms _transforms);

    virtual void operator()(osg::Node* node, osg::NodeVisitor*)
    {
        FrameData fData = reader->getLatestFrame();
        for (std::map< std::string, Matrix > ::const_iterator im = fData.matrices.begin(); im != fData.matrices.end(); im++) {
            osg::Matrix mat(im->second[0], im->second[4], im->second[8], im->second[12],
                            im->second[1], im->second[5], im->second[9], im->second[13],
                            im->second[2], im->second[6], im->second[10], im->second[14],
                            im->second[3], im->second[7], im->second[11], im->second[15]);
            transforms.matrices[im->first]->setMatrix(mat);
        }
        for (std::map< std::string, Translate > ::const_iterator it = fData.translates.begin(); it != fData.translates.end(); it++) {
            osg::Matrix mat;
            mat.setTrans(it->second[0], it->second[1], it->second[2]);
            transforms.translates[it->first]->setMatrix(mat);
        }
        for (std::map< std::string, Wrench > ::const_iterator iw = fData.wrenches.begin(); iw != fData.wrenches.end(); iw++) {
            //TODO
        }
    }
};

MarkerCallback::MarkerCallback(std::tr1::shared_ptr<LastReader> _reader,
                               SceneTransforms _transforms) :
        reader(_reader),
        transforms(_transforms) {};

int main(int argc, char **argv )
{
    /*po::options_description desc("Options");
    desc.add_options()
        ("help", "produce help message")
        ("compression", po::value<int>(), "set compression level")
        ;

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);    
    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }
    if (vm.count("compression")) {
        std::cout << "Compression level was set to " 
                  << vm["compression"].as<int>() << ".\n";
    } else {
        std::cout << "Compression level was not set.\n";
    }*/
    if (argc < 2)
    {
        exit(1);
    }
    H5LastReader _reader(argv[1], argv[2]);
    std::tr1::shared_ptr<LastReader> reader(&_reader); //TODO

    SceneFactory factory;
    SceneTransforms transforms = factory.initScene(reader->getLatestFrame());

    // set the scene to render
    osgViewer::Viewer viewer;
    viewer.setSceneData(transforms.ground);
    viewer.setThreadingModel(osgViewer::Viewer::SingleThreaded);

    // scene is manipulated with the mouse
    viewer.setCameraManipulator(new osgGA::TrackballManipulator());
    //viewer.run();
    viewer.realize();
    
    std::cerr << "before loop\n";
    while (!viewer.done())
    {
        FrameData fData = reader->getLatestFrame();
        for (std::map< std::string, Matrix > ::const_iterator im = fData.matrices.begin(); im != fData.matrices.end(); im++) {
            osg::Matrix mat(im->second[0], im->second[4], im->second[8], im->second[12],
                            im->second[1], im->second[5], im->second[9], im->second[13],
                            im->second[2], im->second[6], im->second[10], im->second[14],
                            im->second[3], im->second[7], im->second[11], im->second[15]);
            transforms.matrices[im->first]->setMatrix(mat);
        }
        for (std::map< std::string, Translate > ::const_iterator it = fData.translates.begin(); it != fData.translates.end(); it++) {
            osg::Matrix mat;
            mat.setTrans(it->second[0], it->second[1], it->second[2]);
            transforms.translates[it->first]->setMatrix(mat);
        }
        for (std::map< std::string, Wrench > ::const_iterator iw = fData.wrenches.begin(); iw != fData.wrenches.end(); iw++) {
            //TODO
        }
	viewer.frame();
    }
    std::cerr << "after loop\n";
}
