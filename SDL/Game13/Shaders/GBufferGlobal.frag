#version 330

struct DirectionalLight {
	vec3 direction;
	vec3 diffuseColor;
	vec3 specularColor;
};

uniform sampler2D uGDiffuse;
uniform sampler2D uGNormal;
uniform sampler2D uGWorldPos;
uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform DirectionalLight uDirLight;

in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
	vec3 gBufferDiffuse = texture(uGDiffuse, fragTexCoord).xyz;
	vec3 gBufferNormal = texture(uGNormal, fragTexCoord).xyz;
	vec3 gBufferWorldPos = texture(uGWorldPos, fragTexCoord).xyz;

	vec3 N = normalize(gBufferNormal);
	vec3 L = normalize(-uDirLight.direction);
	vec3 V = normalize(uCameraPos - gBufferWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0.0)
		phong += uDirLight.diffuseColor * dot(N, L);

	phong = clamp(phong, 0.0, 1.0);
	outColor = vec4(gBufferDiffuse * phong, 1.0);
}