#pragma once

class RenderTexture final
{
public:
	bool Initialize(struct ID3D11Device* device, int width, int height);
	void Release() noexcept;

	void SetRenderTarget(struct ID3D11DeviceContext* context, struct ID3D11DepthStencilView* view);
	void ClearRenderTarget(struct ID3D11DeviceContext* context,
		struct ID3D11DepthStencilView* view, float, float, float, float);

	struct ID3D11ShaderResourceView* GetShaderResourceView() const noexcept { return shaderResourceView; }

private:
	struct ID3D11Texture2D* renderTargetTexture;
	struct ID3D11RenderTargetView* renderTargetView;
	ID3D11ShaderResourceView* shaderResourceView;
};
