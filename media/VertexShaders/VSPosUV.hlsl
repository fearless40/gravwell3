
struct VOUT {
	float4 position : SV_POSITION;
	float2 uv : Texcoord;
};

VOUT main( float4 pos : POSITION, float2 uv : Texcoord ) 
{
	VOUT v;
	v.position = pos;
	v.uv = uv;
	return v;
}