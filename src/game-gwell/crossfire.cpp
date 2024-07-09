#include <vector>
#include <span>
#include <array>
#include <optional>
#include <ranges>
#include <algorithm>
#include "../engine/GameEvent.h"
#include "../util/Math/RectT.h"
#include "../util/FixedFunctionFloat.h"
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

	using coord = util::FixedFunctionFloat<4>; 

	struct Position {
		Heading heading;
		coord x;
		coord y;
		coord velocity; 

		constexpr auto getX() const { return x.as<Coordinate>(); }
		constexpr auto getY() const { return y.as<Coordinate>(); }
		
	};

	std::array<Entity, 128> entityids;
	std::array<Position, 128> positions;
	std::array<CurrentPosition, 128> visiblePositions; 
	std::size_t nbrEntitiesAndPositions = 0;

	std::array<Entity, 64> flaggedForRemoval;
	std::size_t nbrFlaggedForRemoval = 0;

	struct DeltaXY {
		util::FixedFunctionMagnitude x;
		util::FixedFunctionMagnitude y;
	};

	const DeltaXY headingDelta[] = {
		{0,-1}, {0,1},
		{-1,0}, {1,0},
		{0,0}
	};

	const coord Velocities[] = {
		coord{1},
		coord{0.75f},
		coord{0.5f},
		coord{0.25f},
		coord{2},
		coord{3},
		coord{4}
	};

	constexpr auto asHeadingDelta(Heading head) {
		return headingDelta[static_cast<unsigned int>(head)];
	}

	constexpr auto asSpeed(Velocity vel) {
		return Velocities[static_cast<unsigned int>(vel)];
	}

	std::optional<int> find_first(Entity id) {
		for (int i = 0; i < nbrEntitiesAndPositions; ++i) {
			if (entityids[i] == id) return i;
		}
		return {};
	}

	void changePosition(Entity id, Coordinate x, Coordinate y) {
		auto found = find_first(id);
		if (found) {
			auto& pos = positions[found.value()];
			pos.x = x;
			pos.y = y;
		}
	}

	void changeHeading(Entity id, Heading heading, Velocity vel) {
		// Happens immedietly should wait to the end of the current run
		auto found = find_first(id);
		if(found) {
			auto& pos = positions[found.value()];

			if (pos.heading == Heading::Up || pos.heading == Heading::Down) {
				if (heading == Heading::Up || heading == Heading::Down)
					return;
			}

			if (pos.heading == Heading::Left || pos.heading == Heading::Right) {
				if (heading == Heading::Left || heading == Heading::Right)
					return;
			}


			auto valid = map::isValidIntersectionToChangeHeading(pos.heading, pos.getX() + 5, pos.getY() + 5);
			if (valid) {
				pos.heading = heading;
				pos.x = coord{ valid.newX };
				pos.y = coord{ valid.newY };
				//pos.velocity = asSpeed(vel);
			}
		}
	}

	void create(Entity id, Heading heading, Coordinate x, Coordinate y, Velocity vel ) {
		entityids[nbrEntitiesAndPositions] = id;
		positions[nbrEntitiesAndPositions] = { heading, coord{x}, coord{y}, asSpeed(vel)};
		++nbrEntitiesAndPositions;
	}

void remove(Entity id) {
	flaggedForRemoval[nbrFlaggedForRemoval] = id;
	++nbrFlaggedForRemoval;
}

void run(float delta) {

	auto update_positions = [](auto& pos) {
		const auto extents{ map::getMapExtents() };
		auto delta = asHeadingDelta(pos.heading);
		pos.x = pos.x + (pos.velocity * delta.x);
		pos.y = pos.y + (pos.velocity * delta.y);
		
		/*pos.x += delta.x;\
		pos.y += delta.y;*/

		auto testx = pos.getX();
		auto testy = pos.getY(); 

		if (pos.getX() + 10 > extents.x2) pos.x = coord{extents.x2 - 10};
		if (pos.getX() < extents.x) pos.x = coord{ extents.x };
		if (pos.getY() + 10 > extents.y2) pos.y = coord{extents.y2 - 10};
		if (pos.getY() < extents.y) pos.y = coord{extents.y};
	};

	std::ranges::for_each_n(positions.begin(), nbrEntitiesAndPositions, update_positions);

	/*return {
		{entityids.cbegin(), nbrEntitiesAndPositions},
		{positions.cbegin(), nbrEntitiesAndPositions}
	};*/
}

EntityAndData getEntitiesPositions() {
	
	std::transform(positions.cbegin(), positions.cend(), visiblePositions.begin(), [](const auto& input) { return CurrentPosition{ input.heading, input.getX(), input.getY()}; });
	
	
	return {
		{entityids.cbegin(), nbrEntitiesAndPositions},
		{visiblePositions.cbegin(), nbrEntitiesAndPositions}
	};
}


void endFrame() {

}
}

namespace crossfire::map {





	path getRowPath(unsigned int x, unsigned int y) {
		return {
			x,
			y,
			x + ((nbrRows - 1) * 2 * corridorWidth + corridorWidth),
			y + corridorWidth
		};
	}


	path getColPath(unsigned int x, unsigned int y) {
		return {
			x,
y,
corridorWidth + x,
y + ((nbrCols - 1) * 2 * corridorWidth + corridorWidth)
		};
	}



