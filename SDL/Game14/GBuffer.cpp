#include "GBuffer.h"
#include <GL/glew.h>
#include "Texture.h"

bool GBuffer::Create(const int width, const int height) {
	glGenFramebuffers(1, &bufferId);
	glBindFramebuffer(GL_FRAMEBUFFER, bufferId);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT,
		width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
		GL_RENDERBUFFER, depthBuffer);

	for (auto i = 0; i < static_cast<int>(Type::NUM_GBUFFER_TEXTURES); ++i) {
		auto tex = new Texture();
		tex->CreateForRendering(width, height, GL_RGB32F);
		textures.emplace_back(tex);
		glFramebufferTexture(GL_FRAMEBUFFER,
			GL_COLOR_ATTACHMENT0 + i, tex->GetTextureId(), 0);
	}

	std::vector<GLenum> attachments;
	for (auto i = 0; i < static_cast<int>(Type::NUM_GBUFFER_TEXTURES); ++i)
		attachments.emplace_back(GL_COLOR_ATTACHMENT0 + i);

	glDrawBuffers(static_cast<GLsizei>(attachments.size()), attachments.data());
	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

void GBuffer::Destroy() {
	glDeleteFramebuffers(1, &bufferId);
	for (auto t : textures) {
		t->Unload();
		delete t;
	}
}

Texture* GBuffer::GetTexture(Type type) {
	return textures.size() > 0 ?
		textures[static_cast<int>(type)] : nullptr;
}

void GBuffer::SetTextureActive() {
	for (auto i = 0; i < static_cast<int>(Type::NUM_GBUFFER_TEXTURES); ++i)
		textures[i]->SetActive(i);
}