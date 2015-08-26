

/*
cbuffer PerSession : register(b0)
{
	matrix proj;
}

cbuffer PerFrame : register(b1)
{
	matrix view;
}

*/
cbuffer Data : register(b0)
{
	matrix view;
	matrix proj;
	matrix viewproj;
}

cbuffer PerItem : register(b1)
{
	matrix world;
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

	pos = mul( position, world );
	v.position = mul( pos, view );
	v.position = mul( v.position, proj );
	v.normal = normal;
	v.uv = uv;

	return v;
}