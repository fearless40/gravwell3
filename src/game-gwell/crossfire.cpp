#include <vector>
#include <span>
#include <array>
#include <optional>
#include <ranges>
#include <algorithm>
#include "crossfire.h"

namespace crossfire::entities {
	std::vector<Entity> entities;
	constinit Entity nextId = 0; 
	
	Entity create() {
		const auto id = ++nextId;
		entities.push_back(id);
		return id; 
	}

	void remove(Entity id) {
		auto iter = std::ranges::find(entities, id);
		if (iter != std::end(entities))
		{
			*iter = entities[entities.size()];
			entities.pop_back();
		}
	}
}

namespace crossfire::linear {
	std::array<Entity, 128> entityids;
	std::array<CurrentPosition, 128> positions; 
	std::size_t nbrEntitiesAndPositions = 0;

	std::array<Entity, 64> flaggedForRemoval;
	std::size_t nbrFlaggedForRemoval = 0; 

	struct DeltaXY {
		int x;
		int y;
	};

	const DeltaXY headingDelta[] = {
		{0,1}, {0,-1},
		{1,0}, {-1,0},
		{0,0}
	};
	void changeHeading(Entity id, Heading heading) {
		// Happens immedietly should wait to the end of the current run
		for (int i = 0; i < nbrEntitiesAndPositions; ++i) {
			if (entityids[i] == id) {
				positions[i].heading = heading;
			}
		}
	}

	void create(Entity id, Heading heading, Coordinate x, Coordinate y) {
		entityids[nbrEntitiesAndPositions] = id;
		positions[nbrEntitiesAndPositions] = { heading, x, y };
		++nbrEntitiesAndPositions;
	}

	void remove(Entity id) {
		flaggedForRemoval[nbrFlaggedForRemoval] = id;
		++nbrFlaggedForRemoval;
	}
	
	EntityAndData run(float delta) {

		auto update_positions = [](auto& pos) {
			auto delta = headingDelta[static_cast<std::size_t>(pos.heading)];
			pos.x += delta.x;
			pos.y += delta.y;
		};

		std::ranges::for_each_n(positions.begin(), nbrEntitiesAndPositions, update_positions);

		return {
			{entityids.cbegin(), nbrEntitiesAndPositions},
			{positions.cbegin(), nbrEntitiesAndPositions}
		};
	}

	void endFrame() {

	}

}