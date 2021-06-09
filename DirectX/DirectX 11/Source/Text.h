#pragma once

#include "stdafx.h"
#include <DirectXMath.h>

class Text final
{
public:
	[[nodiscard]] bool Initialize(struct ID3D11Device* device, struct ID3D11DeviceContext* context,
		HWND hWnd, SIZE inScreenSize, const DirectX::XMFLOAT4X4& inView);

	[[nodiscard]] bool Render(ID3D11DeviceContext* context, DirectX::FXMMATRIX world, DirectX::CXMMATRIX projection);

	void Release() noexcept;

	bool SetFps(ID3D11DeviceContext* context, int fps);
	bool SetCpu(ID3D11DeviceContext* context, int cpu);

private:
	[[nodiscard]] bool InitializeSentence(struct SentenceType** sentence, ID3D11Device* device, int maxLength);

	[[nodiscard]] bool UpdateSentence(SentenceType* sentence, ID3D11DeviceContext* context,
		const char* text, POINT pos, float r, float g, float b);

	[[nodiscard]] bool RenderSentence(SentenceType* sentence,
		ID3D11DeviceContext* context, DirectX::FXMMATRIX world, DirectX::CXMMATRIX projection);

	void ReleaseSentence(SentenceType** sentence) noexcept;
	
private:
	DirectX::XMFLOAT4X4 viewMatrix;

	class Font* font;
	class FontShader* shader;
	
	SentenceType* sentence1;
	SentenceType* sentence2;

	SIZE screenSize;
};
