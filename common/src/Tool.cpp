#include "Tool.h"

osg::ref_ptr<osgViewer::Viewer> Tool::GetViewer() {
	osg::ref_ptr<osgViewer::Viewer> viewer = new osgViewer::Viewer();

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

	double fovy, aspectRatio, zNear, zFar;
	viewer->getCamera()->getProjectionMatrixAsPerspective(fovy, aspectRatio, zNear, zFar);
	double newAspectRatio = double(traits->width) / double(traits->height);
	double aspectRatioChange = newAspectRatio / aspectRatio;
	if (aspectRatioChange != 1.0) {
		viewer->getCamera()->getProjectionMatrix() *= osg::Matrix::scale(1.0 / aspectRatioChange, 1.0, 1.0);
	}

	viewer->getCamera()->setViewport(new osg::Viewport(0, 0, traits->width, traits->height));
	viewer->getCamera()->setGraphicsContext(gc.get());
	return viewer;
}

osg::ref_ptr<osg::Node> Tool::CreateGeodePlane() {
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();//定义一个几何体
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();//定义一个几何体坐标集合
	v->push_back(osg::Vec3(-1.0, 0.0, -1.0));//左下角坐标点
	v->push_back(osg::Vec3(1.0, 0.0, -1.0));//右下角坐标点
	v->push_back(osg::Vec3(1.0, 0.0, 1.0));//右上角坐标点
	v->push_back(osg::Vec3(-1.0, 0.0, 1.0));//左上角坐标点
	geom->setVertexArray(v.get());//将坐标设置到几何体节点中
	//定义颜色数组
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();//定义一个颜色数组颜色
	c->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));//数组的四个参数分别为RGBA，其中A表示透明度
	c->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	c->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	c->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	geom->setColorArray(c.get());//与几何体中进行关联
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);//设置绑定方式为逐点绑定。
	//定义法线
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();//定义了一个法线绑定到该四方体中
	n->push_back(osg::Vec3(0.0, -1.0, 0.0));//法线为指向Y轴负半轴
	geom->setNormalArray(n.get());//添加法线到几何体中
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);//将法线进行绑定
	//设置顶点的关联方式，这里是Quad方式，总共有这么些方式：POINTS,LINES,LINE_STRIP,LINE_LOOP,TRIANGLES,TRIANGLE_STRIP,TRIANGLE_FAN,QUADS,QUAD_STRIP,POLYGON
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	//定义一个几何组结点，并把几何体结点加入到几何组结点当中
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	geode->addDrawable(geom.get());
	geom->setName("TestGeom");
	return geode.get();
}

osg::ref_ptr<osg::Geometry> Tool::CreateGeometryPlane() {
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();//定义一个几何体
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();//定义一个几何体坐标集合
	v->push_back(osg::Vec3(-1.0, 0.0, -1.0));//左下角坐标点
	v->push_back(osg::Vec3(1.0, 0.0, -1.0));//右下角坐标点
	v->push_back(osg::Vec3(1.0, 0.0, 1.0));//右上角坐标点
	v->push_back(osg::Vec3(-1.0, 0.0, 1.0));//左上角坐标点
	geom->setVertexArray(v.get());//将坐标设置到几何体节点中
	//定义颜色数组
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();//定义一个颜色数组颜色
	c->push_back(osg::Vec4(1.0, 0.0, 0.0, 1.0));//数组的四个参数分别为RGBA，其中A表示透明度
	c->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	c->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	c->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	geom->setColorArray(c.get());//与几何体中进行关联
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);//设置绑定方式为逐点绑定。
	//定义法线
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();//定义了一个法线绑定到该四方体中
	n->push_back(osg::Vec3(0.0, -1.0, 0.0));//法线为指向Y轴负半轴
	geom->setNormalArray(n.get());//添加法线到几何体中
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);//将法线进行绑定
	//设置顶点的关联方式，这里是Quad方式，总共有这么些方式：POINTS,LINES,LINE_STRIP,LINE_LOOP,TRIANGLES,TRIANGLE_STRIP,TRIANGLE_FAN,QUADS,QUAD_STRIP,POLYGON
	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));
	return geom;
}

