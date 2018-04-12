
#include "../stdafx.h"
#include <vector>
#include <algorithm>
#include <mutex>
#include "Game.h"
#include "Coord.h"
#include "Entity.h"
#include "LinearMotion.h"

namespace LinearMotion {
	std::vector<Game::Coord> xPos;
	std::vector<Game::Coord> yPos;
	std::vector<Game::Coord> xChange;
	std::vector<Game::Coord> yChange;
	std::vector<EntityID> entities;

	using LMVector = std::vector<LinearMotionItem>;

	std::mutex Locker;
	std::vector<LMVector> addItems;
	std::size_t mItems_NumberToAdd;

	
	void process_x() {
		auto xStart = std::begin(xPos);
		for (auto deltaX : xChange) {
			*xStart += deltaX;
			++xStart;
		}
	}

	void process_y() {
		auto yStart = std::begin(yPos);
		for (auto deltaY : yChange) {
			*yStart += deltaY;
			++yStart;
		}
	}

	void process_new() {
		if (mItems_NumberToAdd > entities.capacity() - entities.size())
		{
			std::size_t ReserveSize = mItems_NumberToAdd + (entities.capacity() - entities.size());
			//Calculate the total extra size needed
			xPos.reserve(ReserveSize);
			yPos.reserve(ReserveSize);
			xChange.reserve(ReserveSize);
			yChange.reserve(ReserveSize);
			entities.reserve(ReserveSize);
		}

		auto pos = std::distance(entities.begin(), entities.end());
		for (auto & vec : addItems) {
			for (auto & item : vec) {
				entities[pos] = item.id;
				xPos[pos] = item.x;
				yPos[pos] = item.y;
				xChange[pos] = item.xSpeed;
				yChange[pos] = item.ySpeed;
				++pos;
			}
		}

		mItems_NumberToAdd = 0;
		addItems.clear();
	}
	
	void update() {
		std::lock_guard<std::mutex> lock{ Locker };
		process_x();
		process_y();
		process_new();
	}


	void remove(const std::vector<EntityID> * itemsToRemove) {
		// Pre-condition itemsToRemove is sorted by EntityID
		std::lock_guard<std::mutex> lock{ Locker };
		EntityUtil::remove(*itemsToRemove, entities, xPos, yPos, xChange, yChange);
	}
		

	void create(std::vector<LinearMotionItem> && items) {
		std::lock_guard<std::mutex> lock{ Locker };
		addItems.push_back(items);
		mItems_NumberToAdd += items.size();
	}

	
}