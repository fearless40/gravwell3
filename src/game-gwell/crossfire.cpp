#include <vector>
#include <span>
#include <array>
#include <optional>
#include <ranges>
#include <algorithm>
#include "../util/Math/RectT.h"
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
		{0,-1}, {0,1},
		{-1,0}, {1,0},
		{0,0}
	};

	void changeHeading(Entity id, Heading heading) {
		// Happens immedietly should wait to the end of the current run
		for (int i = 0; i < nbrEntitiesAndPositions; ++i) {
			if (entityids[i] == id) {
				
				const auto& pos = positions[i];

				if (pos.heading == Heading::Up || pos.heading == Heading::Down) {
					if (heading == Heading::Up || heading == Heading::Down)
						return;
				}

				if (pos.heading == Heading::Left || pos.heading == Heading::Right) {
					if (heading == Heading::Left || heading == Heading::Right)
						return;
				}


				auto valid = map::isValidIntersectionToChangeHeading(positions[i].heading, positions[i].x + 5, positions[i].y + 5);
				if (valid) {
					positions[i].heading = heading;
					positions[i].x = valid.newX;
					positions[i].y = valid.newY;
				}
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
		const auto extents{ map::getMapExtents() };
		auto delta = headingDelta[static_cast<std::size_t>(pos.heading)];
		pos.x += delta.x;
		pos.y += delta.y;

		if (pos.x + 10 > extents.x2) pos.x = extents.x2 - 10;
		if (pos.x < extents.x) pos.x = extents.x;
		if (pos.y + 10 > extents.y2) pos.y = extents.y2 - 10;
		if (pos.y < extents.y) pos.y = extents.y;
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

