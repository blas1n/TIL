#pragma once

#include "Shader.h"

class ColorShader final : public Shader
{
	std::vector<D3D11_INPUT_ELEMENT_DESC> GetInputElements() const override;
	std::tuple<const TCHAR*, const TCHAR*> GetShaderNames() const override;
};
