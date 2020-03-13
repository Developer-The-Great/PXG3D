//DIFFUSE COLOR VERTEX SHADER

#version 330 core

layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;

uniform mat4 MVP;

void main( void ){
    	gl_Position = MVP * vec4(vertex, 1.f);
}
