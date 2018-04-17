#include "../stdafx.h"
#include "Position.h"
#include <algorithm>

namespace Position {
	std::vector<Positions> freePositions;
	std::vector<Positions> usedPositions;

	// Hold the actual data
	// Sorted by ID
	Positions positions;
	Entity::Vector changed;

	Positions nextVector() {
		return Positions{};
	}

	Temp<const Positions> get(Temp<const Entity::Vector,Sorted> values) {
		auto ents = values.get();
		auto out = nextVector();
		auto valuesIterator = ents.begin();

		std::copy_if(positions.begin(), positions.end(), out.begin(), [&valuesIterator](auto & x) {
			if (*valuesIterator == x.id) {
				++valuesIterator;
				return true;
			}
			else if (*valuesIterator > x.id) {
				++valuesIterator;
				return false;
			}
			return false;
		});

		return { out };
	}

	Temp<Positions> getWriter() {
		return { nextVector() };
	}

	void setWriter(Temp<Positions>  newPos) {
		auto newValues = newPos.get();
		for (auto & x : newValues) {
			auto value = std::lower_bound(positions.begin(), positions.end(), x);
			if (value != positions.end() && value->id == x.id) {
				*value = x;
				changed.push_back(x.id);
			}
			else {
				positions.push_back(x);
				changed.push_back(x.id);
			}
		}
	}

	Temp<const Entity::Vector> getChanged() {
		return { changed };
	}
}