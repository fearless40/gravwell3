
cbuffer PerSession : register(b0)
{
	matrix proj;
}

cbuffer PerFrame : register(b1)
{
	float4 time;
}

cbuffer PerItem : register(b2)
{
	matrix world_view;
}

struct VIn {
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : TexCoord;
};

struct VOut {
	float4 position : SV_POSITION;
	float4 normal : TEXCOORD2;
	float2 uv : TEXCOORD0;
};

VOut main(VIn input)
{
	VOut outpt;
	
	outpt.position = mul(world_view, mul(proj, float4(input.position, 1.0f)));
	outpt.normal = float4(input.normal,1);
	outpt.uv = input.uv;

	return outpt;
}