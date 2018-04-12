#pragma once


namespace LinearMotion {

	struct LinearMotionItem {
		EntityID id;
		Game::Coord x = 0, y = 0;
		Game::Coord xSpeed = 0, ySpeed = 0;
	};



	void process_x();
	void process_y();
	void process_new();
	void update();


	template <typename CallBack>
	void update(CallBack cb) {
		update();
		cb();
	}


	// Items must be sorted by EntityID
	void remove(const std::vector<EntityID> &);

	// Locks the class
	void create(const std::vector<LinearMotionItem> && items);
}