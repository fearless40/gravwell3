#pragma once

#include "Game.h"
#include "Entity.h"
#include "../util/Temp.hpp"

namespace Position {
	struct Pos {
		Game::Coord x, y;
		EntityID id;
	};

	static bool operator < (const Pos & lhs, const Pos & rhs) { return lhs.id < rhs.id; }
	static bool operator == (const Pos & lhs, const Pos & rhs) { return lhs.id == rhs.id; }
	static bool operator <(const EntityID lhs, const Pos & rhs) { 	return lhs < rhs.id; }
	static bool operator <(const Pos & lhs, const EntityID rhs) { 	return lhs.id < rhs; }
	static bool operator == (const EntityID lhs, const Pos & rhs) { return lhs == rhs.id; 	}
	static bool operator == (const Pos & lhs, const EntityID rhs) { return lhs.id == rhs; }

	using Positions = std::vector<Pos>;
	
	Temp<Positions> get(Temp<const Entity::Vector,Sorted> values); 
	Temp<Positions> get(Temp<const Entity::Vector> values);

	Temp<Positions> getEmpty();
	Temp<Positions> getAll();

	void set(Temp<Positions>  positions);

	Temp<const Entity::Vector> getChanged();

	static void frameEnd() {}
}