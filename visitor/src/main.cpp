#include "Tool.h"


class SelectHandler : public osgGA::GUIEventHandler
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

				//std::cout << node->getName() << std::endl;

				if (aa.computeIntersections(ea, np, intersections)) {
					int l = 0;
					for (osgUtil::LineSegmentIntersector::Intersections::iterator it = intersections.begin(); it != intersections.end(); ++it, ++l) {
						const osg::NodePath& npp = it->nodePath;
						std::cout << std::to_string(l) << " " << npp[npp.size() - 1]->getName() << std::endl;
					}
						
					if (node->getName() == "Quade")
					{
						//osg::ref_ptr<osg::Geometry> geo = Tool::CreateGeometryPlane();
						//node->asGeode()->replaceChild(node->asGeode()->getChild(0), geo);
						osg::Vec4 color = osg::Vec4(0.0f, 0.0f, 1.0f, 1.0f);
						osg::ref_ptr<osg::Uniform> SineUniform = new osg::Uniform("mycolor", color);
						node->getOrCreateStateSet()->addUniform(SineUniform);

					}
					else {
						node->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::ON);
					}
				}
				else {
					if (node->getName() == "Quade") {
						osg::ref_ptr<osg::Geometry> geo = Tool::CreateGeometryPlane2();
						node->asGeode()->replaceChild(node->asGeode()->getChild(0), geo);
						osg::Vec4 color = osg::Vec4(1.0f, 0.0f, 0.0f, 1.0f);
						osg::ref_ptr<osg::Uniform> SineUniform = new osg::Uniform("mycolor", color);
						node->getOrCreateStateSet()->addUniform(SineUniform);
					}
					else {
						node->getOrCreateStateSet()->setMode(GL_FOG, osg::StateAttribute::OFF);
					}
				}
			}
		}
				return true;
	}
};

static char * vertexShader = {
	"varying vec4 color;\n"
	"void main(void ){\n"
		"color = gl_Vertex;\n"
		"gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;\n"
	"}\n"
};
static char * fragShader = {
	"varying vec4 color;\n"
	"uniform vec4 mycolor;\n"
	"void main(void){\n"
	//"	gl_FragColor = clamp(color,0.0,1.0);\n"
	"	gl_FragColor = mycolor;\n"
	"}\n"
};


int main(int argc, char *argv[]) {
	
	osg::ref_ptr<osgViewer::Viewer> viewer = Tool::GetViewer();
	
	osg::ref_ptr<osg::Group> root = Tool::CreatGroup();
	osg::ref_ptr<osg::Node> cube = Tool::CreateGeodePlane();

	osg::ref_ptr<osg::StateSet> stateset = cube->getOrCreateStateSet();
	osg::ref_ptr<osg::PolygonMode> polygonMode = new osg::PolygonMode();
	polygonMode->setMode(osg::PolygonMode::FRONT_AND_BACK, osg::PolygonMode::LINE);
	stateset->setAttribute(polygonMode.get(), osg::StateAttribute::OVERRIDE | osg::StateAttribute::ON);


	osg::StateSet * ss = cube->getOrCreateStateSet();
	osg::Program * program = new osg::Program;
	program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragShader));
	program->addShader(new osg::Shader(osg::Shader::VERTEX, vertexShader));
	ss->setAttributeAndModes(program, osg::StateAttribute::ON);

	cube->setName("Quade");
	root->addChild(cube);

	viewer->addEventHandler(new SelectHandler);

	osgUtil::Optimizer optimizer;
	optimizer.optimize(root);
	viewer->setSceneData(root);
	viewer->realize();
	viewer->run();
	return 0;
}
