//DIFFUSE TEXTURE FRAGMENT SHADER
#version 330 // for glsl version (12 is for older versions , say opengl 2.1


uniform vec3 eyePos;

in vec2 texCoord;
in vec3 transformedNormal;
in vec3 FragmentPosition;

out vec4 fragment_color;

struct Material
{
	sampler2D texture_diffuse1;

	sampler2D specular;
	vec3 ambient;
	float shininess;

};

struct SpotLight
{
	vec3 position;
	vec3 direction;
	float cutoff;
	float outerCutoff;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float intensity;
	int isLightUsed;
};

struct PointLight
{
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float intensity;
	int isLightUsed;
};

struct DirectionalLight
{
	vec3 direction;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	int isLightUsed;
};


uniform Material material;

#define POINT_LIGHT_COUNT 10
#define DIRECTIONAL_LIGHT_COUNT 10
#define SPOTLIGHT_COUNT 10
#define TRUE 1


uniform PointLight pointLights[POINT_LIGHT_COUNT];
uniform SpotLight spotLights[DIRECTIONAL_LIGHT_COUNT];
uniform DirectionalLight dirLights[ SPOTLIGHT_COUNT];


vec3 calculateLighting(vec3 eyeDirection,vec3 lightDirection,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,float attenuation);
vec3 calculatePointLight(vec3 lightPosition,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,float intensity);
vec3 calculateDirectionalLight(vec3 lightDirection,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular);
vec3 calculateSpotLight(float cutoff,float outerCutoff,vec3 eyeDirection,vec3 lightPosition,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,float intensity);


void main( void ) {
	
	vec3 result = vec3(0,0,0);


	

	for(int i = 0; i < POINT_LIGHT_COUNT; i++)
	{
		if(pointLights[i].isLightUsed == 1)
		{
			result += 
			calculatePointLight(pointLights[i].position,pointLights[i].ambient,
			pointLights[i].diffuse,pointLights[i].specular,pointLights[i].intensity);
		}
		
	}

	for(int i = 0; i < SPOTLIGHT_COUNT; i++)
	{
		if(spotLights[i].isLightUsed == 1)
		{
			result += 
			calculateSpotLight(spotLights[i].cutoff,spotLights[i].outerCutoff,spotLights[i].direction,spotLights[i].position,
			spotLights[i].ambient,spotLights[i].diffuse,spotLights[i].specular,spotLights[i].intensity);
		}
		
	}

	for(int i =0 ; i < DIRECTIONAL_LIGHT_COUNT; i++)
	{
		if(dirLights[i].isLightUsed == 1)
		{
			result += 
			calculateDirectionalLight(dirLights[i].direction,dirLights[i].ambient,dirLights[i].diffuse,dirLights[i].specular);
		}
		
	}


	fragment_color = vec4(result ,1);



}

vec3 calculateDirectionalLight(vec3 lightDirection,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular)
{
	vec3 lightDir = normalize(-lightDirection); 
	vec3 eyeDir = normalize(  eyePos -  FragmentPosition); 

	return calculateLighting(eyeDir, lightDir ,lightAmbient,lightDiffuse,lightSpecular,1.0f);
}


vec3 calculateSpotLight(float cutoff,float outerCutoff,vec3 eyeDirection,vec3 lightPosition,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,float intensity)
{
	
	//find light direction
	vec3 spotlightDirection = normalize(eyeDirection);

	//find frag to position direction
	vec3 fragToLightPosition = normalize(FragmentPosition - lightPosition);

	//dotproduct
	float dotResult = dot(fragToLightPosition,spotlightDirection);

	float intensityCoefficient = clamp((dotResult - outerCutoff)/(cutoff - outerCutoff),0.0,1.0f);


	vec3 eyeVector = lightPosition -  FragmentPosition;
	vec3 lightDirection = normalize(eyeVector);
	
	vec3 eyeDir = normalize(  eyePos -  FragmentPosition); 

	float attenuation = intensityCoefficient * intensity/(1.0f + pow(length(eyeVector),2.0f)) ;

	return calculateLighting(eyeDir,lightDirection ,lightAmbient,lightDiffuse,lightSpecular,attenuation );

}


vec3 calculatePointLight(vec3 lightPosition,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,float intensity)
{
	vec3 eyeVector = lightPosition -  FragmentPosition;
	vec3 lightDirection = normalize(eyeVector);
	
	vec3 eyeDir = normalize(  eyePos -  FragmentPosition); 

	float attenuation = intensity/(1.0f + pow(length(eyeVector),2.0f));

	//return vec3(1.0f/length(eyeVector),1.0f/length(eyeVector),1.0f/length(eyeVector));

	return calculateLighting(eyeDir,lightDirection ,lightAmbient,lightDiffuse,lightSpecular,attenuation);
}


vec3 calculateLighting(vec3 eyeDirection,vec3 lightDirection,vec3 lightAmbient,vec3 lightDiffuse,vec3 lightSpecular,float attenuation)
{
	//calculate ambient
	vec3 ambient = lightAmbient * vec3(texture(material.texture_diffuse1,texCoord));

	//calculate diffuse
	float diff = max(dot(transformedNormal,lightDirection),0);
	vec3 diffuse = diff * lightDiffuse * vec3(texture(material.texture_diffuse1,texCoord));
	
	//calculate specular
	vec3 halfAngle = normalize(eyeDirection + lightDirection);
	float spec = pow(max(dot(halfAngle, transformedNormal), 0.0), material.shininess);

	vec3 specular = spec * lightSpecular;

	
	//attenuation * (ambient + diffuse + specular)
	return   ambient + attenuation * ( diffuse + specular);
}
