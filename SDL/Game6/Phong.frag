#version 330

struct DirectionalLight {
	vec3 direction;
	vec3 diffuseColor;
	vec3 specularColor;
};

uniform sampler2D uTexture;
uniform vec3 uCameraPos;
uniform vec3 uAmbientLight;
uniform float uSpecularPower;
uniform DirectionalLight uDirLight;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

out vec4 outColor;

void main() {
	vec3 N = normalize(fragNormal);
	vec3 L = normalize(-uDirLight.direction);
	vec3 V = normalize(uCameraPos - fragWorldPos);
	vec3 R = normalize(reflect(-L, N));

	vec3 Phong = uAmbientLight;
	float NdotL = dot(N, L);
	if (NdotL > 0.0) {
		Phong += uDirLight.diffuseColor * NdotL;
		Phong += uDirLight.specularColor * pow(max(0.0, dot(R, V)), uSpecularPower);
	}

	outColor = texture(uTexture, fragTexCoord) * vec4(Phong, 1.0f);
}