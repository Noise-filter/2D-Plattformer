#include "Header.hlsli"

[maxvertexcount(4)]
void main(point PixelIn input[1], inout TriangleStream<PixelIn> quads)
{
	PixelIn output;
	output.pos = float4(-1, -1, 0, 1);
	output.normal = float3(1, 0, 0);
	output.tex = float2(0, 1);
	quads.Append(output);

	output.pos = float4(-1, 1, 0, 1);
	output.normal = float3(1, 0, 0);
	output.tex = float2(0, 0);
	quads.Append(output);

	output.pos = float4(1, -1, 0, 1);
	output.normal = float3(1, 0, 0);
	output.tex = float2(1, 1);
	quads.Append(output);

	output.pos = float4(1, 1, 0, 1);
	output.normal = float3(1, 0, 0);
	output.tex = float2(1, 0);
	quads.Append(output);
}