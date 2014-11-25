#include "Header.hlsli"

float4 main(VertexIn input) : SV_Target
{
	return Texture.Sample(LinearSampler, input.tex);

	return float4(1.0f, 1.0f, 1.0f, 1.0f);
}