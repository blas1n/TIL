#include "Text.h"
#include <d3d11.h>
#include <vector>
#include "Font.h"
#include "FontShader.h"
#include "Texture.h"

namespace
{
	struct VertexType final
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
	};
}

struct SentenceType final
{
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount, indexCount, maxLength;
	float r, g, b;
};

bool Text::Initialize(ID3D11Device* device, ID3D11DeviceContext* context,
	HWND hWnd, SIZE inScreenSize, const DirectX::XMFLOAT4X4& inView)
{
	constexpr static auto CouldNotInitFont = TEXT("Could not initialize the font object.");
	constexpr static auto CouldNotInitFontShader = TEXT("Could not initialize the font shader object.");

	screenSize = inScreenSize;
	viewMatrix = inView;

	font = new Font{};
	if (!font) return false;

	bool result = font->Initialize(device, TEXT("Asset/fontdata.txt"), TEXT("Asset/font.dds"));
	if (!result)
	{
		MessageBox(hWnd, CouldNotInitFont, TEXT("Error"), MB_OK);
		return false;
	}

	shader = new FontShader{};
	if (!shader) return false;

	result = shader->Initialize(device, hWnd);
	if (!result)
	{
		MessageBox(hWnd, CouldNotInitFontShader, TEXT("Error"), MB_OK);
		return false;
	}

	result = InitializeSentence(&sentence1, device, 16);
	if (!result) return false;

	result = UpdateSentence(sentence1, context, "Hello", { 100, 100 }, 1.0f, 1.0f, 1.0f);
	if (!result) return false;

	result = InitializeSentence(&sentence2, device, 16);
	if (!result) return false;

	result = UpdateSentence(sentence2, context, "GoodBye", { 100, 200 }, 1.0f, 1.0f, 0.0f);
	if (!result) return false;

	return true;
}

bool Text::Render(ID3D11DeviceContext* context, DirectX::FXMMATRIX world, DirectX::CXMMATRIX projection)
{
	bool result = RenderSentence(sentence1, context, world, projection);
	if (!result) return false;

	result = RenderSentence(sentence2, context, world, projection);
	if (!result) return false;

	return true;
}

void Text::Release() noexcept
{
	ReleaseSentence(&sentence2);
	ReleaseSentence(&sentence1);

	if (shader)
	{
		shader->Release();
		delete shader;
		shader = nullptr;
	}

	if (font)
	{
		font->Release();
		delete font;
		font = nullptr;
	}
}

bool Text::InitializeSentence(SentenceType** sentence, ID3D11Device* device, int maxLength)
{
	*sentence = new SentenceType{};
	if (!*sentence) return false;

	(*sentence)->vertexBuffer = (*sentence)->indexBuffer = nullptr;
	(*sentence)->maxLength = maxLength;
	(*sentence)->vertexCount = (*sentence)->indexCount = maxLength * 6;

	std::vector<VertexType> vertices((*sentence)->vertexCount);
	std::vector<DWORD> indices((*sentence)->indexCount);
	
	for (int i = 0; i < (*sentence)->indexCount; ++i)
		indices[i] = i;

	D3D11_BUFFER_DESC bufferDesc;
	memset(&bufferDesc, 0, sizeof(D3D11_BUFFER_DESC));

	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA data;
	memset(&data, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	data.pSysMem = vertices.data();

	HRESULT result = device->CreateBuffer(&bufferDesc, &data, &(*sentence)->vertexBuffer);
	if (FAILED(result)) return false;

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.ByteWidth = sizeof(DWORD) * (*sentence)->indexCount;
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	data.pSysMem = indices.data();

	result = device->CreateBuffer(&bufferDesc, &data, &(*sentence)->indexBuffer);
	if (FAILED(result)) return false;

	return true;
}

bool Text::UpdateSentence(SentenceType* sentence, ID3D11DeviceContext* context,
	const char* text, POINT pos, float r, float g, float b)
{
	sentence->r = r;
	sentence->g = g;
	sentence->b = b;

	const int numLetter = static_cast<int>(strlen(text));
	if (numLetter > sentence->maxLength) return false;
	
	std::vector<VertexType> vertices(sentence->vertexCount);

	const float drawX = (float)(((screenSize.cx / 2) * -1) + pos.x);
	const float drawY = (float)((screenSize.cy / 2) - pos.y);

	font->BuildVertexArray(vertices.data(), text, drawX, drawY);

	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	memcpy(mappedResource.pData, vertices.data(), sizeof(VertexType) * sentence->vertexCount);
	context->Unmap(sentence->vertexBuffer, 0);
	return true;
}

bool Text::RenderSentence(SentenceType* sentence, ID3D11DeviceContext* context,
	DirectX::FXMMATRIX world, DirectX::CXMMATRIX projection)
{
	constexpr static unsigned int stride = sizeof(VertexType);
	constexpr static unsigned int offset = 0;

	context->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
	context->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	const DirectX::XMFLOAT4 pixelColor{ sentence->r, sentence->g, sentence->b, 1.0f };

	bool result = shader->Render(context, sentence->indexCount,
		font->GetTexture(), pixelColor, world, DirectX::XMLoadFloat4x4(&viewMatrix), projection);

	return result;
}

void Text::ReleaseSentence(SentenceType** sentence) noexcept
{
	if (!*sentence) return;
	
	if ((*sentence)->indexBuffer)
	{
		(*sentence)->indexBuffer->Release();
		(*sentence)->indexBuffer = nullptr;
	}

	if ((*sentence)->vertexBuffer)
	{
		(*sentence)->vertexBuffer->Release();
		(*sentence)->vertexBuffer = nullptr;
	}

	delete *sentence;
	*sentence = nullptr;
}
