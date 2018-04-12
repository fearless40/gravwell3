#pragma once

#include "Game.h"
#include "Entity.h"
#include "Temp.hpp"

namespace Position {
	struct Pos {
		Game::Coord x, y;
		EntityID id;
	};

	bool operator < (const Pos & lhs, const Pos & rhs) { return lhs.id < rhs.id; }
	bool operator == (const Pos & lhs, const Pos & rhs) { return lhs.id == rhs.id; }
	bool operator <(const EntityID lhs, const Pos & rhs) { 	return lhs < rhs.id; }
	bool operator <(const Pos & lhs, const EntityID rhs) { 	return lhs.id < rhs; }
	bool operator == (const EntityID lhs, const Pos & rhs) { return lhs == rhs.id; 	}
	bool operator == (const Pos & lhs, const EntityID rhs) { return lhs.id == rhs; }

	using Positions = std::vector<Pos>;
	
	Temp<const Positions> get(Temp<Entity::Vector,Sorted> values); 

	Temp<Positions> getWriter();

	void setWriter(Temp<Positions>  positions);

	Temp<const Entity::Vector> getChanged();

	void frameEnd() {}
}