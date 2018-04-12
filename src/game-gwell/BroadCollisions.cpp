#include "../stdafx.h"
#include "BroadCollisions.h"
#include "Position.h"


namespace BroadCollisions {
	struct partialabb {
		Game::Coord width, height;
	};
	struct fullabb {
		Game::Coord x1, y1, x2, y2;
	};

	//Sorted by ids
	Entity::Vector ids;
	std::vector<partialabb> pabbs;
	std::vector<uint32_t> teams;

	std::vector<fullabb> cache;

	bool overlaps(const fullabb & lhs, const fullabb & rhs) {
		return  (lhs.x  < rhs.x2)	&&
				(lhs.x2 > rhs.x )	&&
				(lhs.y  < rhs.y2)	&&
				(lhs.y2 > rhs.y );
	};

	void update() {
		auto changed{ Positions::getChanged().get() };
		std::sort(changed.begin(), changed.end());
		auto updatedPositions = Positions::get(Temp<decltype(changed), Sorted>{changed});


	}
}

1,1 4,4
2,2 3,3
