//DIFFUSE TEXTURE VERTEX SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform	mat4 mvpMatrix;
uniform mat4 model;


in vec3 vertex;
in vec3 normal;
in vec2 uv;

out vec3 FragmentPosition;
out vec2 texCoord; //make sure the texture coord is interpolated
out vec3 transformedNormal;


void main( void ){
	FragmentPosition = vec3( model * vec4(vertex, 1.f));

    gl_Position = mvpMatrix * vec4(vertex, 1.f);
	texCoord = uv;
	transformedNormal = normalize(mat3(transpose(inverse(model))) * normal);
}
