Texture2D shaderTextures[2];
SamplerState SampleType;

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 color0 = shaderTextures[0].Sample(SampleType, input.tex);
	float4 color1 = shaderTextures[1].Sample(SampleType, input.tex);
	return saturate(color0 * color1 * 2.0);
}