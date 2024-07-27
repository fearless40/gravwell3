#pragma once


namespace crossfire {

	
	using Entity = unsigned int;

	const Entity INVALID_ENTITY = 0 - 1; 
	
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

	enum class Velocity : unsigned int {
		Normal = 0,
		ThreeQuarter = 1,
		Half = 2,
		Quarter = 3,
		Double = 4,
		Triple = 5,
		Quadruple = 6
	};

	namespace entities {
		Entity create();
		void remove(Entity id); 
	}

	namespace linear {

		struct EntityAndData {
			const std::span<const Entity, std::dynamic_extent> entities;
			const std::span<const CurrentPosition, std::dynamic_extent> positions;

			const auto size() const { return entities.size(); };
		};
				
		void changeHeading(Entity id, Heading heading, Velocity vel = Velocity::Normal);
		void changePosition(Entity id, Coordinate x, Coordinate y);
		void create(Entity id, Heading heading, Coordinate x, Coordinate y, Velocity vel = Velocity::Normal);
		void remove(Entity id);
	
		void run(float delta);
		EntityAndData getEntitiesPositions();

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

	namespace actions {
		enum class Actions {
			no_action = 0,
			turn_right,
			turn_left,
			turn_up,
			turn_down,
			fire_missle,
			fire_special
		};

		void do_action(Entity id, Actions action); 

		void turn_right(Entity id);
		 void turn_left(Entity id);
		 void turn_up(Entity id);
		 void turn_down(Entity id);
		 void fire_missle(Entity id);
		 void fire_special(Entity id); 
	}

	namespace keyboardinput {

		const std::size_t MAX_LOCAL_PLAYERS{ 4 };
		struct KeyBoardMapping {};
		// Consolidate Input per Entity

		void intialize();
		void on_logic_tick();

		void create(Entity id, const KeyBoardMapping & map);
		void remove(Entity id);

		void on_turn_right(Entity id);
		void on_turn_left(Entity id);
		void on_turn_up(Entity id);
		void on_turn_down(Entity id);
		void on_fire_missle(Entity id);
		void on_fire_special(Entity id);
		
	}

	namespace missle {
		void create(Entity id, Entity owner, unsigned int speed);
		void remove(Entity id);

		void run(float delta); 
	}

	namespace collider {
		struct Collision {
			Entity id1;
			Entity id2;
			std::size_t collisionId; 
		};
		
		
		void do_collisions(const linear::EntityAndData& data);
		std::span<const Collision> get_collisions();

		
		void add_static_collider(Coordinate x, Coordinate y, Coordinate x2, Coordinate y2);

	}

	namespace map {

		struct path {
			unsigned int x;
			unsigned int y;
			unsigned int x2;
			unsigned int y2;
		};

		struct ValidateHeadingChange {
			bool canChangeHeading{ false };
			Coordinate newX{ 0 };
			Coordinate newY{ 0 };

			operator bool() {
				return canChangeHeading; 
			}
		};

		constexpr const int fudgeFactor{ 3 };
		constexpr const int nbrRows{ 8 };
		constexpr const int nbrCols{ 8 };
		constexpr const int corridorWidth{ 10 };
		constexpr const int spaceBetween = corridorWidth * 2;
		constexpr const int startingX{ 0 };
		constexpr const int startingY{ 0 };

		std::array<path, nbrRows+nbrCols> getPaths();
		path getRowPath(unsigned int x = 0, unsigned int y = 0);
		path getColPath(unsigned int x = 0, unsigned int y = 0);
		constexpr const path getMapExtents() {
			return {
				0,
				0,
				((nbrRows - 1) * 2 * corridorWidth + corridorWidth),
				((nbrCols - 1) * 2 * corridorWidth + corridorWidth)
			};
		}

		ValidateHeadingChange isValidIntersectionToChangeHeading(Heading current, Coordinate x, Coordinate y);


		
	}

	

	


}