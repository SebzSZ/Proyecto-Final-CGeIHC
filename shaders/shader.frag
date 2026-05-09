#version 330 core

// Entradas desde el vertex shader
in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec4 VertexColor;

// Salida del fragment shader
out vec4 fragColor;

// Constantes para la iluminación
const int MAX_POINT_LIGHTS = 3;
const int MAX_SPOT_LIGHTS = 3;

// Estructuras para las luces
struct Light {
	vec3 color;
	float ambientIntensity;
	float diffuseIntensity;
};

struct DirectionalLight {
	Light base;
	vec3 direction;
};

struct PointLight {
	Light base;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};

struct SpotLight {
	PointLight base;
	vec3 direction;
	float edge;
};

struct Material {
	float specularIntensity;
	float shininess;
};

// Uniforms
uniform int pointLightCount;
uniform int spotLightCount;

uniform DirectionalLight directionalLight;
uniform PointLight pointLights[MAX_POINT_LIGHTS];
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];

uniform sampler2D theTexture;
uniform Material material;
uniform vec3 eyePosition;

// Funciones de iluminacion
vec4 calcLightByDirection(Light light, vec3 direction)
{
	vec4 ambient = vec4(light.color, 1.0) * light.ambientIntensity;

	float diffuseFactor = max(dot(normalize(Normal), normalize(-direction)), 0.0);
	vec4 diffuse = vec4(light.color * light.diffuseIntensity * diffuseFactor, 1.0);

	vec4 specular = vec4(0.0);
	if (diffuseFactor > 0.0)
	{
		vec3 toEye = normalize(eyePosition - FragPos);
		vec3 reflected = normalize(reflect(direction, normalize(Normal)));
		float specFactor = dot(toEye, reflected);

		if (specFactor > 0.0)
		{
			specFactor = pow(specFactor, material.shininess);
			specular = vec4(light.color * material.specularIntensity * specFactor, 1.0);
		}
	}
	return ambient + diffuse + specular;
}

// Luz direccional
vec4 calcDirectionalLight()
{
	return calcLightByDirection(directionalLight.base, directionalLight.direction);
}

// Luz puntual
vec4 calcPointLight(PointLight plight)
{
	vec3 dir = FragPos - plight.position;
	float dist = length(dir);
	dir = normalize(dir);

	vec4 lightVal = calcLightByDirection(plight.base, dir);
	float attenuation = plight.quadratic * dist * dist + plight.linear * dist + plight.constant;

	return lightVal / attenuation;
}

// Suma de todas las pointlights activas
vec4 calcPointLights()
{
	vec4 total = vec4(0.0);
	for (int i = 0; i < pointLightCount; i++)
		total += calcPointLight(pointLights[i]);
	return total;
}

// Calcular un spotlight
vec4 calcSpotLight(SpotLight sLight)
{
	vec3 rayDirection = normalize(FragPos - sLight.base.position);
	float spotFactor = dot(rayDirection, normalize(sLight.direction));

	if (spotFactor > sLight.edge)
	{
		vec4 color = calcPointLight(sLight.base);
		return color * (1.0 - (1.0 - spotFactor) / (1.0 - sLight.edge));
	}
	return vec4(0.0);
}

// Suma de todos los spotlights activos
vec4 calcSpotLights()
{
	vec4 total = vec4(0.0);
	for (int i = 0; i < spotLightCount; i++)
		total += calcSpotLight(spotLights[i]);
	return total;
}

void main()
{
	vec4 texColor = texture(theTexture, TexCoords);
	if (texColor.a < 0.1)
	{
		discard;
	}

	vec4 lightning = calcDirectionalLight() + calcPointLights() + calcSpotLights();
	fragColor = texColor * VertexColor * lightning;
}
