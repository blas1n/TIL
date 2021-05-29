Texture2D shaderTexture;
SamplerState SampleType;

cbuffer LightBuffer
{
	float4 ambientColor;
	float4 diffuseColor;
	float4 specularColor;
	float3 lightDirection;
	float specularPower;
};

struct PixelInputType
{
	float4 position : SV_POSITION;
	float2 tex : TEXCOORD0;
	float3 normal : NORMAL;
	float3 viewDir : TEXCOORD1;
};

float4 main(PixelInputType input) : SV_TARGET
{
	float4 textureColor = shaderTexture.Sample(SampleType, input.tex);
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);
	float4 color = ambientColor;

	float lightIntensity = saturate(dot(input.normal, -lightDirection));

	if (lightIntensity > 0.0f)
	{
		color += diffuseColor * lightIntensity;
		color = saturate(color);

		float3 reflection = normalize(2 * lightIntensity * input.normal + lightDirection);
		specular = specularColor * pow(saturate(dot(reflection, input.viewDir)), specularPower);
	}

	return saturate(color * textureColor + specular);
}