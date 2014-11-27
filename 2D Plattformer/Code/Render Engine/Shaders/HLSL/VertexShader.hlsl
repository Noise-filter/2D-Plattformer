#include "Header.hlsli"

VertexIn main(VertexIn input)
{
	VertexIn output;
	output.pos = input.pos;
	output.normal = input.normal;
	output.tex = input.tex;

	output.pos = mul(output.pos, translation);
	output.pos = mul(output.pos, view);
	output.pos = mul(output.pos, proj);

	return output;
}