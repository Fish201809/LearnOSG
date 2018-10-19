#include "Tool.h"


int main(int argc, char *argv[]) {
	
	osg::ref_ptr<osgViewer::Viewer> viewer = Tool::GetViewer();
	osg::ref_ptr<osg::Group> root = new osg::Group();
	
	
	osg::MatrixTransform* translateMT = new osg::MatrixTransform;
	translateMT->setMatrix(osg::Matrix::translate(10, 0, 0));
	translateMT->addChild(Tool::CreateQuade());

	osg::MatrixTransform* translateMT2 = new osg::MatrixTransform;
	translateMT2->setMatrix(osg::Matrix::translate(5, 0, 0));
	translateMT2->addChild(Tool::CreateShape());


	root->addChild(translateMT);
	root->addChild(osgDB::readNodeFile("cow.osg"));
	root->addChild(translateMT2);

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root);
	viewer->setSceneData(root);
	viewer->realize();
	viewer->run();
	return 0;
}
