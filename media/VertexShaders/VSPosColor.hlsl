


cbuffer wvp : register(b0) {
	matrix world;
	matrix view;
	matrix proj;
	matrix viewproj;
}

struct VOut {
	float4 position : SV_POSITION;
	float4 color	: COLOR;
};

VOut main( float4 pos : POSITION, float4 color : COLOR ) 
{
	VOut v =(VOut) 0;
	v.position = mul(pos,world);
	//v.position = mul(v.position,view);
	//v.position = mul(v.position,proj);
	v.position = mul( v.position, viewproj );
	v.color = color;
	return v;
}