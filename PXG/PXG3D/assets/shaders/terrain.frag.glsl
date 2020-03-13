//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1

uniform sampler2D diffuseTexture;
uniform sampler2D r1DiffuseTexture;
uniform sampler2D g1DiffuseTexture;
uniform sampler2D b1DiffuseTexture;
uniform sampler2D a1DiffuseTexture;


uniform vec3 eyePos;

in vec2 texCoord;
in vec3 transformedNormal;
in vec3 FragmentPosition;

out vec4 fragment_color;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	vec3 ambient;
	float shininess;

};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
 
};

uniform PointLight pointLight;

uniform DirectionalLight dirLight;

uniform Material material;


vec3 calculateLighting(vec3 eyeDirection,vec3 lightDirection,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,float attenuation,vec3 diffuseColor);

vec3 calculateDirectionalLight(vec3 lightDirection,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,vec3 diffuseColor);

void main( void ) {
	
	vec3 result = vec3(0,0,0);
	vec3 diffuseColor = vec3(0,0,0);
	//result += 

	//result += calculatePointLight(result,pointLight.position,pointLight.ambient,pointLight.diffuse,pointLight.specular);
	vec4 fragmentSplatColor = texture( diffuseTexture,texCoord);

	diffuseColor += vec3(texture(r1DiffuseTexture,texCoord)) * fragmentSplatColor.r;
	diffuseColor += vec3(texture(g1DiffuseTexture,texCoord)) * fragmentSplatColor.g;
	diffuseColor += vec3(texture(b1DiffuseTexture,texCoord)) * fragmentSplatColor.b;
	diffuseColor += vec3(texture(a1DiffuseTexture,texCoord)) * fragmentSplatColor.a;

	result += calculateDirectionalLight(dirLight.direction,dirLight.ambient,dirLight.diffuse,dirLight.specular,diffuseColor);

	fragment_color = vec4(result ,1);

}

vec3 calculateDirectionalLight(vec3 lightDirection,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,vec3 diffuseColor)
{
	vec3 lightDir = normalize(-lightDirection); 
	vec3 eyeDir = normalize(  eyePos -  FragmentPosition); 

	return calculateLighting(eyeDir, lightDir ,lightAmbient,lightDiffuse,lightSpecular,1.0f,diffuseColor);
}




vec3 calculateLighting(vec3 eyeDirection,vec3 lightDirection,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,float attenuation,vec3 diffuseColor)
{
	//calculate ambient
	vec3 ambient = lightAmbient * diffuseColor;

	//calculate diffuse
	float diff = max(dot(transformedNormal,lightDirection),0);
	vec3 diffuse = diff * lightDiffuse * diffuseColor;
	
	//calculate specular
	vec3 halfAngle = normalize(eyeDirection + lightDirection);
	float spec = pow(max(dot(halfAngle, transformedNormal), 0.0), material.shininess);

	vec3 specular = spec * lightSpecular;

	
	//attenuation * (ambient + diffuse + specular)
	return attenuation * (ambient  +diffuse);
}
