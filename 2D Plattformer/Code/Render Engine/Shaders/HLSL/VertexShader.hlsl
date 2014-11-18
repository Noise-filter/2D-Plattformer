#include "Header.hlsli"

PixelIn main(VertexIn input)
{
	PixelIn output;
	output.pos = float4(input.pos, 0, 0);
	output.normal = float3(1, 0, 0);
	output.tex = float2(0, 0);

	return output;
}