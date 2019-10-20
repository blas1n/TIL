#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;
uniform mat4 uMatrixPalette[96];

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec3 inNormal;
layout(location = 2) in uvec4 inSkinBones;
layout(location = 3) in vec4 inSkinWeights;
layout(location = 4) in vec2 inTexCoord;

out vec2 fragTexCoord;
out vec3 fragNormal;
out vec3 fragWorldPos;

void main() {
	mat4 skinMat0 = uMatrixPalette[inSkinBones.x] * inSkinWeights.x;
	mat4 skinMat1 = uMatrixPalette[inSkinBones.y] * inSkinWeights.y;
	mat4 skinMat2 = uMatrixPalette[inSkinBones.z] * inSkinWeights.z;
	mat4 skinMat3 = uMatrixPalette[inSkinBones.w] * inSkinWeights.w;

	vec4 skinnedPos = vec4(inPosition, 1.0);
	skinnedPos = skinnedPos * skinMat0
		+ skinnedPos * skinMat1
		+ skinnedPos * skinMat2
		+ skinnedPos * skinMat3;

	skinnedPos *= uWorldTransform;
	fragWorldPos = skinnedPos.xyz;
	gl_Position = skinnedPos * uViewProjection;

	vec4 skinnedNormal = vec4(inNormal, 0.0f);
	skinnedNormal = skinnedNormal * skinMat0
		+ skinnedNormal * skinMat1
		+ skinnedNormal * skinMat2
		+ skinnedNormal * skinMat3;

	fragNormal = (skinnedNormal * uWorldTransform).xyz;
	fragTexCoord = inTexCoord;
}