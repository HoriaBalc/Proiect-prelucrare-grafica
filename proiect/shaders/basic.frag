#version 410 core

in vec3 fPosition;
in vec3 fNormal;
in vec2 fTexCoords;

out vec4 fColor;

//matrices
uniform float fogDensity = 0.00f;
uniform mat4 model;
uniform mat4 view;
uniform mat3 normalMatrix;
//lighting
uniform vec3 lightDir;
uniform vec3 lightColor;
uniform vec3 lightDir1;
uniform vec3 lightColor1;
// textures
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

//components
vec3 ambient;
float ambientStrength = 0.2f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.5f;
vec3 ambient1;
vec3 diffuse1;
vec3 specular1;
void computeDirLight()
{
    //compute eye space coordinates
    vec4 fPosEye = view * model * vec4(fPosition, 1.0f);
    vec3 normalEye = normalize(normalMatrix * fNormal);

    //normalize light direction
    vec3 lightDirN = vec3(normalize(view * vec4(lightDir, 0.0f)));
	vec3 lightDirN1 = vec3(normalize(view * vec4(lightDir1, 0.0f)));

    //compute view direction (in eye coordinates, the viewer is situated at the origin
    vec3 viewDir = normalize(- fPosEye.xyz);

    //compute ambient light
    ambient = ambientStrength * lightColor;
    ambient1 = ambientStrength * lightColor1;

    //compute diffuse light
    diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
    diffuse1 = max(dot(normalEye, lightDirN1), 0.0f) * lightColor1;

    //compute specular light
    vec3 reflectDir = reflect(-lightDirN, normalEye);
	vec3 reflectDir1 = reflect(-lightDirN1, normalEye);

    float specCoeff = pow(max(dot(viewDir, reflectDir), 0.0f), 32);
	float specCoeff1 = pow(max(dot(viewDir, reflectDir1), 0.0f), 32);

    specular = specularStrength * specCoeff * lightColor;
    specular1 = specularStrength * specCoeff1 * lightColor1;
    
    

    
	
    
	
	

}

float computeFog()
{
    vec4 fPosEye = view * model * vec4(fPosition, 1.0f);
    float fragmentDistance = length(fPosEye);
    float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

    return clamp(fogFactor, 0.0f, 1.0f);
}

void main() 
{
vec3 fogColor = vec3 (0.5f,0.5f,0.5f);
    
    float fogFactor = computeFog();
    computeDirLight();

    //compute final vertex color
    vec3 color = min((ambient + diffuse) * texture(diffuseTexture, fTexCoords).rgb + specular * texture(specularTexture, fTexCoords).rgb, 1.0f);
    vec3 color1 = min((ambient1 + diffuse1) * texture(diffuseTexture, fTexCoords).rgb + specular1 * texture(specularTexture, fTexCoords).rgb, 1.0f);

    
fColor = vec4 (mix(fogColor,color+color1,fogFactor),1.0f);
}
