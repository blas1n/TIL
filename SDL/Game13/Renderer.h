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
	void GetScreenDirection(const Vector2& screenPos, Vector3& outStart, Vector3& outDir) const;

	void AddSpriteComponent(class SpriteComponent* sprite);
	void RemoveSpriteComponent(SpriteComponent* sprite);

	void AddMeshComponent(class MeshComponent* mesh);
	void RemoveMeshComponent(MeshComponent* mesh);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);
	class Skeleton* GetSkeleton(const std::string& fileName);
	class Animation* GetAnimation(const std::string& fileName);

	inline void SetViewMatrix(const Matrix4& inView) { view = inView; }

	inline void SetAmbientLight(const Vector3& inAmbientLight) { ambientLight = inAmbientLight; }
	inline DirectionalLight& GetDirectionalLight() { return dirLight; }

	inline float GetScreenWidth() const { return screenWidth; }
	inline float GetScreenHeight() const { return screenHeight; }

private:
	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader);

	std::unordered_map<std::string, Texture*> textures;
	std::unordered_map<std::string, Mesh*> meshes;
	std::unordered_map<std::string, Skeleton*> skeletons;
	std::unordered_map<std::string, Animation*> animations;

	std::vector<SpriteComponent*> spriteComps;
	std::vector<MeshComponent*> meshComps;
	std::vector<class SkeletalMeshComponent*> skMeshComps;

	SDL_Window* window;
	SDL_GLContext context;

	class Game* game;

	Shader* spriteShader;
	Shader* meshShader;
	Shader* skinnedShader;

	class VertexArray* spriteVerts;

	Matrix4 view;
	Matrix4 projection;

	Vector3 ambientLight;
	DirectionalLight dirLight;

	float screenWidth;
	float screenHeight;
};

