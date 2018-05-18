
#pragma once

#include "Coord.h"


namespace Game {


	void onUpdate();

	void onRender();
}

namespace GameUtil {
	using CoordVector = std::vector<Game::Coord>;
}