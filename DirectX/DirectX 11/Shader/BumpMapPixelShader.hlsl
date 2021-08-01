Texture2D shaderTextures[3];
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 diffuseColor;
	float4 specularColor;
	float specularPower;
	float3 lightDirection;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 binormal : BINORMAL;
	float3 viewDir : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor = shaderTextures[0].Sample(SampleType, input.tex);
	float4 bumpMap = shaderTextures[1].Sample(SampleType, input.tex);
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	float3 bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;
	bumpNormal = normalize(bumpNormal);

	float lightIntensity = saturate(dot(bumpNormal, -lightDirection));
	float4 color = saturate(diffuseColor * lightIntensity) * textureColor;

	if (lightIntensity > 0.0f)
	{
		float4 specularIntensity = shaderTextures[2].Sample(SampleType, input.tex);
		float3 reflection = normalize(2.0f * lightIntensity * bumpNormal - lightDirection);
		float4 specular = pow(saturate(dot(reflection, input.viewDir)), specularPower);
		specular = specular * specularIntensity;
		color = saturate(color + specular);
	}

	return color;
}