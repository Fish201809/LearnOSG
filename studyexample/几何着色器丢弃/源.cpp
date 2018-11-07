#include <windows.h>
#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/FrameBufferObject>
#include <osg/Texture2D>
#include <osgGA/OrbitManipulator>
#include <osgGA/CameraManipulator>
#include <osgGA/TrackballManipulator>
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
#include <osg/Program>
#include <osg/Shader>
#include <random>

static float vertex[] = {
			-5.0f, -5.0f, -5.0f,
			5.0f, -5.0f, -5.0f,
			5.0f, 5.0f, -5.0f,
			5.0f, 5.0f, -5.0f,
			-5.0f, 5.0f, -5.0f,
			-5.0f, -5.0f, -5.0f,

			-5.0f, -5.0f, 5.0f,
			5.0f, -5.0f, 5.0f,
			5.0f, 5.0f, 5.0f,
			5.0f, 5.0f, 5.0f,
			-5.0f, 5.0f, 5.0f,
			-5.0f, -5.0f, 5.0f,

			-5.0f, 5.0f, 5.0f,
			-5.0f, 5.0f, -5.0f,
			-5.0f, -5.0f, -5.0f,
			-5.0f, -5.0f, -5.0f,
			-5.0f, -5.0f, 5.0f,
			-5.0f, 5.0f, 5.0f,

			5.0f, 5.0f, 5.0f,
			5.0f, 5.0f, -5.0f,
			5.0f, -5.0f, -5.0f,
			5.0f, -5.0f, -5.0f,
			5.0f, -5.0f, 5.0f,
			5.0f, 5.0f, 5.0f,

			-5.0f, -5.0f, -5.0f,
			5.0f, -5.0f, -5.0f,
			5.0f, -5.0f, 5.0f,
			5.0f, -5.0f, 5.0f,
			-5.0f, -5.0f, 5.0f,
			-5.0f, -5.0f, -5.0f,

			-5.0f, 5.0f, -5.0f,
			5.0f, 5.0f, -5.0f,
			5.0f, 5.0f, 5.0f,
			5.0f, 5.0f, 5.0f,
			-5.0f, 5.0f, 5.0f,
			-5.0f, 5.0f, -5.0f
};


osg::ref_ptr<osg::Program> basic_program = nullptr;
osg::ref_ptr<osg::Program> index_program = nullptr;

osg::ref_ptr<osg::Node> cow_node = nullptr;
osg::ref_ptr<osg::Node> cube_node = nullptr;
osg::ref_ptr<osg::Node> sphere_node = nullptr;

/**
 * 创建一个基本立方体
 */
osg::ref_ptr<osg::Node> createCube() {
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> geo = new osg::Geometry();

	osg::ref_ptr<osg::Vec3Array> vertex_array = new osg::Vec3Array;
	osg::ref_ptr<osg::FloatArray> int_array = new osg::FloatArray;
	osg::ref_ptr<osg::Vec3Array> color_array = new osg::Vec3Array;
	//设置随机数
	std::random_device rd;
	std::default_random_engine engine(rd());
	std::uniform_real_distribution<GLfloat> dis(0.0f, 1.0f);
	auto dice = std::bind(dis, engine);

	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 6; ++j) {
			vertex_array->push_back(osg::Vec3(vertex[i * 18 + j * 3], vertex[i * 18 + j * 3 + 1], vertex[i * 18 + j * 3 + 2])); 
			color_array->push_back(osg::Vec3(dice(), dice(), dice()));
			if (i == 0) {
				int_array->push_back(0);
			}
			else {
				int_array->push_back(1);
			}
		}
	}
	geo->setVertexArray(vertex_array);
	geo->setVertexAttribArray(1, int_array);
	
	geo->setVertexAttribBinding(1, osg::Geometry::BIND_PER_VERTEX);

	geo->setVertexAttribArray(2, color_array);
	geo->setVertexAttribBinding(2, osg::Geometry::BIND_PER_VERTEX);

	geo->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES, 0, 36));

	

	geode->addDrawable(geo.get());

	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);	//关闭光照

	/*osg::StateSet* stateset = geode->getOrCreateStateSet();
	osg::ref_ptr<osg::PolygonMode> polygonMode = new osg::PolygonMode();
	polygonMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	stateset->setAttribute(polygonMode.get(), osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);*/

	return geode;
}

void init() {
	//创建shader
	basic_program = new osg::Program;
	
	basic_program->addShader(osgDB::readShaderFile(osg::Shader::VERTEX, "basic.vert"));
	basic_program->addShader(osgDB::readShaderFile(osg::Shader::FRAGMENT, "basic.frag"));
	basic_program->addShader(osgDB::readShaderFile(osg::Shader::GEOMETRY, "basic.geom"));

	cube_node = createCube();

	cube_node->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);	//关闭光照
	cube_node->getOrCreateStateSet()->setAttributeAndModes(basic_program, osg::StateAttribute::ON);
}




int main(int argc, char *argv[]) {

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

	init();
	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(cube_node);
	group->addChild(sphere_node);


	const std::string version("3.3");
	osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
	traits->x = 50; traits->y = 30;
	traits->width = 800; traits->height = 600;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->glContextVersion = version;

	osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext(traits.get());
	gc->getState()->setUseModelViewAndProjectionUniforms(true);
	gc->getState()->setUseVertexAttributeAliasing(true);

	if (gc.valid()) {
		osg::notify(osg::INFO) << "  GraphicsWindow has been created successfully." << std::endl;
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
	}


	viewer->getCamera()->setGraphicsContext(gc);
	viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));

	viewer->setSceneData(group);
	viewer->run();

	return 0;
}