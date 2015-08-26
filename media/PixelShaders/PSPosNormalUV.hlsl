

float4 main(float4 position : SV_POSITION, float4 normal : Normal, float2 uv : TexCoord) : SV_TARGET
{
	return float4(normal.x+uv.x,normal.y+uv.y,normal.z+uv.x,1.0);
}