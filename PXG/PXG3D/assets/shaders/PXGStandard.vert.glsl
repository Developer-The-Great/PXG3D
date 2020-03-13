//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 MVP;
uniform mat4 model;


layout (location = 0) in vec3 vertex;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 uv;


out vec3 FragmentPosition;
out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 transformedNormal;


void main( void ){
	FragmentPosition = vec3( model * vec4(vertex, 1.f));

    gl_Position = MVP * vec4(vertex, 1.f);
	texCoord = uv;
	transformedNormal = normalize(mat3(transpose(inverse(model))) * normal);
}
