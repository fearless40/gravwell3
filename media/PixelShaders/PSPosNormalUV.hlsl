struct PIn {
	float4 position : SV_POSITION;
	float4 normal : TEXCOORD2;
	float2 uv : TEXCOORD0;
};

float4 main(PIn pin) : SV_TARGET
{
	return float4(pin.normal.x+ pin.uv.x, pin.normal.y+ pin.uv.y, pin.normal.z+ pin.uv.x,1.0);
}