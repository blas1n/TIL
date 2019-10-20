#version 330

uniform sampler2D uTexture;
uniform float uSpecularPower;

in vec2 fragTexCoord;
in vec3 fragNormal;
in vec3 fragWorldPos;

layout(location = 0) out vec3 outDiffuse;
layout(location = 1) out vec3 outNormal;
layout(location = 2) out vec3 outWorldPos;
layout(location = 3) out vec3 outSpecularPower;

void main() {
	outDiffuse = texture(uTexture, fragTexCoord).xyz;
	outNormal = fragNormal;
	outWorldPos = fragWorldPos;
	outSpecularPower = vec3(uSpecularPower);
}