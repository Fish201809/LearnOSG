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


/**
 * 绘制索引
 */
static char * index_vert = {
	"#version 450 core\n"
	"layout(location = 0) in vec3 vPosition;\n"
	"uniform mat4 osg_ModelViewProjectionMatrix;"
	"void main(void ){\n"
		"gl_Position = osg_ModelViewProjectionMatrix * vec4(vPosition,1.0f);\n"
	"}\n"
};


static char * index_frag = {
	"#version 450 core\n"
	"uniform vec4 uIndex;\n"
	"out vec4 Color;\n"
	"void main() {\n"
	"	Color = uIndex;\n"
	"}\n"
};

/**
 * 基础绘制
 */
static char * basic_vert = {
	"#version 450 core\n"
	"layout(location = 0) in vec3 vPosition;\n"
	"uniform mat4 osg_ModelViewProjectionMatrix;"
	"void main(void ){\n"
		"gl_Position = osg_ModelViewProjectionMatrix * vec4(vPosition,1.0f);\n"
	"}\n"
};

static char * basic_frag = {
	"#version 450 core\n"
	"uniform vec3 uColor;\n"
	"out vec4 Color;\n"
	"void main() {\n"
	"	Color = vec4(uColor, 1.0f);\n"
	"}\n"
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
	osg::TessellationHints *hints = new osg::TessellationHints();
	float radius = 0.08;//(以米为单位)
	hints->setDetailRatio(0.5);//表示模型的粗细度。值越大精细度就越大。打个比方，当精细度小时，一个球可能只有几条线组成，很不圆。但是当精细度大时就十分像个球了。
	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.2, 0.0, 0.0), 2 * radius), hints));
	geode->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);	//关闭光照
	return geode;
}

/**
 * 创建一个基本球体
 */
osg::ref_ptr<osg::Node> createSphere() {
	osg::TessellationHints *hints = new osg::TessellationHints();
	float radius = 0.08;//(以米为单位)
	float height = 0.1;
	osg::ref_ptr<osg::Geode> cylinder = new osg::Geode;
	cylinder->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.6, 0.0, 0.0), radius, height), hints));
	return cylinder;
}

void init() {
	//创建shader
	basic_program = new osg::Program;
	basic_program->addShader(new osg::Shader(osg::Shader::FRAGMENT, basic_frag));
	basic_program->addShader(new osg::Shader(osg::Shader::VERTEX, basic_vert));

	//创建shader
	index_program = new osg::Program;
	index_program->addShader(new osg::Shader(osg::Shader::FRAGMENT, index_frag));
	index_program->addShader(new osg::Shader(osg::Shader::VERTEX, index_vert));

	cow_node = osgDB::readNodeFile("cow.osg");
	cube_node = createCube();
	sphere_node = createSphere();


	sphere_node->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);	//关闭光照
	cube_node->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);	//关闭光照

}

int x_ = 0;
int y_ = 0;

//RTT相机后处理
struct AfterCallBack :public osg::Camera::DrawCallback
{
	AfterCallBack() {
		//image_ = image;
	}
	~AfterCallBack() {}

	virtual void operator()(const osg::Camera &camera) const {
		static float data;
		GLuint tex_data[3];
		float mydata[4];
		////////image_->readPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE);
		////////osgDB::writeImageFile(*image_, "aa.jpg");
		//printf("x: %d, y: %d -- [ %d ]\n", x_, y_, data);
		
		
		/*glReadPixels(x_, y_, 1, 1, GL_RED, GL_FLOAT, (void*)&data);
		printf("x: %d, y: %d -- [ %f ]\n", x_, y_, data);*/

		glReadPixels(x_, y_, 1, 1, GL_RGBA, GL_FLOAT, (void*)mydata);
		printf("x: %d, y: %d -- [ %f , %f, %f, %f]\n", x_, y_, mydata[0], mydata[1], mydata[2], mydata[3]);

		/*glReadPixels(x_, y_, 1, 1, GL_RED, GL_UNSIGNED_INT, tex_data);
		printf("x: %d, y: %d -- [ %d, %d, %d ]\n", x_, y_, tex_data[0], tex_data[1], tex_data[2]);*/

		

		sphere_node->getOrCreateStateSet()->addUniform(new osg::Uniform("uColor", osg::Vec3(1.0f, 0.0f, 0.0f)));
		sphere_node->getOrCreateStateSet()->setAttributeAndModes(basic_program, osg::StateAttribute::ON);

		cube_node->getOrCreateStateSet()->addUniform(new osg::Uniform("uColor", osg::Vec3(0.0f, 1.0f, 0.0f)));
		cube_node->getOrCreateStateSet()->setAttributeAndModes(basic_program, osg::StateAttribute::ON);

	}
	void setX(int x) {
		x_ = x;
	}
	void setY(int y) {
		y_ = y;
	}

private:
	osg::ref_ptr<osg::Image> image_;

};


class InitCallBack : public osg::Camera::DrawCallback
{
public:
	virtual void operator()(const osg::Camera &camera) const {
		sphere_node->getOrCreateStateSet()->addUniform(new osg::Uniform("uIndex", osg::Vec4(0.3f, 0.3, 0.3f, 0.3f)));
		sphere_node->getOrCreateStateSet()->setAttributeAndModes(index_program, osg::StateAttribute::ON);

		cube_node->getOrCreateStateSet()->addUniform(new osg::Uniform("uIndex", osg::Vec4(0.6f, 0.6, 0.6f, 0.6f)));
		cube_node->getOrCreateStateSet()->setAttributeAndModes(index_program, osg::StateAttribute::ON);
	}
};


class PickHandle : public osgGA::TrackballManipulator
{
public:

	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& ga)  override {
		x_ = ea.getX();
		y_ = ea.getY();
		return __super::handle(ea, ga);
	}
};




int main(int argc, char *argv[]) {

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

	init();
	osg::ref_ptr<osg::Group> group = new osg::Group;
	group->addChild(cube_node);
	group->addChild(sphere_node);


	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	const std::string version("4.5");
	traits->x = 400;
	traits->y = 300;
	traits->width = 800;
	traits->height = 600;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;
	traits->glContextVersion = version;
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits);
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

	//创建一个RTT相机
	osg::ref_ptr<osg::Camera> rtt = new osg::Camera;
	rtt->setGraphicsContext(gc);
	rtt->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	osg::ref_ptr<osg::Texture2D> texture2d = new osg::Texture2D;


	texture2d->setSourceFormat(GL_RGBA);
	texture2d->setInternalFormat(GL_RGBA);
	texture2d->setSourceType(GL_FLOAT);
	rtt->setRenderTargetImplementation(osg::Camera::FRAME_BUFFER);
	rtt->attach(osg::Camera::COLOR_BUFFER, texture2d.get());
	rtt->addInitialDrawCallback(new InitCallBack);
	rtt->setRenderOrder(osg::Camera::PRE_RENDER);
	rtt->addPostDrawCallback(new AfterCallBack);


	//加入一个RTT相机到Viewer
	viewer->addSlave(rtt);

	//创建一个观察相机
	osg::ref_ptr<osg::Camera> master = new osg::Camera;

	master->setGraphicsContext(gc);
	master->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	master->addChild(group);

	viewer->setCameraManipulator(new PickHandle);


	viewer->addSlave(master, false);
	viewer->setSceneData(group);
	viewer->run();

	return 0;
}