osg::ref_ptr<osg::Geometry> Tool::CreateGeometryPlane2() {
	osg::ref_ptr<osg::Geometry> geom = new osg::Geometry();//定义一个几何体
	osg::ref_ptr<osg::Vec3Array> v = new osg::Vec3Array();//定义一个几何体坐标集合
	v->push_back(osg::Vec3(-1.0, 0.0, -1.0));//左下角坐标点
	v->push_back(osg::Vec3(1.0, 0.0, -1.0));//右下角坐标点
	v->push_back(osg::Vec3(1.0, 0.0, 1.0));//右上角坐标点
	v->push_back(osg::Vec3(-1.0, 0.0, 1.0));//左上角坐标点
	geom->setVertexArray(v.get());//将坐标设置到几何体节点中
	//定义颜色数组
	osg::ref_ptr<osg::Vec4Array> c = new osg::Vec4Array();//定义一个颜色数组颜色
	c->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));//数组的四个参数分别为RGBA，其中A表示透明度
	c->push_back(osg::Vec4(0.0, 1.0, 0.0, 1.0));
	c->push_back(osg::Vec4(0.0, 0.0, 1.0, 1.0));
	c->push_back(osg::Vec4(1.0, 1.0, 1.0, 1.0));
	geom->setColorArray(c.get());//与几何体中进行关联
	geom->setColorBinding(osg::Geometry::BIND_PER_VERTEX);//设置绑定方式为逐点绑定。
	//定义法线
	osg::ref_ptr<osg::Vec3Array> n = new osg::Vec3Array();//定义了一个法线绑定到该四方体中
	n->push_back(osg::Vec3(0.0, -1.0, 0.0));//法线为指向Y轴负半轴
	geom->setNormalArray(n.get());//添加法线到几何体中
	geom->setNormalBinding(osg::Geometry::BIND_OVERALL);//将法线进行绑定
	//设置顶点的关联方式，这里是Quad方式，总共有这么些方式：POINTS,LINES,LINE_STRIP,LINE_LOOP,TRIANGLES,TRIANGLE_STRIP,TRIANGLE_FAN,QUADS,QUAD_STRIP,POLYGON
	//geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	geom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS, 0, 4));

	return geom;
}

osg::ref_ptr<osg::Geode> Tool::CreateGeodeShape() {
	osg::ref_ptr<osg::Geode> geode = new osg::Geode();
	float radius = 0.08;//(以米为单位)
	float height = 0.1;
	osg::TessellationHints *hints = new osg::TessellationHints();
	hints->setDetailRatio(0.5);//表示模型的粗细度。值越大精细度就越大。打个比方，当精细度小时，一个球可能只有几条线组成，很不圆。但是当精细度大时就十分像个球了。
	//绘制一个球型，以(0,0,0)为圆心，以0.08为半径，精细度为hints绘制一个球体
	geode->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), radius), hints));
	//以(0.2,0,0)为坐标，以0.16为盒心，以长0.16米为长，绘制一个立方体
	geode->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.2, 0.0, 0.0), 2 * radius), hints));
	//以底面圆心(0.2,0,0)为坐标，半径为0.08米为圆心，以0.1米为高度，绘制一个圆锥
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0.4, 0.0, 0.0), radius, height), hints));
	//以底面圆心(0.6,0,0)为坐标，半径为0.08米为圆心，以0.1米为高度，绘制一个圆柱体
	geode->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.6, 0.0, 0.0), radius, height), hints));
	//以圆心(0.8,0,0)为坐标，半径为0.08米为圆心，以0.1米为高度，绘制一个胶囊体
	geode->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(0.8, 0.0, 0.0), radius, height), hints));
	return geode.get();
}

osg::ref_ptr<osg::Group> Tool::CreatGroup() {
	osg::ref_ptr<osg::Group> root = new osg::Group();

	float radius = 0.08;//(以米为单位)
	float height = 0.1;
	osg::TessellationHints *hints = new osg::TessellationHints();
	hints->setDetailRatio(0.5);//表示模型的粗细度。值越大精细度就越大。打个比方，当精细度小时，一个球可能只有几条线组成，很不圆。但是当精细度大时就十分像个球了。

	osg::ref_ptr<osg::Geode> sphere = new osg::Geode;
	sphere->addDrawable(new osg::ShapeDrawable(new osg::Sphere(osg::Vec3(0.0, 0.0, 0.0), radius), hints));
	sphere->setName("spere");
	//以(0.2,0,0)为坐标，以0.16为盒心，以长0.16米为长，绘制一个立方体
	osg::ref_ptr<osg::Geode> box = new osg::Geode;
	box->addDrawable(new osg::ShapeDrawable(new osg::Box(osg::Vec3(0.2, 0.0, 0.0), 2 * radius), hints));
	box->setName("box");
	//以底面圆心(0.2,0,0)为坐标，半径为0.08米为圆心，以0.1米为高度，绘制一个圆锥
	osg::ref_ptr<osg::Geode> cone = new osg::Geode;
	cone->addDrawable(new osg::ShapeDrawable(new osg::Cone(osg::Vec3(0.4, 0.0, 0.0), radius, height), hints));
	cone->setName("cone");
	//以底面圆心(0.6,0,0)为坐标，半径为0.08米为圆心，以0.1米为高度，绘制一个圆柱体
	osg::ref_ptr<osg::Geode> cylinder = new osg::Geode;
	cylinder->addDrawable(new osg::ShapeDrawable(new osg::Cylinder(osg::Vec3(0.6, 0.0, 0.0), radius, height), hints));
	cylinder->setName("cylinder");
	//以圆心(0.8,0,0)为坐标，半径为0.08米为圆心，以0.1米为高度，绘制一个胶囊体
	osg::ref_ptr<osg::Geode> capsule = new osg::Geode;
	capsule->addDrawable(new osg::ShapeDrawable(new osg::Capsule(osg::Vec3(0.8, 0.0, 0.0), radius, height), hints));
	capsule->setName("capsule");

	root->addChild(box);
	root->addChild(sphere);
	root->addChild(cone);
	root->addChild(cylinder);
	root->addChild(capsule);
	return root;
}