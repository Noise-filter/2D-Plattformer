#include "Header.hlsli"

VertexInPos main(float4 pos : POSITION) : SV_POSITION
{
	VertexInPos output;
	output.pos = pos;

	//output.pos = mul(output.pos, translation);
	//output.pos = mul(output.pos, view);
	//output.pos = mul(output.pos, proj);

	return output;
}