
namespace Physics {
	namespace twod {

		struct Position {
			uint_64 x, y;
		};

		void setPosition(EntityID id, pos);
		void setCollisionStatus(EntityID id);
		void setMass(EntityID id, float mass);
		void setCenterOfMass(EntityID id, Position center = { 0,0 });
		//void addForce(EntityId id);
		
	}

	namespace 3d{

	}
}