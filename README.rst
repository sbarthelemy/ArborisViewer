=============
ArborisViewer
=============

ArborisViewer is 3d viewer based on OpenSceneGraph (OSG_). It reads
trajectories from HDF5_ files and animates the scene accordingly.


Development status
==================

The project is currently ABANDONED in favor of dænim_. If you came here
looking for the viewer to use with arboris_, it is dænim_.

Install & Test
==============

On Ubuntu Karmic
----------------

Prerequisites::

    sudo aptitude install cmake
    sudo aptitude install libopenscenegraph-dev \
    libboost-program-options-dev zlib1g-dev

Build::

    cd ArborisViewer
    mkdir build
    cd build
    cmake ..
    make

Test::
    bin/test_avreader
    bin/arborisviewer ../data/simplearm.h5 /


Content
=======

:cmake:
    Helper files for cmake

:data:
    Example data, useful for tests

:hdf5:
    The vanilla HDF5_ library, downloaded from their website

:src:
    The source code of the libray, the main executable and the tests

:szip:
    The vanilla szip library


Inner workings
==============

This software does 3 things:

1. get the trajectories data
2. setup a 3d scene
3. animate the scene

Getting trajectories data
-------------------------

We define two pure virtual reader classes: RandomReader and LastReader.
The former can acces the position at any point in time, the later can
only access the latest position.

The two classes H5RandomReader and H5LastReader get their data from 
HDF5 files and implement these interfaces.

Setting up a 3d scene
---------------------

It's the SceneFactory class responsability to set up the scene. Its
init_scene method returns a SceneTransforms object which has mappings
between scene object names and the corresponding OSG transforms

Animating the scene
-------------------

The main executable (arborisviewer) intializes a scene, a reader then
updates the scene with the data it gets from the reader.


We use...
=========

- CMake_ to generate the build toolchain (compatible with Make, 
  Visual C++, Eclipse CDT, Code::Blocks, etc.)

- Doxygen_ to generate the documentation,

- Boost.Program_options_ library to parse options from configuration 
  files and/or command-line

- HDF5_ library to store numerical data oin a portable way. The doc
  for the C++ version is sparse, but is there: 
  http://www.hdfgroup.org/HDF5/doc/cpplus_RM/

.. _ISIR: www.isir.fr
.. _HDF5: http://www.hdfgroup.org/HDF5/
.. _CSV: http://tools.ietf.org/html/rfc4180
.. _OSG: www.openscenegraph.org
.. _Codamotion: http://www.codamotion.com
.. _CMake: http://www.cmake.org
.. _Doxygen: http://www.stack.nl/~dimitri/doxygen/index.html
.. _Boost.Program_options: 
    http://www.boost.org/doc/libs/1_40_0/doc/html/program_options.html
.. _dænim: http://github.com/sbarthelemy/daenim
.. _arboris: http://github.com/sbarthelemy/arboris-python
