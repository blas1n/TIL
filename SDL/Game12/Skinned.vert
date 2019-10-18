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
	mat4 skinnedMatrix0 = uMatrixPalette[inSkinBones.x] * inSkinWeights[inSkinBones.x];
	mat4 skinnedMatrix1 = uMatrixPalette[inSkinBones.y] * inSkinWeights[inSkinBones.y];
	mat4 skinnedMatrix2 = uMatrixPalette[inSkinBones.z] * inSkinWeights[inSkinBones.z];
	mat4 skinnedMatrix3 = uMatrixPalette[inSkinBones.w] * inSkinWeights[inSkinBones.w];

	vec4 skinnedPos = vec4(inPosition, 1.0);
	skinnedPos = (skinnedPos * skinnedMatrix0)
		+ (skinnedPos * skinnedMatrix1)
		+ (skinnedPos * skinnedMatrix2)
		+ (skinnedPos * skinnedMatrix3);

	skinnedPos *= uWorldTransform;
	fragWorldPos = skinnedPos.xyz;
	gl_Position = skinnedPos * uViewProjection;

	vec4 skinnedNormal = vec4(inNormal, 0.0);
	skinnedNormal = (skinnedNormal * skinnedMatrix0)
		+ (skinnedNormal * skinnedMatrix1)
		+ (skinnedNormal * skinnedMatrix2)
		+ (skinnedNormal * skinnedMatrix3);

	fragNormal = (skinnedNormal * uWorldTransform).xyz;
	fragTexCoord = inTexCoord;
}