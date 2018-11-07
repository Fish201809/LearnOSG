#include "Tool.h"



int main(int argc, char *argv[]) {
	
	osg::ref_ptr<osgViewer::Viewer> viewer = Tool::GetViewer();
	
	osg::ref_ptr<osg::Group> root = Tool::CreatGroup();

	
	osgUtil::Optimizer optimizer;
	optimizer.optimize(root);
	viewer->setSceneData(root);
	viewer->realize();
	viewer->run();
	return 0;
}
