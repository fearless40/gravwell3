#pragma once
#include "../util/Temp.hpp"


namespace BroadCollisions {
	struct CInfo {
		Game::Coord width, height;
		EntityID id;
		uint32_t team;
	};

	struct Collision {
		Collision(EntityID _id1, EntityID _id2) : id1(_id1), id2(_id2) {}
		EntityID id1;
		EntityID id2;
	};

	using Collisions = std::vector<Collision>;
	using CInfos = std::vector<CInfo>;

	Temp<CInfos> getWriter();
	void setWriter(Temp<CInfos> writer);

	//Depends on positions component
	void update(Temp<Entity::Vector, Sorted> changed_pos);

	Temp<const Collisions> getCollisions();

	void frameEnd();
}