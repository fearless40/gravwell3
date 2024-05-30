#pragma once


namespace crossfire {

	
	using Entity = unsigned int;
	
	enum class Heading : unsigned int
	{
		Up = 0,
		Down = 1,
		Left = 2,
		Right = 3,
		Stopped = 4
	};

	using Coordinate = int;

	struct CurrentPosition {
		Heading heading;
		Coordinate x;
		Coordinate y;
	};

	namespace entities {
		Entity create();
		void remove(Entity id); 
	}

	namespace linear {

		struct EntityAndData {
			const std::span<const Entity, std::dynamic_extent> entities;
			const std::span<const CurrentPosition, std::dynamic_extent> positions;

			const auto size() { return entities.size(); };
		};
				
		void changeHeading(Entity id, Heading heading);
		void create(Entity id, Heading heading, Coordinate x, Coordinate y);
		void remove(Entity id);
	
		EntityAndData run(float delta);
		void endFrame();


	
		// onEntityStopped()
	}

	namespace missle_shooter {
		void create(Entity id, unsigned int maxLivingMissles, unsigned int delay);
		void fireMissle(Entity id);

	 	void run(float delta);

		std::optional<bool> canFireMissles(Entity id);
		std::optional<unsigned int> nbrMisslesLeft(Entity id);
	}

	namespace keyboardinput {
		using KeyToActionMapper = void*;
		
		void create(Entity id, KeyToActionMapper map); 
		void remove(Entity id);

		void run(float delta);

	}

	namespace missle {
		void create(Entity id, Entity owner, unsigned int speed);
		void remove(Entity id);

		void run(float delta); 
	}

	namespace collider {
		void create(Entity id, unsigned int width, unsigned int height);
		void run(float delta, std::span<CurrentPosition> positions);

	}

	namespace map {
		void setMap();
		bool canTurn(CurrentPosition pos);
		
		
	}

	

	


}