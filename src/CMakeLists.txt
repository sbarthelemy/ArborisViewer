#FIND_PACKAGE(HDF5)
SET(HDF5_INCLUDE_DIRS
    ${ARBORISVIEWER_SOURCE_DIR}/hdf5/include)
SET(HDF5_LIBRARIES
    ${ARBORISVIEWER_SOURCE_DIR}/hdf5/lib/libhdf5_cpp.a
    ${ARBORISVIEWER_SOURCE_DIR}/hdf5/lib/libhdf5.a
    ${ARBORISVIEWER_SOURCE_DIR}/szip/lib/libsz.a)
FIND_PACKAGE(ZLIB)

include_directories(${HDF5_INCLUDE_DIRS})

add_library(
    avreader
    avreader.cpp
)

add_library(
    avreaderh5
    avreaderh5.cpp
)
target_link_libraries(
    avreaderh5
    avreader
    ${HDF5_LIBRARIES}
    ${ZLIB_LIBRARIES}
)

add_executable(
    test_avreader
    test_avreader.cpp
    avreaderh5_test.cpp
)
target_link_libraries(
    test_avreader
    avreaderh5
    ${CPPUNIT_LIBRARY}
)

FIND_PACKAGE(Boost COMPONENTS program_options)
INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
FIND_PACKAGE(OpenSceneGraph COMPONENTS osgViewer osgUtil osgGA osgSim osgDB osgAnimation)
INCLUDE_DIRECTORIES(${OPENSCENEGRAPH_INCLUDE_DIRS})

add_library(
    scenefactory
    scenefactory.cpp
)

add_executable(
    arborisviewer
    arborisviewer.cpp
)
target_link_libraries(
    arborisviewer
    scenefactory
    avreader
    avreaderh5
    ${OPENSCENEGRAPH_LIBRARIES}
    ${Boost_LIBRARIES}
)
