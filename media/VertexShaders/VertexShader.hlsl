
// Need this so when I specify a matrix I do not have to transpose it in C++ code. 
#pragma pack_matrix( row_major )

float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}