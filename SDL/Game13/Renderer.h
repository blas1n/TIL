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

	void AddPointLight(class PointLightComponent* light);
	void RemovePointLight(PointLightComponent* light);

	class Texture* GetTexture(const std::string& fileName);
	class Mesh* GetMesh(const std::string& fileName);
	class Skeleton* GetSkeleton(const std::string& fileName);
	class Animation* GetAnimation(const std::string& fileName);

	inline void SetViewMatrix(const Matrix4& inView) { view = inView; }

	inline void SetAmbientLight(const Vector3& inAmbientLight) { ambientLight = inAmbientLight; }
	inline DirectionalLight& GetDirectionalLight() { return dirLight; }

	inline float GetScreenWidth() const { return screenWidth; }
	inline float GetScreenHeight() const { return screenHeight; }

	inline Texture* GetMirrorTexture() const { return mirrorTexture; }
	inline void SetMirrorView(const Matrix4& inMirrorView) { mirrorView = inMirrorView; }
	
	inline class GBuffer* GetGBuffer() const { return gBuffer; }

private:
	void Draw3DScene(unsigned int framebuffer, const Matrix4& viewMat, const Matrix4& projMat,
		float viewPortScale = 1.0f, bool lit = true);

	void DrawFromGBuffer();
	bool CreateMirrorTarget();

	bool LoadShaders();
	void CreateSpriteVerts();
	void SetLightUniforms(class Shader* shader, const Matrix4& viewMat);

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
	Matrix4 proj;

	Vector3 ambientLight;
	DirectionalLight dirLight;

	float screenWidth;
	float screenHeight;

	Matrix4 mirrorView;
	Texture* mirrorTexture;
	unsigned int mirrorBuffer;

	GBuffer* gBuffer;
	Shader* gGlobalShader;

	Shader* gPointLightShader;
	std::vector<PointLightComponent*> pointLights;
	Mesh* pointLightMesh;
};