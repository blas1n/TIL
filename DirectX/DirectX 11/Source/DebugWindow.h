#pragma once

class DebugWindow final
{
public:
	bool Initialize(struct ID3D11Device* device, int inScreenWidth, int inScreenHeight, int inBitmapWidth, int inBitmapHeight);
	bool Render(struct ID3D11DeviceContext* context, int posX, int posY);
	void Release() noexcept;

	int GetIndexCount() const noexcept { return indexCount; }

private:
	bool UpdateBuffer(ID3D11DeviceContext* context, int posX, int posY);
	void RenderBuffer(ID3D11DeviceContext* context);

private:
	struct ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;

	int vertexCount, indexCount;
	int screenWidth, screenHeight;
	int bitmapWidth, bitmapHeight;
	int prevPosX, prevPosY;
};
