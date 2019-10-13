#include "Renderer.h"
#include <algorithm>
#include <GL/glew.h>
#include "Texture.h"
#include "Mesh.h"
#include "Shader.h"
#include "VertexArray.h"
#include "VertexArray.h"
#include "SpriteComponent.h"
#include "MeshComponent.h"

Renderer::Renderer(Game* inGame)
	: game(inGame),
	textures(),
	meshes(),
	spriteComps(),
	meshComps(),
	window(nullptr),
	context(),
	spriteShader(nullptr),
	meshShader(nullptr),
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

	window = SDL_CreateWindow("Game 7",
		100,
		100,
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
		mc->Draw(meshShader);

	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
	glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

	spriteShader->SetActive();
	spriteVerts->SetActive();

	for (auto sprite : spriteComps)
		sprite->Draw(spriteShader);

	SDL_GL_SwapWindow(window);
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

class Mesh* Renderer::GetMesh(const std::string& fileName) {
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

bool Renderer::LoadShaders() {
	spriteShader = new Shader();
	if (!spriteShader->Load("Shaders/Sprite.vert", "Shaders/Sprite.frag"))
		return false;

	spriteShader->SetActive();

	const auto viewProjection = Matrix4::CreateSimpleViewProjection(screenWidth, screenHeight);
	spriteShader->SetUniformValue("uViewProjection", viewProjection);

	meshShader = new Shader();
	if (!meshShader->Load("Shaders/Phong.vert", "Shaders/Phong.frag"))
		return false;

	meshShader->SetActive();

	view = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
	
	projection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
		screenWidth, screenHeight, 25.0f, 10000.0f);

	meshShader->SetUniformValue("uViewProjection", view * projection);
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

	spriteVerts = new VertexArray(vertices, 4, indices, 6);
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