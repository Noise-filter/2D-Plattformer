#include "Header.hlsli"

[maxvertexcount(4)]
void main(point PixelIn input[1], inout TriangleStream<PixelIn> quads)
{
	PixelIn output;
	output.pos = float4(-1, -1, 0, 1);
	output.normal = float3(1, 0, 0);
	output.tex = float2(0, 1);
	output.pos = mul(output.pos, translation);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);
	quads.Append(output);

	output.pos = float4(-1, 1, 0, 1);
	output.normal = float3(1, 0, 0);
	output.tex = float2(0, 0);
	output.pos = mul(output.pos, translation);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);
	quads.Append(output);

	output.pos = float4(1, -1, 0, 1);
	output.normal = float3(1, 0, 0);
	output.tex = float2(1, 1);
	output.pos = mul(output.pos, translation);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);
	quads.Append(output);

	output.pos = float4(1, 1, 0, 1);
	output.normal = float3(1, 0, 0);
	output.tex = float2(1, 0);
	output.pos = mul(output.pos, translation);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);
	quads.Append(output);
}