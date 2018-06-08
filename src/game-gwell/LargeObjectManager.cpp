#include "../stdafx.h"

using namespace DirectX;


/*
namespace LargeObjectManager 
{

	std::vector< GravityGenerator > mGravityBodies;
	std::vector< RenderableInstance >		mDisplayVisual;


	void addNewGravityBody( XMFLOAT3 pos, float radius, float mass, float maxGravityEffect, const RenderableInstance & ri )
	{
		GravityGenerator gen = { pos, radius, mass, maxGravityEffect };

		mGravityBodies.push_back( gen );
		mDisplayVisual.push_back( ri );
	}

	
	template< class Micro >
	void renderGravityBodies(Micro & manager)
	{
		for( uint32_t count = 0, size = mDisplayVisual.size(); count < size; ++count )
		{
			
			XMMATRIX trans = XMMatrixTranspose(XMMatrixTranslation( mGravityBodies[count].position.x, mGravityBodies[count].position.y, mGravityBodies[count].position.z ));
			XMFLOAT4X4 dataBuff;
			XMStoreFloat4x4( &dataBuff, trans );

			manager.prep( mDisplayVisual[count], dataBuff );
		}
	}

	void renderVisible( BasicMM::MicroManager & manager )
	{
		renderGravityBodies( manager );
	}

};
*/