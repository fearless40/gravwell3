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
	VOut output;
	output.position = float4(input.position, 1.0f);
	output.normal = float4(input.normal, 1);
	output.uv = input.uv;
	return output;
}