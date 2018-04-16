#include "../stdafx.h"
#include "BroadCollisions.h"
#include "Position.h"
#include <algorithm>


namespace BroadCollisions {
	struct partialabb {
		Game::Coord width, height;
	};
	struct fullabb {
		Game::Coord x1, y1, x2, y2;
	};

	//Unsorted
	Entity::Vector ids;
	std::vector<partialabb> pabbs;
	std::vector<uint32_t> teams;

	std::vector<fullabb> cache;

	bool overlaps(const fullabb & lhs, const fullabb & rhs) {
		return  (lhs.x1 < rhs.x2)	&&
				(lhs.x2 > rhs.x1)	&&
				(lhs.y1 < rhs.y2)	&&
				(lhs.y2 > rhs.y1);
	};

	fullabb make_aabb(const Position::Pos & pos, const partialabb & abb) {
		return {
			pos.x, pos.y,
			pos.x + abb.width,
			pos.y + abb.height
		};
	}

	void update_cache(const Entity::Vector & eId, const Position::Positions & pos) {
		auto idsBegin = eId.begin();
		auto idsEnd = eId.end();
		auto posBegin = pos.begin();
		for (const auto x : ids) {
			auto item = std::lower_bound(idsBegin, idsEnd, x );
			if (item != idsBegin && *item == x) {
				auto srcDistance = std::distance(idsBegin, item);
				auto dstDistance = &ids[0] - &x;
				cache[dstDistance] = make_aabb(pos[srcDistance], pabbs[dstDistance]);
			}
		}
	}

	void update() {
		auto changed{ Position::getChanged().get() };
		std::sort(changed.begin(), changed.end());
		auto updatedPositions = Position::get({changed}).get();

		//Regenerate cache
		assert(changed.size() == updatedPositions.size()); //The two must be of the same size

		update_cache(changed, updatedPositions);


	}
}

