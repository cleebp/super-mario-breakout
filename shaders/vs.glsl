#version 330 core

//layout( location = 0 ) in vec2 vertex;
//layout( location = 1 ) in vec2 texcoord;
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 texCoords>

out vec2 texcoord;

uniform mat4 model;
uniform mat4 projection;

void main() 
{
	texcoord = vertex.zw;
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}