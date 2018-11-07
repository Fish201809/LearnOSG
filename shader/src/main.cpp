#include "Tool.h"

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
int main() {
	osg::ref_ptr<osgViewer::Viewer> viewer = Tool::GetViewer();
	osg::ref_ptr<osg::Node> node = Tool::CreateGeodePlane();

	osg::StateSet * ss = node->getOrCreateStateSet();
	osg::Program * program = new osg::Program;
	program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragShader));
	program->addShader(new osg::Shader(osg::Shader::VERTEX, vertexShader));

	osg::Vec4 color = osg::Vec4(0.0f, 1.0f, 1.0f, 1.0f);
	osg::Uniform* SineUniform = new osg::Uniform("mycolor", color);
	ss->addUniform(SineUniform);

	ss->setAttributeAndModes(program, osg::StateAttribute::ON);

	viewer->addEventHandler(new osgViewer::WindowSizeHandler);
	viewer->setSceneData(node.get());
	return viewer->run();
}