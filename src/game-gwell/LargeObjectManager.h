

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

struct Entity {
	Handle GamePosition;
	Handle CollisionData;
	event onCollision;
	event onDeath;
	event onDamage;
};

struct GamePosition {
	Handle renderable;		// 4 byte integer
	Math::float3 pos;		// 3 * 4 byte floats
	Math::float4 rot;		// 4 * 4 byte floats
};							// Total: 32 bytes

struct GamePositionScale : public GamePosition {
	Math::float4 scale;
};

// Updated each frame (associated with renderable)
struct CullData {
	Math::Matrix data;
	float sphere;
	float * boxes;
};

struct CollisionData {
	Handle entity;
	Handle GamePosition;
	uint32_t flags;
	uint32_t teams;
	float sphere;		// Creates a full sphere
	float EmptySphere;	// Creates a ring
	float * boxes;		// Collision bounding boxes
};

struct GravityGenerator {
	float maxGravityRadius;
	float mass;
	Handle gameposition;
};
	
struct Movable {
	Math::float4 velocity;
	Math::float4 accleration;
	Handle gameposition;
};


// Seperate Numbers are things in sequence.
// Same number different letter can happen at the same time

1.  Loop over Movable and update GamePosition
2a. Loop over CollisionData and update Entity and update Movable
2b. Loop over GamePosition and send to Render
3.  loop over RenderItems with CullData and do cull 
4.  Render items to screen


// More advanced items

struct AI {
	Handle Entity;
	Handle enemyEntity;
	State machine data;
	event onCollision;
	event onDeath;
}

struct PlanetMotion {
	Math::float3 centerPoint;
	Math::float3 radius;
}


struct Bullet {
	// Velocity is always constant
	// Accleration is always 0
	Handle OwnerEntity;
	Handle Entity;
	Handle GamePosition;
}

struct Missle {
	// Velocity is always changing
	// Accleration is always changing
	Handle GamePosition;
	Handle TrackingEntity;
	Handle OwnerEntity;
	Handle Entity;
	// How to update a missle
	{
		1. Get current position;
		2. Get enemy position;
		3. calculat orientation difference;
		4. Turn by max amount;
		5. develop accleration --> Movable;
	}
};

struct Explosion {
	Handle GamePosition;
	float ringSize;
	float forcePush;
	float initalDamage;
	float attenuation;
	// How to update explosion
	{
		1. Expand ringSize
		2. If initalDamage - attenuation = 0 end explosion
		3. Update collision data
		4. If collision happens store entity and ignore it for the next force push
	}
}

struct BlackHole {
	Handle Entity;
	Handle GravityGenerator;
	Handle OwnerEntity;
	float TimeToEnd;
}


}