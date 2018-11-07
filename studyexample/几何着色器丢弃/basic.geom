#version 330 core
in VS_OUT {
	int show;
	vec3 fColor;
}gs_in[];

out GS_OUT {
	vec3 gColor;
}gs_out;

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;

void main(void){
	if(gs_in[0].show == 1 && gs_in[1].show == 1 && gs_in[2].show == 1){
		gl_Position = gl_in[0].gl_Position;
		gs_out.gColor = gs_in[0].fColor;
		EmitVertex();
		gl_Position = gl_in[1].gl_Position;
		gs_out.gColor = gs_in[1].fColor;
		EmitVertex();
		gl_Position = gl_in[2].gl_Position;
		gs_out.gColor = gs_in[2].fColor;
		EmitVertex();
		EndPrimitive();
	}
}	