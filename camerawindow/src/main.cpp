#include "Tool.h"

void singleWindowMultipleCameras(osg::ref_ptr<osgViewer::Viewer> viewer) {
	//创建窗口系统接口（调用底层API得到绘图设备、窗口特性）
	osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (!wsi) {
		osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return;
	}

	//得到当前窗口分辨率
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	//设置图形环境特性（设置要创建的图形设备上下文）
	osg::ref_ptr<osg::GraphicsContext::Traits> traits = new osg::GraphicsContext::Traits();
	//设置图形设备的尺寸
	traits->x = 0;
	traits->y = 0;
	traits->width = width;
	traits->height = height;
	//显示标题栏
	traits->windowDecoration = true;
	traits->doubleBuffer = true;
	traits->sharedContext = 0;

	//创建图形环境（创建图形设备上下文）
	osg::ref_ptr<osg::GraphicsContext> gc = osg::GraphicsContext::createGraphicsContext(traits.get());

	if (gc->valid()) {
		osg::notify(osg::INFO) << "GraphicsWindow has been created successfully." << std::endl;

		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 1.6f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		osg::notify(osg::NOTICE) << "GraphicsWindow has not been created successfully." << std::endl;
	}

	//设置第一个视口（创建cameraMaster主相机）
	osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();
	//设置图形环境
	cameraMaster->setGraphicsContext(gc.get());

	//根据分辨率确定合适的投影来保证显示的图形不变形
	//double fovy, aspectRatio, zNear, zFar;
	//cameraMaster->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	//double newAspectRatio = double(traits->width) / double(traits->height);
	//double aspectRatioChange = newAspectRatio / aspectRatio;
	//if (aspectRatioChange != 1.0) {
	//	cameraMaster->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	//}

	//设置视口位置
	cameraMaster->setViewport(new osg::Viewport(0, 0, width, height));
	//根据是否使用双缓存，确定绘制哪个缓存，如果使用双缓存，先绘制前缓存
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//设置缓冲区（设置每帧开始绘制时，绘制哪个缓存）
	cameraMaster->setDrawBuffer(bufferMaster);
	cameraMaster->setReadBuffer(bufferMaster);

	////设置第二个视口（创建从属相机）
	//osg::ref_ptr<osg::Camera> cameraClient = new osg::Camera();
	////设置图形环境
	//cameraClient->setGraphicsContext(gc.get());
	////设置视口位置，视口的坐标为左下角为坐标原点
	//cameraClient->setViewport(new osg::Viewport(0, 0, 400, 400));
	////根据是否使用双缓存，确定绘制哪个缓存，如果使用双缓存，先绘制前缓存
	//GLenum bufferClient = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	////设置缓冲区（设置每帧开始绘制时，绘制哪个缓存）
	//cameraClient->setDrawBuffer(bufferClient);
	//cameraClient->setReadBuffer(bufferClient);

	////添加从属相机
	//viewer->addSlave(cameraClient, osg::Matrix::scale(aspectRatio, 1.0, 1.0), osg::Matrix());
}

int main(int argc, char *argv[]) {
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer;

	//创建窗口系统接口（调用底层API得到绘图设备、窗口特性）
	osg::ref_ptr<osg::GraphicsContext::WindowingSystemInterface> wsi = osg::GraphicsContext::getWindowingSystemInterface();

	if (!wsi) {
		osg::notify(osg::NOTICE) << "Error, no WindowSystemInterface available, cannot create windows." << std::endl;
		return 0;
	}

	//得到当前窗口分辨率
	unsigned int width, height;
	wsi->getScreenResolution(osg::GraphicsContext::ScreenIdentifier(0), width, height);

	width = 800;
	height = 600;

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
		gc->setClearColor(osg::Vec4f(0.0f, 0.8f, 0.4f, 1.0f));
		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}
	else {
		osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
	}

	//double fovy, aspectRatio, zNear, zFar;
	//viewer->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	/*double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0) {
		viewer->getCamera()->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}*/

	//设置第一个视口（创建cameraMaster主相机）
	osg::ref_ptr<osg::Camera> cameraMaster = viewer->getCamera();
	//设置图形环境
	cameraMaster->setGraphicsContext(gc.get());

	//设置视口位置
	cameraMaster->setViewport(new osg::Viewport(0, 0, width, height));
	//根据是否使用双缓存，确定绘制哪个缓存，如果使用双缓存，先绘制前缓存
	GLenum bufferMaster = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	//设置缓冲区（设置每帧开始绘制时，绘制哪个缓存）
	cameraMaster->setDrawBuffer(bufferMaster);
	cameraMaster->setReadBuffer(bufferMaster);

	////设置第二个视口（创建从属相机）
	//osg::ref_ptr<osg::Camera> cameraClient = new osg::Camera();
	////设置图形环境
	//cameraClient->setGraphicsContext(gc.get());
	////设置视口位置，视口的坐标为左下角为坐标原点
	//cameraClient->setViewport(new osg::Viewport(0, 0, 400, 400));
	////根据是否使用双缓存，确定绘制哪个缓存，如果使用双缓存，先绘制前缓存
	//GLenum bufferClient = traits->doubleBuffer ? GL_BACK : GL_FRONT;
	////设置缓冲区（设置每帧开始绘制时，绘制哪个缓存）
	//cameraClient->setDrawBuffer(bufferClient);
	//cameraClient->setReadBuffer(bufferClient);

	////添加从属相机
	//viewer->addSlave(cameraClient, osg::Matrix::scale(aspectRatio, 1.0, 1.0), osg::Matrix());

	osg::ref_ptr<osg::Node> cow = osgDB::readNodeFile("cow.osg");
	viewer->setSceneData(cow);
	viewer->run();
	return 0;
}