#version 330 core
layout (location = 0) in vec4 Position;
layout (location = 1) in vec4 color;

uniform mat4 osg_ModelViewProjectionMatrix;

out vec4 frag_color;

void main()
{
    gl_Position = osg_ModelViewProjectionMatrix * vec4(Position.x, Position.y, Position.z, Position.w);
    frag_color = color;
}