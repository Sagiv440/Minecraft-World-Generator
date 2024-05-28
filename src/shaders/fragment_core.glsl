#version 440

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	sampler2D diffuseTex;
	sampler2D specularTex;
};

in vec3 vs_position;
in vec3 vs_color;
in vec2 vs_texcoord;
in vec3 vs_normal;

out vec4 fs_color;

uniform Material material;

//Uniforms

uniform vec3 lightPos0;
uniform vec3 cameraPos;

//Functions
vec3 calculateAmbient(Material material)
{
	return vec3(0.4f, 0.4f, 0.4f);
}

vec3 calculateDiffuse(Material material ,vec3 vs_position ,vec3 lightPos)
{
	vec3 posToLightDirVec = normalize(vec3(0.2f, 1.f, 0.5f));
	float diffuse = clamp(dot(posToLightDirVec, vs_normal), 0, 1);
	return material.diffuse * diffuse;
}

vec3 calculateSpecular(Material material , vec3 vs_normal, vec3 vs_position ,vec3 cameraPos, vec3 lightPos)
{
	vec3 lightToPosDirVec = normalize(lightPos - vs_position);
	vec3 ReflactDirVec = normalize(reflect(lightToPosDirVec, normalize(vs_normal)));
	vec3 PosToViewDirVec = normalize(vs_position - cameraPos);
	float speculareConstant = pow(max(dot(PosToViewDirVec, ReflactDirVec), 0), 70);
	return material.specular * speculareConstant;
}


void main()
{

	//Ambient light
	vec3 ambientLight = calculateAmbient(material);

	//Diffuse light

	vec3 diffuseFinal = calculateDiffuse(material, lightPos0, vs_position);

	//Specular light
	vec3 specularFinal = calculateSpecular(material, vs_normal, vs_position, cameraPos, lightPos0);

	//Attenuaton

	//fs_color = vec4(vs_color, 1.f) * (vec4(ambientLight , 1.f) + vec4(diffuseFinal, 1.f));
	fs_color = texture(material.diffuseTex, vs_texcoord) * (vec4(ambientLight , 1.f) + vec4(diffuseFinal, 1.f) + vec4(specularFinal, 1.f) );
	//fs_color = texture(material.diffuseTex, vs_texcoord);
}