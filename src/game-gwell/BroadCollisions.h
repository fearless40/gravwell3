#pragma once

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

	void update();

	Temp<const Collisions> getCollisions();

	void frameEnd();
}