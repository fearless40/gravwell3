
cbuffer PerSession : register(b0)
{
	matrix proj;
}

cbuffer PerFrame : register(b1)
{
	float time;
}

cbuffer PerItem  : register(b2)
{
	matrix world_view;
}


struct VOut {
	float4 position : SV_POSITION;
	float4 normal : Normal;
	float2 uv : TexCoord;
};

VOut main(  float4 position : Position, float4 normal : Normal, float2 uv : TexCoord ) 
{
	VOut v;
	float4 pos;

	pos = mul( position, world_view );
	v.position = mul( pos, proj );
	v.normal = normal;
	v.uv = uv;

	return v;
}