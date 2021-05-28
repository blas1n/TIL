#include "LightShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "DirectionalLight.h"
#include "Texture.h"

#pragma comment(lib, "D3DCompiler.lib")

struct MatrixBufferType final
{
	DirectX::XMFLOAT4X4 world;
	DirectX::XMFLOAT4X4 viewProjection;
};

struct LightBufferType final
{
	DirectX::XMFLOAT4 ambientColor;
	DirectX::XMFLOAT4 diffuseColor;
	DirectX::XMFLOAT3 lightDir;
	float padding;
};

bool LightShader::Initialize(ID3D11Device* device, HWND hWnd)
{
	constexpr static auto MissingShader = TEXT("Missing Shader File");

	constexpr auto vsName = TEXT("Shader/LightVertexShader.hlsl");
	constexpr auto psName = TEXT("Shader/LightPixelShader.hlsl");

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

	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].SemanticIndex = polygonLayout[0].InputSlot =
		polygonLayout[0].AlignedByteOffset = polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].SemanticIndex = polygonLayout[1].InputSlot = polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].SemanticIndex = polygonLayout[2].InputSlot = polygonLayout[2].InstanceDataStepRate = 0;

	const UINT numElements = sizeof(polygonLayout) / sizeof(polygonLayout[0]);
	result = device->CreateInputLayout(polygonLayout, numElements,
		vertexShaderBuffer->GetBufferPointer(), vertexShaderBuffer->GetBufferSize(), &inputLayout);

	if (FAILED(result)) return false;

	vertexShaderBuffer->Release();
	pixelShaderBuffer->Release();

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);
	if (FAILED(result)) return false;

	D3D11_BUFFER_DESC lightBufferDesc;
	lightBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	lightBufferDesc.ByteWidth = sizeof(LightBufferType);
	lightBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	lightBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightBufferDesc.MiscFlags = matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&lightBufferDesc, nullptr, &lightBuffer);
	if (FAILED(result)) return false;

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

	return true;
}

bool LightShader::Render(ID3D11DeviceContext* context, UINT indexCount, Texture* texture,
	DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection, const DirectionalLight& light)
{
	const bool result = SetParameter(context, texture, world, view, projection, light);
	if (!result) return false;

	context->IASetInputLayout(inputLayout);
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);
	context->PSSetSamplers(0, 1, &samplerState);
	context->DrawIndexed(indexCount, 0, 0);
	return true;
}

void LightShader::Release() noexcept
{
	if (samplerState)
	{
		samplerState->Release();
		samplerState = 0;
	}

	if (lightBuffer)
	{
		lightBuffer->Release();
		lightBuffer = nullptr;
	}

	if (matrixBuffer)
	{
		matrixBuffer->Release();
		matrixBuffer = nullptr;
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

bool LightShader::SetParameter(ID3D11DeviceContext* context, Texture* texture, DirectX::FXMMATRIX worldMatrix,
	DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix, const DirectionalLight& light)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	MatrixBufferType* matrixData = reinterpret_cast<MatrixBufferType*>(mappedResource.pData);
	DirectX::XMStoreFloat4x4(&matrixData->world, DirectX::XMMatrixTranspose(worldMatrix));
	DirectX::XMStoreFloat4x4(&matrixData->viewProjection, DirectX::XMMatrixTranspose(viewMatrix * projectionMatrix));

	context->Unmap(matrixBuffer, 0);

	UINT bufferNum = 0;
	context->VSSetConstantBuffers(bufferNum, 1, &matrixBuffer);

	const auto resource = texture->GetTexture();
	context->PSSetShaderResources(0, 1, &resource);

	result = context->Map(lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	LightBufferType* lightData = reinterpret_cast<LightBufferType*>(mappedResource.pData);
	lightData->ambientColor = light.GetAmbientColor();
	lightData->diffuseColor = light.GetDiffuseColor();
	lightData->lightDir = light.GetDirection();
	lightData->padding = 0.0f;

	context->Unmap(lightBuffer, 0);

	bufferNum = 0;
	context->PSSetConstantBuffers(bufferNum, 1, &lightBuffer);

	return true;
}

void LightShader::OutputShaderError(ID3D10Blob* errorMsg, HWND hWnd, LPCTSTR shader)
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