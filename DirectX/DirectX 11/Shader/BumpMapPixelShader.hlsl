Texture2D shaderTextures[2];
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float3 lightDirection;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	float4 bumpMap = shaderTextures[1].Sample(SampleType, input.tex);
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	float3 bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;
	bumpNormal = normalize(bumpNormal);

	float lightIntensity = saturate(dot(bumpNormal, -lightDirection));
	return saturate(diffuseColor * lightIntensity) * textureColor;
}