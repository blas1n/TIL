#version 330

uniform sampler2D uTexture;

in vec2 fragTexCoord;
in vec4 fragTexColor;

out vec4 outColor;

void main() {
	outColor = texture(uTexture, fragTexCoord);

	if (outColor.a > 0.1)
		outColor += fragTexColor * 0.5;
}