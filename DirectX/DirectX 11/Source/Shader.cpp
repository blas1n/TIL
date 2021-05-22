#include "Shader.h"
#include <d3d11.h>
#include <d3dcompiler.h>
#include <fstream>

#pragma comment(lib, "D3DCompiler.lib")

struct MatrixBufferType final
{
	DirectX::XMMATRIX world;
	DirectX::XMMATRIX viewProjection;
};

bool Shader::Initialize(ID3D11Device* device, HWND hWnd)
{
	constexpr static auto MissingShader = TEXT("Missing Shader File");

	constexpr auto vsName = TEXT("Shader/VertexShader.hlsl");
	constexpr auto psName = TEXT("Shader/PixelShader.hlsl");

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

	polygonLayout[1].SemanticName = "COLOR";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
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

	D3D11_BUFFER_DESC matrixBufferDesc;
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = matrixBufferDesc.StructureByteStride = 0;

	result = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixBuffer);
	if (FAILED(result)) return false;

	return true;
}

bool Shader::Render(ID3D11DeviceContext* context, UINT indexCount,
	DirectX::FXMMATRIX world, DirectX::CXMMATRIX view, DirectX::CXMMATRIX projection)
{
	const bool result = SetParameter(context, world, view, projection);
	if (!result) return false;

	context->IASetInputLayout(inputLayout);
	context->VSSetShader(vertexShader, nullptr, 0);
	context->PSSetShader(pixelShader, nullptr, 0);
	context->DrawIndexed(indexCount, 0, 0);
	return true;
}

void Shader::Release() noexcept
{
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

bool Shader::SetParameter(ID3D11DeviceContext* context,
	DirectX::FXMMATRIX worldMatrix, DirectX::CXMMATRIX viewMatrix, DirectX::CXMMATRIX projectionMatrix)
{
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	HRESULT result = context->Map(matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result)) return false;

	MatrixBufferType* data = reinterpret_cast<MatrixBufferType*>(mappedResource.pData);
	data->world = DirectX::XMMatrixTranspose(worldMatrix);
	data->viewProjection = DirectX::XMMatrixTranspose(viewMatrix * projectionMatrix);

	context->Unmap(matrixBuffer, 0);

	UINT bufferNum = 0;
	context->VSSetConstantBuffers(bufferNum, 1, &matrixBuffer);
	return true;
}

void Shader::OutputShaderError(ID3D10Blob* errorMsg, HWND hWnd, LPCTSTR shader)
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
