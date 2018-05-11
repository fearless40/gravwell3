#pragma once
#include "Entity.h"

namespace Physics {

	struct Force {
		EntityID id;
		float x, y;
	};

	struct Mass {
		float mass;
	};

	struct SingleAxesForce {
		EntityID id;
		float amount;
	};

	using SingleAxisForces = std::vector<float>;
	using Forces = std::vector<Force>;

	Temp<Forces> getWriter();
	void setWriter(Temp<Forces> f);

	/*
		spaceship.engineThrust --> Physics.add(engine push)
		explosion.find(others)
		physics.add(explosion.attenuation.push per other)
	*/

	void set_mass(EntityID id, float mass);

	//Consolidates all the forces per item 
	void update_velocities();	

	// Applies max speed values to items in the universe
	//void apply_universal_constants();
	
	// Applies a friction amount to all items in the universe
	//void apply_friction();

	// Same as above but in one loop
	//void apply_friction_and_universal();
	
	// Changes the position of items based on the force
	void update_positions();

	void frameEnd(const Entity::Vector & removeEntity);
}