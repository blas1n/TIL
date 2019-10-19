#include "Renderer.h"
#include <algorithm>
#include <GL/glew.h>
#include "Game.h"
#include "Texture.h"
#include "Mesh.h"
#include "Skeleton.h"
#include "Animation.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"
#include "SkeletalMeshComponent.h"
#include "UiScreen.h"

Renderer::Renderer(Game* inGame)
	: game(inGame),
	textures(),
	meshes(),
	spriteComps(),
	meshComps(),
	skMeshComps(),
	window(nullptr),
	context(),
	spriteShader(nullptr),
	meshShader(nullptr),
	skinnedShader(nullptr),
	spriteVerts(nullptr),
	view(),
	proj(),
	ambientLight(),
	dirLight(),
	screenWidth(0.0f),
	screenHeight(0.0f),
	mirrorView(),
	mirrorTexture(nullptr),
	mirrorBuffer(0) {}

bool Renderer::Initialize(const float inScreenWidth, const float inScreenHeight) {
	screenWidth = inScreenWidth;
	screenHeight = inScreenHeight;
	
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

	window = SDL_CreateWindow("Game 13",
		50,
		50,
		static_cast<int>(screenWidth),
		static_cast<int>(screenHeight),
		SDL_WINDOW_OPENGL);

	if (!window) {
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	context = SDL_GL_CreateContext(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) {
		SDL_Log("Failed to initialize GLEW");
		return false;
	}

	glGetError();

	if (!LoadShaders()) {
		SDL_Log("Failed to load shader");
		return false;
	}

	CreateSpriteVerts();

	if (!CreateMirrorTarget()) {
		SDL_Log("Failed to create render target for mirror");
		return false;
	}

	return true;
}

void Renderer::Shutdown() {
	UnloadData();

	if (mirrorTexture) {
		glDeleteFramebuffers(1, &mirrorBuffer);
		mirrorTexture->Unload();
		delete mirrorTexture;
	}

	delete spriteVerts;

	spriteShader->Unload();
	delete spriteShader;

	meshShader->Unload();
	delete meshShader;

	skinnedShader->Unload();
	delete skinnedShader;

	SDL_GL_DeleteContext(context);
	SDL_DestroyWindow(window);
}

void Renderer::UnloadData() {
	for (auto texture : textures) {
		texture.second->Unload();
		delete texture.second;
	}
	textures.clear();

	for (auto mesh : meshes) {
		mesh.second->Unload();
		delete mesh.second;
	}
	meshes.clear();

	for (auto skeleton : skeletons)
		delete skeleton.second;
	skeletons.clear();

	for (auto animation : animations)
		delete animation.second;
	animations.clear();
}

void Renderer::Draw() {
	Draw3DScene(mirrorBuffer, mirrorView, proj, 0.25f);
	Draw3DScene(0, view, proj, 1.0f, false);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	spriteShader->SetActive();
	spriteVerts->SetActive();

	for (auto sc : spriteComps)
		if (sc->GetVisible())
			sc->Draw(spriteShader);

	for (auto ui : game->GetUiStack())
		ui->Draw(spriteShader);

	SDL_GL_SwapWindow(window);
}

Vector3 Renderer::Unproject(const Vector3& screenPoint) const {
	const auto deviceCoord =
		screenPoint / Vector3{ screenWidth, screenHeight, 1.0f } * 0.5f;
	
	auto unprojection = view * proj;
	unprojection.Invert();
	return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(const Vector2& screenPos, Vector3& outStart, Vector3& outDir) const {
	if (Math::Abs(screenPos.x) > screenWidth * 0.5f || Math::Abs(screenPos.y) > screenHeight * 0.5f)
		return;

	Vector3 screenPoint{ screenPos.x, screenPos.y, 0.0f };
	outStart = Unproject(screenPoint);

	screenPoint.z = 0.9f;
	outDir = Unproject(screenPoint) - outStart;
	outDir.Normalized();
}

void Renderer::AddSpriteComponent(SpriteComponent* sprite) {
	const auto myDrawOrder = sprite->GetDrawOrder();
	auto iter = spriteComps.begin();

	for (; iter != spriteComps.end(); ++iter)
		if (myDrawOrder < (*iter)->GetDrawOrder())
			break;

	spriteComps.insert(iter, sprite);
}

void Renderer::RemoveSpriteComponent(SpriteComponent* sprite) {
	const auto iter = std::find(spriteComps.begin(), spriteComps.end(), sprite);
	spriteComps.erase(iter);
}

void Renderer::AddMeshComponent(MeshComponent* mesh) {
	if (mesh->IsSkeletal())
		skMeshComps.emplace_back(static_cast<SkeletalMeshComponent*>(mesh));
	else
		meshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComponent(MeshComponent* mesh) {
	if (mesh->IsSkeletal()) {
		auto iter = std::find(skMeshComps.begin(), skMeshComps.end(),
			static_cast<SkeletalMeshComponent*>(mesh));
		skMeshComps.erase(iter);
	}
	else {
		const auto iter = std::find(meshComps.begin(), meshComps.end(), mesh);
		meshComps.erase(iter);
	}
}

Texture* Renderer::GetTexture(const std::string& fileName) {
	Texture* texture = nullptr;
	auto iter = textures.find(fileName);

	if (iter != textures.end())
		texture = iter->second;
	else {
		texture = new Texture();
		if (texture->Load(fileName))
			textures.emplace(fileName, texture);
		else {
			delete texture;
			texture = nullptr;
		}
	}

	return texture;
}

Mesh* Renderer::GetMesh(const std::string& fileName) {
	Mesh* mesh = nullptr;
	auto iter = meshes.find(fileName);

	if (iter != meshes.end())
		mesh = iter->second;
	else {
		mesh = new Mesh();
		if (mesh->Load(fileName, this))
			meshes.emplace(fileName, mesh);
		else {
			delete mesh;
			mesh = nullptr;
		}
	}

	return mesh;
}

Skeleton* Renderer::GetSkeleton(const std::string& fileName) {
	Skeleton* skeleton = nullptr;
	auto iter = skeletons.find(fileName);

	if (iter != skeletons.end())
		skeleton = iter->second;
	else {
		skeleton = new Skeleton();
		if (skeleton->Load(fileName))
			skeletons.emplace(fileName, skeleton);
		else {
			delete skeleton;
			skeleton = nullptr;
		}
	}

	return skeleton;
}

Animation* Renderer::GetAnimation(const std::string& fileName) {
	Animation* animation = nullptr;
	auto iter = animations.find(fileName);

	if (iter != animations.end())
		animation = iter->second;
	else {
		animation = new Animation();
		if (animation->Load(fileName))
			animations.emplace(fileName, animation);
		else {
			delete animation;
			animation = nullptr;
		}
	}

	return animation;
}

void Renderer::Draw3DScene(const unsigned int framebuffer, const Matrix4& viewMat, const Matrix4& projMat,
	const float viewPortScale /*= 1.0f*/, const bool lit /*= true*/) {

	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glViewport(0, 0,
		static_cast<int>(screenWidth * viewPortScale),
		static_cast<int>(screenHeight * viewPortScale));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glDepthMask(GL_TRUE);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	meshShader->SetActive();
	meshShader->SetUniformValue("uViewProjection", viewMat * projMat);
	if (lit) SetLightUniforms(meshShader, viewMat);

	for (auto mc : meshComps)
		if (mc->GetVisible())
			mc->Draw(meshShader);

	skinnedShader->SetActive();
	skinnedShader->SetUniformValue("uViewProjection", viewMat * projMat);
	if (lit) SetLightUniforms(skinnedShader, viewMat);

	for (auto smc : skMeshComps)
		if (smc->GetVisible())
			smc->Draw(skinnedShader);
}

bool Renderer::CreateMirrorTarget() {
	const auto width = static_cast<int>(screenWidth) / 4;
	const auto height = static_cast<int>(screenHeight) / 4;

	glGenFramebuffers(1, &mirrorBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, mirrorBuffer);

	mirrorTexture = new Texture();
	mirrorTexture->CreateForRendering(width, height, GL_RGB);

	GLuint depthBuffer;
	glGenRenderbuffers(1, &depthBuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthBuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBuffer);

	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, mirrorTexture->GetTextureId(), 0);

	GLenum drawBuffers[] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, drawBuffers);

	return (glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
}

bool Renderer::LoadShaders() {
	spriteShader = new Shader();
	if (!spriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
		return false;

	spriteShader->SetActive();

	const auto viewProjection = Matrix4::CreateSimpleViewProjection(screenWidth, screenHeight);
	spriteShader->SetUniformValue("uViewProjection", viewProjection);

	view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	proj = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		screenWidth, screenHeight, 25.0f, 10000.0f);

	meshShader = new Shader();
	if (!meshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
		return false;

	meshShader->SetActive();
	meshShader->SetUniformValue("uViewProjection", view * proj);

	skinnedShader = new Shader();
	if (!skinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
		return false;

	skinnedShader->SetActive();
	skinnedShader->SetUniformValue("uViewProjection", view * proj);
	return true;
}

void Renderer::CreateSpriteVerts() {
	float vertices[] {
		-0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 0.f,
		0.5f, 0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 0.f,
		0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 1.f, 1.f,
		-0.5f, -0.5f, 0.f, 0.f, 0.f, 0.0f, 0.f, 1.f
	};

	unsigned int indices[] {
		0, 1, 2,
		2, 3, 0
	};

	spriteVerts = new VertexArray(vertices, 4, VertexArray::Layout::PosNormTex, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader, const Matrix4& viewMat) {
	auto invView = viewMat;
	invView.Invert();
	shader->SetUniformValue("uCameraPos", invView.GetTranslation());

	shader->SetUniformValue("uAmbientLight", ambientLight);

	shader->SetUniformValue("uDirLight.direction", dirLight.direction);
	shader->SetUniformValue("uDirLight.diffuseColor", dirLight.diffuseColor);
	shader->SetUniformValue("uDirLight.specularColor", dirLight.specularColor);
}