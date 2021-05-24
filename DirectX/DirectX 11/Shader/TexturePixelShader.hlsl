Texture2D shaderTexture;
SamplerState SampleType;

struct PixelInputType
{
	float position : SV_POSITION;
	float tex : TEXCOORD0;
};

float4 main(PixelInputType input) : SV_TARGET
{
	return shaderTexture.Sample(SampleType, input.tex);
}