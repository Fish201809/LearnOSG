#version 330 core
layout(location = 0) in vec3 vPosition;
layout(location = 1) in float vShow;
layout(location = 2) in vec3 vColor;
uniform mat4 osg_ModelViewProjectionMatrix;

out VS_OUT {
	int show;
	vec3 fColor;
}vs_out; 

void main(void ){
	gl_Position = osg_ModelViewProjectionMatrix * vec4(vPosition,1.0f);
	vs_out.show = int(vShow);
	vs_out.fColor = vColor;
}