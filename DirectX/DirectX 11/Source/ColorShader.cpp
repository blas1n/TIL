#include "ColorShader.h"
#include <d3d11.h>

std::vector<D3D11_INPUT_ELEMENT_DESC> ColorShader::GetInputElements() const
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> elements(2);

	elements[0].SemanticName = "POSITION";
	elements[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	elements[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elements[0].SemanticIndex = elements[0].InputSlot = elements[0].AlignedByteOffset = elements[0].InstanceDataStepRate = 0;

	elements[1].SemanticName = "COLOR";
	elements[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	elements[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	elements[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
	elements[1].SemanticIndex = elements[0].InputSlot = elements[0].InstanceDataStepRate = 0;

	return elements;
}

std::tuple<const TCHAR*, const TCHAR*> ColorShader::GetShaderNames() const
{
	return std::make_tuple(TEXT("Shader/ColorVertexShader.hlsl"), TEXT("Shader/ColorPixelShader.hlsl"));
}
