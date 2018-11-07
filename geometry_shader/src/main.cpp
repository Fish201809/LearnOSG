#include <osgViewer/Viewer>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osg/FrameBufferObject>
#include <osg/Texture2D>
#include <osgGA/OrbitManipulator>
#include <osgGA/CameraManipulator>
#include <osgGA/TrackballManipulator>

/**
 * 绘制纹理
 */
static char * vertexShader = {
	"#version 450 core\n"
	"layout(location = 0) in vec3 vPosition;\n"
	"layout(location = 1) in vec2 vCoord;\n"
	"out vec2 fCoord;"
	"void main(void ){\n"
		"fCoord = vCoord;\n"
		"gl_Position = vec4(vPosition,1.0f);\n"
	"}\n"
};
static char * fragShader = {
	"#version 450 core\n"
	"in vec2 fCoord;\n"
	"out vec4 Color;\n"
	"uniform sampler2D image;\n"
	"void main(void){\n"
	"	Color = texture(image, fCoord);\n"
	"}\n"
};

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
		GLubyte data[3];
		//image_->readPixels(0, 0, 800, 600, GL_RGB, GL_UNSIGNED_BYTE);
		//osgDB::writeImageFile(*image_, "aa.jpg");
		//printf("%d, %d, %d\n", data[0], data[1], data[2]);
		glReadPixels(x_, y_, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &data);
		printf("x: %d, y: %d -- [ %d, %d, %d ]\n", x_, y_, data[0], data[1], data[2]);
		//std::cout << x_ << " " << y_ << " " <<  data[0] << " " << data[1] << " " << data[2] << std::endl;
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

class PickHandle : public osgGA::TrackballManipulator
{
public:
	
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& ga)  override {
		x_ = ea.getX();
		y_ = ea.getY();
		return __super::handle(ea, ga);
	}
};

osg::ref_ptr<osg::Node> CreatNode(osg::ref_ptr<osg::Texture2D> texture) {
	//创建几何体
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
	osg::ref_ptr<osg::Vec3Array> array = new osg::Vec3Array;
	array->push_back(osg::Vec3(-1.0f, -1.0f, 0.0f));
	array->push_back(osg::Vec3(1.0f, -1.0f, 0.0f));
	array->push_back(osg::Vec3(1.0f, 1.0f, 0.0f));
	array->push_back(osg::Vec3(-1.0f, 1.0f, 0.0f));
	gm->setVertexAttribArray(0, array);
	gm->setVertexAttribBinding(0, osg::Geometry::BIND_PER_VERTEX);
	//gm->setVertexArray(array);
	//纹理数组
	osg::ref_ptr<osg::Vec2Array> tArr0 = new osg::Vec2Array;//创建一个 Vec2Array对象以保存纹理单元 0 的纹理坐标
	tArr0->push_back(osg::Vec2(0, 0));
	tArr0->push_back(osg::Vec2(1, 0));
	tArr0->push_back(osg::Vec2(1, 1));
	tArr0->push_back(osg::Vec2(0, 1));
	//gm->setTexCoordArray(0, tArr0.get());
	gm->setVertexAttribArray(1, tArr0);
	gm->setVertexAttribBinding(1, osg::Geometry::BIND_PER_VERTEX);

	//创建master着色器程序
	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
	gm->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);	//关闭光照
	gm->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);

	//创建shader
	osg::Program * program = new osg::Program;
	program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragShader));
	program->addShader(new osg::Shader(osg::Shader::VERTEX, vertexShader));

	osg::Vec4 color = osg::Vec4(0.0f, 1.0f, 1.0f, 1.0f);
	osg::Uniform* SineUniform = new osg::Uniform("mycolor", color);
	gm->getOrCreateStateSet()->addUniform(SineUniform);
	gm->getOrCreateStateSet()->setAttributeAndModes(program, osg::StateAttribute::ON);
	

	geode->addDrawable(gm.get());
	return geode;
}


int main(int argc, char *argv[]) {

	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;
	osg::ref_ptr<osg::Node> cow_node = osgDB::readNodeFile("cow.osg");

	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	traits->x = 400;
	traits->y = 300;
	traits->width = 800;
	traits->height = 600;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits);
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
	rtt->addChild(cow_node);
	rtt->setRenderOrder(osg::Camera::PRE_RENDER);
	rtt->addPostDrawCallback(new AfterCallBack);

	//加入一个RTT相机到Viewer
	viewer->addSlave(rtt);

	//创建一个观察相机
	osg::ref_ptr<osg::Camera> master = new osg::Camera;
	//master->setProjectionMatrixAsFrustum(-10, 10, -10, 10, 0.1f, 1000.0f);
	////master->setReferenceFrame(osg::Transform::ABSOLUTE_RF);
	//master->setViewMatrixAsLookAt(osg::Vec3(0.0f, 10.0f, 0.0f), osg::Vec3(0.0f, 0.0f, 0.0f),osg::Vec3(0.0f, -1.0f, 0.0f));

	master->setGraphicsContext(gc);
	master->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	osg::ref_ptr<osg::Node> texture_node = CreatNode(texture2d);
	master->addChild(texture_node);
	//master->addPostDrawCallback(new CaptureCallback);

	viewer->setCameraManipulator(new PickHandle);
//	viewer->addEventHandler(new PickHandle);
	//osg::Matrixf matrix;
	//matrix.makeFrustum(-10.0f, 1.0f, -1.0f, 1.0f, -1.0f, 10000.0f);
	//master->setViewMatrix(matrix);
	

	viewer->addSlave(master, false);
	viewer->setSceneData(cow_node);
	viewer->run();
	
	return 0;
}