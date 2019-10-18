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
	projection(),
	ambientLight(),
	dirLight(),
	screenWidth(0.0f),
	screenHeight(0.0f) {}

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

	window = SDL_CreateWindow("Game 12",
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
	return true;
}

void Renderer::Shutdown() {
	delete spriteVerts;

	spriteShader->Unload();
	delete spriteShader;

	meshShader->Unload();
	delete meshShader;

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
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);

	meshShader->SetActive();
	meshShader->SetUniformValue("uViewProjection", view * projection);
	SetLightUniforms(meshShader);

	for (auto mc : meshComps)
		if (mc->GetVisible())
			mc->Draw(meshShader);

	skinnedShader->SetActive();
	skinnedShader->SetUniformValue("uViewProjection", view * projection);
	SetLightUniforms(skinnedShader);

	for (auto smc : skMeshComps)
		if (smc->GetVisible())
			smc->Draw(skinnedShader);

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
	
	auto unprojection = view * projection;
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
	if (const auto sk = dynamic_cast<SkeletalMeshComponent*>(mesh))
		skMeshComps.emplace_back(sk);
	else
		meshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComponent(MeshComponent* mesh) {
	const auto iter = std::find(meshComps.begin(), meshComps.end(), mesh);
	meshComps.erase(iter);
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

bool Renderer::LoadShaders() {
	spriteShader = new Shader();
	if (!spriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
		return false;

	spriteShader->SetActive();

	const auto viewProjection = Matrix4::CreateSimpleViewProjection(screenWidth, screenHeight);
	spriteShader->SetUniformValue("uViewProjection", viewProjection);

	view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);

	projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		screenWidth, screenHeight, 25.0f, 10000.0f);

	meshShader = new Shader();
	if (!meshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
		return false;

	meshShader->SetActive();
	meshShader->SetUniformValue("uViewProjection", view * projection);

	skinnedShader = new Shader();
	if (!skinnedShader->Load("Shaders/Skinned.vert", "Shaders/Phong.frag"))
		return false;

	skinnedShader->SetActive();
	skinnedShader->SetUniformValue("uViewProjection", view * projection);
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

void Renderer::SetLightUniforms(Shader* shader) {
	auto invView = view;
	invView.Invert();
	shader->SetUniformValue("uCameraPos", invView.GetTranslation());

	shader->SetUniformValue("uAmbientLight", ambientLight);

	shader->SetUniformValue("uDirLight.direction", dirLight.direction);
	shader->SetUniformValue("uDirLight.diffuseColor", dirLight.diffuseColor);
	shader->SetUniformValue("uDirLight.specularColor", dirLight.specularColor);
}