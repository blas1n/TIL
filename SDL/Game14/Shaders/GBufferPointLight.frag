#version 330

struct PointLight {
	vec3 worldPos;
	vec3 diffuseColor;
	float innerRadius;
	float outerRadius;
};

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;
uniform sampler2D uSpecularPower;

uniform PointLight uPointLight;
uniform vec2 uScreenDimensions;

in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
	vec2 gBufferCoord = gl_FragCoord.xy / uScreenDimensions;

	vec3 gBufferDiffuse = texture(uGDiffuse, gBufferCoord).xyz;
	vec3 gBufferNormal = texture(uGNormal, gBufferCoord).xyz;
	vec3 gBufferWorldPos = texture(uGWorldPos, gBufferCoord).xyz;

	vec3 N = normalize(gBufferNormal);
	vec3 L = normalize(uPointLight.worldPos - gBufferWorldPos);

	vec3 phong = vec3(0.0, 0.0, 0.0);
	float NdotL = dot(N, L);

	if (NdotL > 0) {
		float dist = distance(uPointLight.worldPos, gBufferWorldPos);
		float intensity = smoothstep(uPointLight.innerRadius,
			uPointLight.outerRadius, dist);

		vec3 diffuseColor = mix(uPointLight.diffuseColor, vec3(0.0, 0.0, 0.0), intensity);
		phong = diffuseColor * NdotL;
	}

	outColor = vec4(gBufferDiffuse * phong, 1.0);
}