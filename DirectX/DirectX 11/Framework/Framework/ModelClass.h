#pragma once

#ifndef __MODELCLASS_H__
#define __MODELCLASS_H__

#include <d3d11.h>
#include <DirectXMath.h>
#include <memory>

class ModelClass {
private:
	struct VertexType {
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT4 color;
	};

public:
	bool Init(ID3D11Device*);
	void Release();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool InitBuffers(ID3D11Device*);
	void ReleaseBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	int m_vertexCount;
	int m_indexCount;
};

#endif