	std::array<path, nbrRows + nbrCols> getPaths() {
		std::array<path, nbrRows + nbrCols> paths;

		unsigned count = 0;
		for (unsigned int row = 0; row < nbrRows; ++row) {

			paths[row] = getRowPath(0, count * corridorWidth);

			/* {
				0,
				count* corridorWidth,
				(nbrRows) * 2 * corridorWidth, //+ corridorWidth,
				count* corridorWidth //+ corridorWidth
			};*/
			count += 2;
		};

		count = 0;
		for (unsigned int col = 0; col < nbrCols; ++col) {
			paths[col + nbrRows] = getColPath(count * corridorWidth, 0);
			/*{
				count* corridorWidth,
				0,
				count* corridorWidth,// + corridorWidth,
				(nbrCols) * 2 * corridorWidth //+ corridorWidth
			};*/
			count += 2;
		}

		return paths;
	}

	template<class T, class Compare = std::less_equal<> >
	bool between(T const& val, T const& lo, T const& hi, Compare comp = Compare{})
	{
		return comp(lo, val) && comp(val, hi);
	}

	ValidateHeadingChange isValidIntersectionToChangeHeading(Heading current, Coordinate x, Coordinate y) {
		auto xIntersection = std::div(x, corridorWidth * 2);
		auto yIntersection = std::div(y, corridorWidth * 2);

		// Find the remainder that is acceptable
		const int remainder = (corridorWidth / 4) % corridorWidth;


		bool okToChange = false;

		/*switch (current) {
		case Heading::Up:
			okToChange = (yIntersection.rem - fudgeFactor) < 0 && (xIntersection.rem < fudgeFactor || xIntersection.rem > fudgeFactor);
			break;
		case Heading::Down:
			okToChange = (yIntersection.rem - fudgeFactor) > 0 && (xIntersection.rem < fudgeFactor || xIntersection.rem > fudgeFactor);
			break;
		case Heading::Left:
			okToChange = (xIntersection.rem - fudgeFactor) < 0 && (yIntersection.rem < fudgeFactor || yIntersection.rem > fudgeFactor);
			break;
		case Heading::Right:
			okToChange = (xIntersection.rem - fudgeFactor) && (yIntersection.rem < fudgeFactor || yIntersection.rem > fudgeFactor);
			break;
		case Heading::Stopped:
			return {};
		}*/

		okToChange = between(xIntersection.rem, remainder - fudgeFactor, remainder + fudgeFactor) && between(yIntersection.rem, remainder - fudgeFactor, remainder + fudgeFactor);

		//okToChange = xIntersection.rem == remainder && yIntersection.rem == remainder; 

		if (okToChange)
		{
			return {
				true,
				xIntersection.quot * corridorWidth * 2,
				yIntersection.quot * corridorWidth * 2
			};
		}

		return {};
	}

}

namespace crossfire::collider {
	const std::size_t MAX_COLLISIONS{ 255 };
	const int ITEM_WIDTH = crossfire::map::corridorWidth;

	std::array<Collision, MAX_COLLISIONS> lastCollisions;
	std::size_t nbrCollisions;

	bool aabb(const CurrentPosition& pos1, const CurrentPosition& pos2) noexcept {
		if (  ((pos1.x >= pos2.x && pos1.x <= pos2.x + ITEM_WIDTH) || (pos1.x + ITEM_WIDTH >= pos2.x && pos1.x + ITEM_WIDTH <= pos2.x + ITEM_WIDTH))
			&& ((pos1.y >= pos2.y && pos1.y <= pos2.y + ITEM_WIDTH) || (pos1.y + ITEM_WIDTH >= pos2.y && pos1.y + ITEM_WIDTH <= pos2.y + ITEM_WIDTH))
		   ) {
			return true; 
		}
		return false; 
	}
	
	void doCollisions(const linear::EntityAndData& data) {
		nbrCollisions = 0;
		if (data.size() == 1) return;

		for (std::size_t outerloop = 0; outerloop < data.size()-1; ++outerloop) 
		{
			for (std::size_t innerloop = outerloop + 1; innerloop < data.size(); ++innerloop)
			{
				if (aabb(data.positions[outerloop], data.positions[innerloop])) {
					lastCollisions[nbrCollisions] = { data.entities[outerloop], data.entities[innerloop], nbrCollisions };
					++nbrCollisions; 
				}
			}
		}
	}

	std::span<const Collision> getCollisions() {
		return { lastCollisions.data(), nbrCollisions};
	}

}

namespace crossfire::keyboardinput {
	struct EntityIdToMap {
		Entity id {crossfire::INVALID_ENTITY};
		Actions moveAction; 
		Actions fireAction; 
	};

	struct KeyMapping {
		int key;
		Actions action;
		Entity id; 
	};
	
	std::array<EntityIdToMap, MAX_LOCAL_PLAYERS> players_input; 
	std::size_t nbrPlayers{ 0 };

	std::optional<EntityIdToMap&> find_player(Entity id) {
		for (std::size_t i = 0; i < players_input.size(); ++i) {
			if (players_input[i].id == id) return players_input[i];
		}
		return {};
	}

	std::vector<KeyMapping> keys; 

	void mapKeysToEntity(Entity id, const KeyToActionMapper& map) {
		for (const auto& item : map) {
			keys.emplace_back(item.key, item.action, id);
		}
	}

	void create(Entity id, const KeyToActionMapper & map) {
		auto playerSearch = find_player(id);
		if (playerSearch) {
			// Todo: overwrite the key mappings
			return;
		}
		else {
			if (nbrPlayers > MAX_LOCAL_PLAYERS) return;
			players_input[nbrPlayers].id = id;
			mapKeysToEntity(id, map);
			++nbrPlayers;
		}
	}
	void remove(Entity id) {
		// Do nothing
	}

	void inject_keys(Engine::KeyEvent key_event) {

	}

}

