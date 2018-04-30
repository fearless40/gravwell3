#pragma once
#include "../util/Temp.hpp"
#include "Entity.h"

namespace BroadCollisions {
	struct CInfo {
		Game::Coord width, height;
		EntityID id;
		uint32_t team;
	};

	struct Collision {
		EntityID id1;
		EntityID id2;
	};

	using Collisions = std::vector<Collisions>;
	using CInfos = std::vector<CInfo>;

	Temp<CInfos> getWriter();
	void setWriter(Temp<CInfos> writer);

	//Depends on positions component
	void update(Temp<Entity::Vector, Sorted> changed_pos);

	Temp<const Collisions> getCollisions();

	void frameEnd();
}