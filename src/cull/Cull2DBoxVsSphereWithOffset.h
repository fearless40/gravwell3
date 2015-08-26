#include "../util/Math.h"
#include <DirectXCollision.h>

using namespace Math;

// Definitions for the class
//
// InputFunctionCullData = function with the following decleration
//		DirectX::BoundingSphere function( uint32_t loopindex );

// InputFunctionOutData = function with the following decleration
//		any function( uint32_t loopindex )

// OutputFunction = function with the following decleration
//		void function( any [from InputFunctionOutData] )

class Cull2DBoxSphereOffset
{
public:
	float3 mOffset;
	DirectX::BoundingBox mBox;
	
protected:
	template <class InputFunctionCullData,class InputFunctionOutData, class OutputFunction> 
	cullNoIf( int nbrItems, InputFunctionCullData ifunc,InputFunctionPassData ipfunc, OutputFunction ofunc ) 
	{
		std::vector<bool> collideItems;
		collideItems.reserve( nbrItems );
		
		for( uint32_t loop = 0; loop < nbrItems; ++loop )
		{
			DirectX::BoundingSphere bs = ifunc(loop);

			// Do math
			collideItems.push_back(  mBox.Intersects( bs ) );
		}

		for( int loop = 0; loop < nbrItems; ++loop )
		{
			if( collideItems[loop] == true )
			{
				ofunc( ipFunc( loop ) );
			}
		}
	}

	template <class InputFunctionCullData,class InputFunctionOutData, class OutputFunction> 
	cullLoopIf( int nbrItems, InputFunctionCullData ifunc,InputFunctionPassData ipfunc, OutputFunction ofunc )
	{
		collideItems.reserve( nbrItems );
		
		for( uint32_t loop = 0; loop < nbrItems; ++loop )
		{
			DirectX::BoundingSphere bs = ifunc(loop);

			// Do math
			if(  mBox.Intersects( bs ) )
			{
				ofunc( ipFunc( loop ) );
			}
		}

	}
		

public:
	template <class InputFunctionCullData,class InputFunctionOutData, class OutputFunction> 
	cull( int nbrItems, InputFunctionCullData ifunc,InputFunctionPassData ipfunc, OutputFunction ofunc )
	{
		cullNoIf( nbrItems, ifunc, ipfunc, ofunc );
	}
};
