#include <vector>
#include <span>
#include <array>
#include <optional>
#include <ranges>
#include <algorithm>
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

	void changeHeading(Entity id, Heading heading, Velocity vel) {
		// Happens immedietly should wait to the end of the current run
		for (int i = 0; i < nbrEntitiesAndPositions; ++i) {
			if (entityids[i] == id) {
				
				auto& pos = positions[i];

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
		
		/*pos.x += delta.x;
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
		auto xIntersection = std::div(x, corridorWidth *2);
		auto yIntersection = std::div(y, corridorWidth *2);
		
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

		okToChange = between(xIntersection.rem, remainder - fudgeFactor, remainder + fudgeFactor) && between(yIntersection.rem, remainder - fudgeFactor, remainder+fudgeFactor);
		
		//okToChange = xIntersection.rem == remainder && yIntersection.rem == remainder; 

		if(okToChange)
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

