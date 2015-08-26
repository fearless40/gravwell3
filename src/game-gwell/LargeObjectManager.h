

#pragma once
#include "../render/RenderableInstance.h"
#include "../util/Math.h"

namespace BasicMM {
	class MicroManager;
};

namespace LargeObjectManager
{
	typedef std::vector< RenderableInstance > RenderableList;

	typedef uint32_t LargeObjectID;
	
	struct GravityGenerator 
	{
		Math::float3	position;
		float		radius;
		float		mass;
		float		maxGravityEffect;
	};

	void renderVisible( BasicMM::MicroManager & manager );

	void addNewGravityBody( Math::float3 pos, float radius, float mass, float maxGravityEffect, const RenderableInstance & ri );
}