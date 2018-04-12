
#pragma once




namespace Game {
	using Coord = __int64;
	using dCoord = double;



	void onUpdate();

	void onRender();
}

namespace GameUtil {
	using CoordVector = std::vector<Game::Coord>;
}