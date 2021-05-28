Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float3 lightDirection;
	float padding;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
	float4 color = ambientColor;

	float lightIntensity = saturate(dot(input.normal, -lightDirection));

	if (lightIntensity > 0.0f)
		color += diffuseColor * lightIntensity;

	return saturate(color) * textureColor;
}