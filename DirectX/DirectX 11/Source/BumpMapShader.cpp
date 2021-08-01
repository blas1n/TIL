#include "BumpMapShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "DirectionalLight.h"
#include "TextureArray.h"

#pragma comment(lib, "D3DCompiler.lib")

struct MatrixBufferType final
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewProjection;
};

struct CameraBufferType final
{
	DirectX::XMFLOAT3 cameraPos;
	float padding;
};

struct LightBufferType final
{
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT4 specularColor;
	float specularPower;
	DirectX::XMFLOAT3 lightDirection;
};

bool BumpMapShader::Initialize(ID3D11Device* device, HWND hWnd)
{
	constexpr static auto MissingShader = TEXT("Missing Shader File");

	constexpr auto vsName = TEXT("Shader/BumpMapVertexShader.hlsl");
	constexpr auto psName = TEXT("Shader/BumpMapPixelShader.hlsl");

	ID3DBlob* errorMessage = nullptr;

	ID3DBlob* vertexShaderBuffer;
	HRESULT result = D3DCompileFromFile(vsName, nullptr, nullptr,
		"main", "vs_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &vertexShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderError(errorMessage, hWnd, vsName);
		else
			MessageBox(hWnd, vsName, MissingShader, MB_OK);

		return false;
	}

	ID3DBlob* pixelShaderBuffer;
	result = D3DCompileFromFile(psName, nullptr, nullptr,
		"main", "ps_5_0", D3D10_SHADER_ENABLE_STRICTNESS, 0, &pixelShaderBuffer, &errorMessage);

	if (FAILED(result))
	{
		if (errorMessage)
			OutputShaderError(errorMessage, hWnd, psName);
		else
			MessageBox(hWnd, psName, MissingShader, MB_OK);

		return false;
	}

	result = device->CreateVertexShader(vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), nullptr, &vertexShader);
	if (FAILED(result)) return false;

	result = device->CreatePixelShader(pixelShaderBuffer->GetBufferPointer(), pixelShaderBuffer->GetBufferSize(), nullptr, &pixelShader);
	if (FAILED(result)) return false;

	D3D11_INPUT_ELEMENT_DESC polygonLayout[5];
	memset(polygonLayout, 0, sizeof(polygonLayout));

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	polygonLayout[3].SemanticName = "TANGENT";
	polygonLayout[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[3].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	polygonLayout[4].SemanticName = "BINORMAL";
	polygonLayout[4].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[4].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[4].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;

	const UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	result = device->CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	if (FAILED(result)) return false;

	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();

	D3D11_SAMPLER_DESC samplerDesc;
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = samplerDesc.AddressV = samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
	samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	result = device->CreateSamplerState(&samplerDesc, &samplerState);
	if (FAILED(result)) return false;

	D3D11_BUFFER_DESC bufferDesc;
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(MatrixBufferType);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	bufferDesc.MiscFlags = bufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&bufferDesc, nullptr, &matrixBuffer);
	if (FAILED(result)) return false;

	bufferDesc.ByteWidth = sizeof(CameraBufferType);

	result = device->CreateBuffer(&bufferDesc, nullptr, &cameraBuffer);
	if (FAILED(result)) return false;

	bufferDesc.ByteWidth = sizeof(LightBufferType);

	result = device->CreateBuffer(&bufferDesc, nullptr, &lightBuffer);
	if (FAILED(result)) return false;

	return true;
}

bool BumpMapShader::Render(ID3D11DeviceContext* context, UINT indexCount, TextureArray* textures,
	DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection,
	const struct DirectionalLight& light, const DirectX::XMFLOAT3& cameraPos)
{
	const bool result = SetParameter(context, textures, world, view, projection, light, cameraPos);
	if (!result) return false;

	context->IASetInputLayout(inputLayout);
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);
	context->PSSetSamplers(0, 1, &samplerState);
	context->DrawIndexed(indexCount, 0, 0);
	return true;
}

void BumpMapShader::Release() noexcept
{
	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = nullptr;
	}

	if (cameraBuffer)
	{
		cameraBuffer->Release();
		cameraBuffer = nullptr;
	}

	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
	}

	if (samplerState)
	{
		samplerState->Release();
		samplerState = 0;
	}

	if (inputLayout)
	{
		inputLayout->Release();
		inputLayout = nullptr;
	}

	if (pixelShader)
	{
		pixelShader->Release();
		pixelShader = nullptr;
	}

	if (vertexShader)
	{
		vertexShader->Release();
		vertexShader = nullptr;
	}
}

bool BumpMapShader::SetParameter(ID3D11DeviceContext* context, TextureArray* textures,
	DirectX::FXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix,
	const struct DirectionalLight& light, const DirectX::XMFLOAT3& cameraPos)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	MatrixBufferType* matrixData = reinterpret_cast<MatrixBufferType*>(mappedResource.pData);
	matrixData->world = DirectX::XMMatrixTranspose(worldMatrix);
	matrixData->viewProjection = DirectX::XMMatrixTranspose(viewMatrix * projectionMatrix);

	context->Unmap(matrixBuffer, 0);

	context->VSSetConstantBuffers(0, 1, &matrixBuffer);

	result = context->Map(cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	CameraBufferType* cameraData = reinterpret_cast<CameraBufferType*>(mappedResource.pData);
	cameraData->cameraPos = cameraPos;

	context->Unmap(cameraBuffer, 0);

	context->VSSetConstantBuffers(1, 1, &cameraBuffer);

	result = context->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	LightBufferType* lightData = reinterpret_cast<LightBufferType*>(mappedResource.pData);
	lightData->diffuseColor = light.GetDiffuseColor();
	lightData->specularColor = light.GetSpecularColor();
	lightData->specularPower = light.GetSpecularPower();
	lightData->lightDirection = light.GetDirection();

	context->Unmap(lightBuffer, 0);

	context->PSSetConstantBuffers(0, 1, &lightBuffer);

	ID3D11ShaderResourceView** resources = textures->GetTextures();
	context->PSSetShaderResources(0, textures->GetTextureNum(), resources);
	return true;
}

void BumpMapShader::OutputShaderError(ID3D10Blob* errorMsg, HWND hWnd, LPCTSTR shader)
{
	constexpr static auto ErrorShader = TEXT("Error compiling shader. Check shader-error.txt for message.");

	char* compileErrors = reinterpret_cast<char*>(errorMsg->GetBufferPointer());
	const auto bufferSize = errorMsg->GetBufferSize();

	std::ofstream out{ "shader-error.txt" };
	for (size_t i = 0; i < bufferSize; ++i)
		out << compileErrors[i];

	out.close();

	errorMsg->Release();
	MessageBox(hWnd, ErrorShader, shader, MB_OK);
}
