struct VertexIn
{
	float2 pos : Position;
};

cbuffer EveryObject2D : register(b0)
{
	float4x4 translation;
};

cbuffer ColorData : register(b0)
{
	float4 color;
};

struct PixelIn
{
	float4 pos : SV_Position;
	float3 normal : NORMAL;
	float2 tex : UV;
};

Texture2D Texture : register(t0);
SamplerState LinearSampler : register(s0);