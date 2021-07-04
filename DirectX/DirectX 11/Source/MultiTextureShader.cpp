#include "MultiTextureShader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>
#include "TextureArray.h"

#pragma comment(lib, "D3DCompiler.lib")

struct MatrixBufferType final
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewProjection;
};

struct GammaBufferType final
{
	float gammaCorrection;
	float padding[3];
};

bool MultiTextureShader::Initialize(ID3D11Device* device, HWND hWnd, float gammaCorrection)
{
	constexpr static auto MissingShader = TEXT("Missing Shader File");

	constexpr auto vsName = TEXT("Shader/TextureVertexShader.hlsl");
	constexpr auto psName = TEXT("Shader/MultiTexturePixelShader.hlsl");

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

	D3D11_INPUT_ELEMENT_DESC polygonLayout[2];
	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

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

	bufferDesc.Usage = D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth = sizeof(GammaBufferType);
	bufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA gammaData;
	memset(&gammaData, 0, sizeof(D3D11_SUBRESOURCE_DATA));

	GammaBufferType gamma{ gammaCorrection };
	gammaData.pSysMem = &gamma;

	result = device->CreateBuffer(&bufferDesc, &gammaData, &gammaBuffer);
	if (FAILED(result)) return false;

	return true;
}

bool MultiTextureShader::Render(ID3D11DeviceContext* context, UINT indexCount, TextureArray* textures,
	DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection)
{
	const bool result = SetParameter(context, textures, world, view, projection);
	if (!result) return false;

	context->IASetInputLayout(inputLayout);
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);
	context->PSSetSamplers(0, 1, &samplerState);
	context->DrawIndexed(indexCount, 0, 0);
	return true;
}

void MultiTextureShader::Release() noexcept
{
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

bool MultiTextureShader::SetParameter(ID3D11DeviceContext* context, TextureArray* textures,
	DirectX::FXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	MatrixBufferType* data = reinterpret_cast<MatrixBufferType*>(mappedResource.pData);
	data->world = DirectX::XMMatrixTranspose(worldMatrix);
	data->viewProjection = DirectX::XMMatrixTranspose(viewMatrix * projectionMatrix);

	context->Unmap(matrixBuffer, 0);

	context->VSSetConstantBuffers(0, 1, &matrixBuffer);
	context->PSSetConstantBuffers(0, 1, &gammaBuffer);

	ID3D11ShaderResourceView** resources = textures->GetTextures();
	context->PSSetShaderResources(0, textures->GetTextureNum(), resources);
	return true;
}

void MultiTextureShader::OutputShaderError(ID3D10Blob* errorMsg, HWND hWnd, LPCTSTR shader)
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
