#include "RenderTexture.h"
#include <d3d11.h>

bool RenderTexture::Initialize(ID3D11Device* device, int width, int height)
{
	D3D11_TEXTURE2D_DESC textureDesc;
	memset(&textureDesc, 0, sizeof(textureDesc));

	textureDesc.Width = width;
	textureDesc.Height = height;
	textureDesc.MipLevels = textureDesc.ArraySize = textureDesc.SampleDesc.Count = 1;
	textureDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	
	HRESULT result = device->CreateTexture2D(&textureDesc, nullptr, &renderTargetTexture);
	if (FAILED(result)) return false;

	D3D11_RENDER_TARGET_VIEW_DESC renderTargetViewDesc;
	memset(&renderTargetViewDesc, 0, sizeof(renderTargetViewDesc));

	renderTargetViewDesc.Format = textureDesc.Format;
	renderTargetViewDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;

	result = device->CreateRenderTargetView(renderTargetTexture, &renderTargetViewDesc, &renderTargetView);
	if (FAILED(result)) return false;

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDesc;
	memset(&shaderResourceViewDesc, 0, sizeof(shaderResourceViewDesc));

	shaderResourceViewDesc.Format = textureDesc.Format;
	shaderResourceViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	shaderResourceViewDesc.Texture2D.MipLevels = 1;

	result = device->CreateShaderResourceView(renderTargetTexture, &shaderResourceViewDesc, &shaderResourceView);
	return SUCCEEDED(result);
}

void RenderTexture::Release() noexcept
{
	if (shaderResourceView)
	{
		shaderResourceView->Release();
		shaderResourceView = nullptr;
	}

	if (renderTargetView)
	{
		renderTargetView->Release();
		renderTargetView = nullptr;
	}

	if (renderTargetTexture)
	{
		renderTargetTexture->Release();
		renderTargetTexture = nullptr;
	}
}

void RenderTexture::SetRenderTarget(ID3D11DeviceContext* context, ID3D11DepthStencilView* view)
{
	context->OMSetRenderTargets(1, &renderTargetView, view);
}

void RenderTexture::ClearRenderTarget(ID3D11DeviceContext* context,
	ID3D11DepthStencilView* view, float r, float g, float b, float a)
{
	float color[4]{ r, g, b, a };
	context->ClearRenderTargetView(renderTargetView, color);
	context->ClearDepthStencilView(view, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
