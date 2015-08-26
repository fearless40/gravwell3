float4 main(float4 position : SV_Position, float2 uv : Texcoord) : SV_TARGET
{
	return float4(uv.x, uv.y, 1.0f, 1.0f);
}