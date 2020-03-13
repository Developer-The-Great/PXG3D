//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1


in vec2 texCoord;
out vec4 fragment_color;


struct Material
{
	sampler2D texture_diffuse1;
};

uniform Material material;

void main( void ) {

	fragment_color =  texture(material.texture_diffuse1,texCoord);
}
