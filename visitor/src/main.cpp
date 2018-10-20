#include "Tool.h"


class ControlMan : public osgGA::GUIEventHandler
{
	
public:
	virtual bool handle(const osgGA::GUIEventAdapter& ea, osgGA::GUIActionAdapter& aa) override {
		osgViewer::Viewer* viewer = dynamic_cast<osgViewer::Viewer*>(&aa);

		osgUtil::IntersectionVisitor vieitor;

		osgUtil::LineSegmentIntersector::Intersections intersections;

		

		int children = viewer->getSceneData()->asGroup()->getNumChildren();
		for (int i = 0; i < children; ++i) {
			osg::NodePathList nps = viewer->getSceneData()->asGroup()->getChild(i)->getParentalNodePaths();
			for (int j = 0; j < nps.size(); ++j) {
				osg::NodePath np = nps[j];
				osg::ref_ptr<osg::Node> node = np[np.size() - 1];

				std::cout << node->getName() << std::endl;

				if (aa.computeIntersections(ea, np, intersections)) {
					node->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);
				}
				else {
					node->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF);
				}

			}
			
		}



		/*for (osg::NodePath::iterator itr = np.begin(); itr != np.end(); itr++) {
			(*itr)->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF);
			if (!(*itr)->getName().empty()) {

				std::cout << (*itr)->getName() << std::endl;
				(*itr)->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);

			}

		}*/


		/*if (ea.getEventType() == osgGA::GUIEventAdapter::MOVE) {*/
				//std::cout << ea.getX() << " " << ea.getY() << std::endl;

			//static int j = 1;
			////viewer->getSceneData()->asGroup()->getChild(0)->asGeode()->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);
			//	int children = viewer->getSceneData()->asGroup()->getNumChildren();
			//	for (int i = 0; i < children; ++i) {
			//		if (i % 2 == j) {
			//			viewer->getSceneData()->asGroup()->getChild(i)->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF);
			//		}
			//		else {
			//			viewer->getSceneData()->asGroup()->getChild(i)->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);
			//		}
			//		
			//	}
			//	static int k = 0;
			//	k++;
			//	if (k == 100) {
			//		if (j == 1) {
			//			j = 0;
			//		}
			//		else {
			//			j = 1;
			//		}
			//		k = 0;
			//	}
				/*	OpenThreads::Thread::microSleep(5000);
					for (int i = 0; i < children; ++i) {
						viewer->getSceneData()->asGroup()->getChild(i)->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);
					}*/
				
				//osgUtil::LineSegmentIntersector::Intersections intersections;
				//osg::ref_ptr<osg::Node> node;
				//osg::ref_ptr<osg::Group> parent;

				//if (aa.computeIntersections(ea, intersections)) {
				//	//osg::Vec3d point = inter.begin()->getWorldIntersectPoint();
				//	////std::cout << point.x() << " " << point.y() << " " << point.z() << std::endl;
				//	////std::cout << inter.begin()->primitiveIndex << std::endl;
				//	////std::cout << inter.begin()->nodePath.;

				//	//osg::NodePath np = inter.begin()->nodePath;

				//	/*for (osg::NodePath::iterator itr = np.begin(); itr != np.end(); itr++) {
				//		(*itr)->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF);
				//		if (!(*itr)->getName().empty()) {

				//			std::cout << (*itr)->getName() << std::endl;
				//			(*itr)->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);

				//		}

				//	}*/
				//	osgUtil::LineSegmentIntersector::Intersection intersection = *intersections.begin();
				//	osg::NodePath& nodePath = intersection.nodePath;
				//	//得到选择的物体
				//	node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
				//	parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;
				//}
				////用一种高亮显示来显示物体已经被选中
				//if (parent.get() && node.get()) {
				//	osg::ref_ptr<osgFX::Scribe> parentAsScribe = dynamic_cast<osgFX::Scribe*>(parent.get());
				//	if (!parentAsScribe) {
				//		//如果对象选择到，高亮显示
				//	/*	osg::ref_ptr<osgFX::Scribe> scribe = new osgFX::Scribe();
				//		scribe->addChild(node.get());*/
				//		node->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);
				//		//parent->replaceChild(node.get(), scribe.get());
				//	}
				//	else {
				//		//如果没有没有选择到，则移除高亮显示的对象
				//		osg::Node::ParentList parentList = parentAsScribe->getParents();
				//		for (osg::Node::ParentList::iterator itr = parentList.begin();
				//			 itr != parentList.end();
				//			 ++itr) {
				//			(*itr)->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF);
				//		}
				//	}
				//}

				//osgUtil::LineSegmentIntersector::Intersection intersection = inter.begin();
				//osg::NodePath& nodePath = intersection.nodePath;
				//得到选择的物体
			/*	node = (nodePath.size() >= 1) ? nodePath[nodePath.size() - 1] : 0;
				parent = (nodePath.size() >= 2) ? dynamic_cast<osg::Group*>(nodePath[nodePath.size() - 2]) : 0;*/



				return true;
	}

		


};

int main(int argc, char *argv[]) {
	
	osg::ref_ptr<osgViewer::Viewer> viewer = Tool::GetViewer();
	
	osg::ref_ptr<osg::Group> root = Tool::CreatGroup();

	viewer->addEventHandler(new ControlMan);

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root);
	viewer->setSceneData(root);
	viewer->realize();
	viewer->run();
	return 0;
}
