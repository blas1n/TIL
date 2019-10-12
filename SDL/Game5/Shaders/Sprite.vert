#version 330

uniform mat4 uWorldTransform;
uniform mat4 uViewProjection;

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inTexCoord;
layout(location = 2) in vec4 inTexColor;

out vec2 fragTexCoord;
out vec4 fragTexColor;

void main() {
	vec4 pos = vec4(inPosition, 1.0);
	gl_Position = pos * uWorldTransform * uViewProjection;
	fragTexCoord = inTexCoord;
	fragTexColor = inTexColor;
}