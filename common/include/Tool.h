#pragma once

#include <osgViewer/Viewer>
#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgUtil/Optimizer>
#include <osg/MatrixTransform>
#include <osg/ShapeDrawable>
#include <osgFX/Scribe>
#include <OpenThreads/Thread>
#include <osgViewer/ViewerEventHandlers>
#include <osg/PolygonMode>


class Tool
{
public:
	static osg::ref_ptr<osgViewer::Viewer> GetViewer();
	static osg::ref_ptr<osg::Node> CreateGeodePlane();
	static osg::ref_ptr<osg::Geometry> CreateGeometryPlane();
	static osg::ref_ptr<osg::Geometry> CreateGeometryPlane2();
	static osg::ref_ptr<osg::Geode> CreateGeodeShape();
	static osg::ref_ptr<osg::Group> CreatGroup();
};
