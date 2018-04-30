#include "../stdafx.h"
#include "BroadCollisions.h"
#include "Position.h"
#include <algorithm>
#include "../util/VectorCache.hpp"


namespace BroadCollisions {
	struct partialabb {
		Game::Coord width, height;
	};
	struct fullabb {
		Game::Coord x1, y1, x2, y2;
	};

	VectorCache<CInfo> cInfoCache;
		
	//Unsorted
	Entity::Vector ids;
	std::vector<partialabb> pabbs;
	std::vector<uint32_t> teams;
	std::vector<fullabb> cache;
	Collisions colCache;

	Temp<CInfos> getWriter() {
		return { cInfoCache.get() };
	}

	void setWriter(Temp<CInfos> writer) {
		for (const auto & x : writer.get()) {
			ids.push_back(x.id);
			pabbs.emplace_back(x.width, x.height);
			teams.push_back(x.team);
			cache.emplace_back(0, 0, 0, 0);
		}
		cInfoCache.recycle(writer.get());
	}

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

	void update(Temp<const Entity::Vector, Sorted> changed_pos) {
		auto updatedPositions = Position::get(changed_pos).get();
		
		//Regenerate cache
		assert(changed_pos.get().size() == updatedPositions.size()); //The two must be of the same size

		update_cache(changed_pos, updatedPositions);

		colCache.clear();

		for (int i = 0; i < teams.size()-1; ++i) {
			auto firstVal = teams[i];
			auto first_aabb = cache[i];
			for (int k = i + 1; k < teams.size(); ++k) {
				if (firstVal != teams[k]) {
					if (overlaps(first_aabb, cache[k])) {
						colCache.emplace_back(ids[i], ids[k]);
					}
				}
			}
		}
	}

	Temp<const Collisions> getCollisions() {
		return { colCache };
	}
}

