#pragma once



namespace subfire {

	enum class Heading : unsigned int
	{
		Up,
		Down,
		Left, 
		Right
	};

	using Coordinate = int;

	struct CurrentPosition {
		Heading heading;
		Coordinate x;
		Coordinate y;
	};

	namespace playercomponent{
		void doUpdate(float frameDelta); 
	}






}