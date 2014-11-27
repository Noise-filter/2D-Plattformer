#include "Header.hlsli"

[maxvertexcount(4)]
void main(point VertexInPos input[1], inout TriangleStream<PixelIn> quads)
{
	float4x4 m;
	m = mul(mul(translation, view), proj);

	VertexIn output1;
	output1.pos = input[0].pos + float4(-1, -1, 0, 0);
	output1.normal = float3(1, 0, 0);
	output1.tex = float2(0, 1);
	output1.pos = mul(output1.pos, m);
	//output1.pos = mul(output1.pos, view);
	//output1.pos = mul(output1.pos, proj);
	quads.Append(output1);

	PixelIn output2;
	output2.pos = input[0].pos + float4(-1, 1, 0, 0);
	output2.normal = float3(1, 0, 0);
	output2.tex = float2(0, 0);
	output2.pos = mul(output2.pos, m);
	//output2.pos = mul(output2.pos, view);
	//output2.pos = mul(output2.pos, proj);
	quads.Append(output2);

	PixelIn output3;
	output3.pos = input[0].pos + float4(1, -1, 0, 0);
	output3.normal = float3(1, 0, 0);
	output3.tex = float2(1, 1);
	output3.pos = mul(output3.pos, m);
	//output3.pos = mul(output3.pos, view);
	//output3.pos = mul(output3.pos, proj);
	quads.Append(output3);

	PixelIn output4;
	output4.pos = input[0].pos + float4(1, 1, 0, 0);
	output4.normal = float3(1, 0, 0);
	output4.tex = float2(1, 0);
	output4.pos = mul(output4.pos, m);
	//output4.pos = mul(output4.pos, view);
	//output4.pos = mul(output4.pos, proj);
	quads.Append(output4);
}