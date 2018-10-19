#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgUtil/Optimizer>

int main() {
	//创建Viewer对象，场景浏览器
	osgViewer::Viewer* viewer = new osgViewer::Viewer();
	//创建场景组节点
	osg::Group* root = new osg::Group();
	//创建一个节点
	osg::Node* node = osgDB::readNodeFile("cow.osg");
	//添加到场景
	root->addChild(node);


	//设置图形环境特性
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	traits->x = 400;
	traits->y = 300;
	traits->width = 800;
	traits->height = 600;
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//创建图形环境特性
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits);
	if (gc.valid()) {
		osg::notify(osg::INFO) << "  GraphicsWindow has been created successfully." << std::endl;

		//清除窗口颜色及清除颜色和深度缓冲
		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
	}

	//根据分辨率来确定合适的投影来保证显示的图形不变形
	double fovy, aspectRatio, zNear, zFar;
	viewer->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0) {
		//设置投影矩阵
		viewer->getCamera()->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}

	viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	//设置图形环境
	viewer->getCamera()->setGraphicsContext(gc.get());


	//优化场景数据
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root);
	//设置场景数据
	viewer->setSceneData(root);
	//初始化并创建窗口
	viewer->realize();
	//开始渲染
	viewer->run();
	return 0;
}

/*
	x,y,width,height;//窗口的坐标、高度及宽度，默认值都为0
	windowDecration(false),  //是否支持窗口扩展的功能，Win32中style
	supportsResize(true),    //是否支持窗口缩放
	red(8),                  //红色位数，默认8位
	blue(8),                 //蓝色位数，默认8位
	green(8),                //绿色位数，默认8位
	alpha(0),                //alpha值位数，透明度，默认没有alpha通道，为RGB格式。
	depth(24),               //颜色的深度(16,24,32),默认使用24位
	stencil(0),              //模板默认无
	sampleBuffers(0),        //采样缓存，默认无
	samples(0),              //采样倍数（抗锯齿的倍数），默认无
	pbufferStereo(false),    //pbuffer,默认不支持
	quadBufferStereo(false), //立体四缓存，主要在高端显卡上有，如QUDRO显卡上。
	doubleBuffer(false),     //是否支持双缓存，默认不支持
	target(0),               //目标
	format(0),               //格式
	level(0),                //嵌套的层数，默认无
	face(0),                 //面
	mipMapGeneration(false)  //是否支持生成Mipmap,默认不支持
	vsync(true),             //是否支持同步，默认同步模式
	useMultiThreadedOpenGLEngine(false),    //是否采用多线程，默认不支持
	useCursor(true),         //是否使用鼠标的指针，默认使用
	sharedContext(0),        //共享上下文
	setInheritedWindowPixelFormat(false)   //是否继承Window中的位格式
*/