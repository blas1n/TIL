#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <SDL/SDL.h>
#include "Math.h"

struct DirectionalLight {
	Vector3 direction;
	Vector3 diffuseColor;
	Vector3 specularColor;
};

class Renderer {
public:
	Renderer(class Game* inGame);

	bool Initialize(float inScreenWidth, float inScreenHeight);
	void Shutdown();
	void UnloadData();

	void Draw();

	Vector3 Unproject(const Vector3& screenPoint) const;
	void GetScreenDirection(Vector2 screenPos, Vector3& outStart, Vector3& outDir) const;

	void AddSpriteComponent(class SpriteComponent* sprite);
	void RemoveSpriteComponent(class SpriteComponent* sprite);

	void AddMeshComponent(class MeshComponent* mesh);
	void RemoveMeshComponent(class MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);

	inline void SetViewMatrix(const Matrix4& inView) { view = inView; }

	inline void SetAmbientLight(const Vector3& inAmbientLight) { ambientLight = inAmbientLight; }
	inline DirectionalLight& GetDirectionalLight() { return dirLight; }

	inline float GetScreenWidth() const { return screenWidth; }
	inline float GetScreenHeight() const { return screenHeight; }

private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader);

	std::unordered_map<std::string, class Texture*> textures;
	std::unordered_map<std::string, class Mesh*> meshes;

	std::vector<class SpriteComponent*> spriteComps;
	std::vector<class MeshComponent*> meshComps;

	SDL_Window* window;
	SDL_GLContext context;

	class Game* game;

	class Shader* spriteShader;
	class Shader* meshShader;

	class VertexArray* spriteVerts;

	Matrix4 view;
	Matrix4 projection;

	Vector3 ambientLight;
	DirectionalLight dirLight;

	float screenWidth;
	float screenHeight;
};

