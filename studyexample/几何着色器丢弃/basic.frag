#version 330 core
out vec4 Color;

in GS_OUT {
	vec3 gColor;
}gs_out;

void main() {
	Color = vec4(gs_out.gColor, 1.0f);
}	