struct VertexIn
{
	float4 pos : SV_POSITION;
	float3 normal : NORMAL;
	float2 tex : UV;
};

struct VertexInPos
{
	float4 pos : POSITION2;
};

cbuffer EveryObject2D : register(b0)
{
	float4x4 translation;
	//float4 color;
};

cbuffer EveryFrame : register(b1)
{
	float4x4 view;
	float4x4 proj;
};

struct PixelIn
{
	float4 pos : SV_Position;
	float3 normal : NORMAL;
	float2 tex : UV;
};

Texture2D Texture : register(t0);
SamplerState LinearSampler : register(s0);