//#include "Tool.h"
//
//static char vertex_shader[] = {
//	"#version 430 \n"
//	"layout (location=0) in vec3 vPosition;\n"
//	"layout (location=1) in vec2 TexCorrd;\n"
//	"out vec2 Corrd;\n"
//	"void main()\n"
//	"{\n"
//	"   Corrd = TexCorrd;"
//	"   gl_Position = vec4(vPosition,1.0);\n"
//	"}\n"
//};
//
//static char fragment_shader[] = {
//	"#version 430 \n"
//	"layout (location=0) out vec4 FragColor;\n"
//	"in vec2 Corrd;\n"
//	"uniform sampler2D sampler2d_0;\n"
//	"void main() {\n"
//	"   vec4 tmpColor = texture(sampler2d_0, Corrd);"
//	"   FragColor = tmpColor ;\n"
//	"   FragColor = vec4(1.0f,1.0f,0.0f,1.0f) ;\n"
//	"}\n"
//};
//
//
////static char * vertex_shader = {
////	"varying vec4 color;\n"
////	"void main(void ){\n"
////		"color = gl_Vertex;\n"
////		"gl_Position = gl_ModelViewProjectionMatrix*gl_Vertex;\n"
////	"}\n"
////};
////static char * fragment_shader = {
////	"varying vec4 color;\n"
////	"uniform vec4 mycolor;\n"
////	"void main(void){\n"
////	//"	gl_FragColor = clamp(color,0.0,1.0);\n"
////	"	gl_FragColor = mycolor;\n"
////	"}\n"
////};
//
//int main(int agrc, char *argv[]) {
//	osg::ref_ptr< osg::GraphicsContext::Traits > traits = new osg::GraphicsContext::Traits();
//	traits->x = 50; traits->y = 30;
//	traits->width = 640; traits->height = 480;
//	traits->windowDecoration = true;
//	traits->doubleBuffer = true;
//	//traits->glContextVersion = 4.3;
//	osg::ref_ptr< osg::GraphicsContext > gc = osg::GraphicsContext::createGraphicsContext(traits.get());
//	osg::ref_ptr<osgViewer::Viewer> view = new osgViewer::Viewer;
//	
//	view->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
//
//	gc->getState()->setUseModelViewAndProjectionUniforms(true);
//	gc->getState()->setUseVertexAttributeAliasing(true);
//
//	if (gc.valid()) {
//		osg::notify(osg::INFO) << "  GraphicsWindow has been created successfully." << std::endl;
//		gc->setClearColor(osg::Vec4f(0.2f, 0.2f, 0.6f, 1.0f));
//		gc->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//	}
//	else {
//		osg::notify(osg::NOTICE) << "  GraphicsWindow has not been created successfully." << std::endl;
//	}
//	view->getCamera()->setGraphicsContext(gc);
//
//	osg::ref_ptr<osg::Program> program = new osg::Program;
//	program->addShader(new osg::Shader(osg::Shader::VERTEX, vertex_shader));
//	program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragment_shader));
//
//
//	osg::ref_ptr<osg::Geometry> plane = new osg::Geometry;
//	osg::ref_ptr<osg::Vec3Array> vertex_array = new osg::Vec3Array;
//
//	vertex_array->push_back(osg::Vec3(-100.0f, 0.0f, -100.0f));
//	vertex_array->push_back(osg::Vec3(10.0f, 0.0f, -1.0f));
//	vertex_array->push_back(osg::Vec3(1.0f, 0.0f, 1.0f));
//	vertex_array->push_back(osg::Vec3(-1.0f, 0.0f, 1.0f));
//
//	osg::Geode * geode = new osg::Geode;
//	
//	plane->setVertexArray(vertex_array);
//	plane->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, vertex_array->size()));
//
//	plane->setVertexAttribArray(0, vertex_array);
//	plane->setVertexAttribBinding(0, osg::Geometry::BIND_PER_VERTEX);
//
//	plane->getOrCreateStateSet()->setAttributeAndModes(program, osg::StateAttribute::ON);
//
//	geode->addDrawable(plane);
//
//	view->setSceneData(geode);
//	view->run();
//	return 0;
//}
//
//
//
//
//
//
//
//
//
//
//



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

//osg::ref_ptr<osg::Node> CreatNode(osg::ref_ptr<osg::Texture2D> texture) {
//	//创建几何体
//	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
//	osg::ref_ptr<osg::Geometry> gm = new osg::Geometry;
//	osg::ref_ptr<osg::Vec3Array> array = new osg::Vec3Array;
//	array->push_back(osg::Vec3(-1.0f, -1.0f, 0.0f));
//	array->push_back(osg::Vec3(1.0f, -1.0f, 0.0f));
//	array->push_back(osg::Vec3(1.0f, 1.0f, 0.0f));
//	array->push_back(osg::Vec3(-1.0f, 1.0f, 0.0f));
//	gm->setVertexAttribArray(0, array);
//	gm->setVertexAttribBinding(0, osg::Geometry::BIND_PER_VERTEX);
//	//gm->setVertexArray(array);
//	//纹理数组
//	osg::ref_ptr<osg::Vec2Array> tArr0 = new osg::Vec2Array;//创建一个 Vec2Array对象以保存纹理单元 0 的纹理坐标
//	tArr0->push_back(osg::Vec2(0, 0));
//	tArr0->push_back(osg::Vec2(1, 0));
//	tArr0->push_back(osg::Vec2(1, 1));
//	tArr0->push_back(osg::Vec2(0, 1));
//	//gm->setTexCoordArray(0, tArr0.get());
//	gm->setVertexAttribArray(1, tArr0);
//	gm->setVertexAttribBinding(1, osg::Geometry::BIND_PER_VERTEX);
//
//	//创建master着色器程序
//	gm->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
//	gm->getOrCreateStateSet()->setMode(GL_LIGHTING, osg::StateAttribute::OFF);	//关闭光照
//	gm->getOrCreateStateSet()->setTextureAttributeAndModes(0, texture, osg::StateAttribute::ON);
//
//	//创建shader
//	osg::Program * program = new osg::Program;
//	program->addShader(new osg::Shader(osg::Shader::FRAGMENT, fragShader2));
//	program->addShader(new osg::Shader(osg::Shader::VERTEX, vertexShader2));
//
//	int index = 1;
//	osg::Uniform* SineUniform = new osg::Uniform("uIndex", index);
//	gm->getOrCreateStateSet()->addUniform(SineUniform);
//	gm->getOrCreateStateSet()->setAttributeAndModes(program, osg::StateAttribute::ON);
//
//
//	geode->addDrawable(gm.get());
//	return geode;
//}
