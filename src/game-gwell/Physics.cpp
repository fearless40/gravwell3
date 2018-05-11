#include "../stdafx.h"
#include "Physics.h"
#include <unordered_map>
#include <optional>
#include <algorithm>
#include "Position.h"
#include "../util/Temp.hpp"

namespace Physics {


	using fVector = std::vector<float>;

	struct PerFrame {
		
		std::unordered_map<EntityID, unsigned int> ids;

		fVector xForce;
		fVector yForce;
		fVector mass;
		fVector xAcc;
		fVector yAcc;

		void clear() {
			ids.clear();
			xForce.clear();
			yForce.clear();
			xAcc.clear();
			yAcc.clear();
			
		}

		void addForce(EntityID id, float xF, float yF) {
			if (ids.count(id) == 0) {
				xForce.push_back(xF);
				yForce.push_back(yF);

				xAcc.push_back(0.f);
				yAcc.push_back(0.f);

				if (auto imass = game.mass.find(id); imass != game.mass.end()) {
					mass.push_back(imass->second);
				}
				else {
					mass.push_back(1.f);
				}
			}
			else {
				auto pos = ids.find(id)->second;
				xForce[pos] += xF;
				yForce[pos] += yF;
			}
		}
	};

	struct PerGame {
		std::unordered_map<EntityID, float> mass;
		std::unordered_map<EntityID, unsigned int> entityMap;

		Entity::Vector ids;
		fVector xVel;
		fVector yVel;

		std::optional<unsigned int> hasId(EntityID id) {
			auto temp = entityMap.find(id);
			if (temp != entityMap.end()) {
				return { temp->second };
			}
			return {};
		}
	};

	PerGame game;
	PerFrame frame;

	//Data access
	/*
		Get forces foreach entity. Apply each force according to the mass of the entity. Or assume mass is always 1
			Read xForce, yForce, for ID
				if( ID == new.ID) {
					ID.xForce += new.xForce
					ID.yForce += new.yForce
				}
			Write xForce, yForce
		Turn forces into acclerations
			Read xForce, yForce, mass, for ID
				xAcc = ID.xForce / ID.mass
				yAcc = ID.yForce / ID.mass
			Write xAcc, yAcc
			Optional turn too much accleration into damage:
				Read xAcc, yAcc, EntityID
				Out EntityID damage_acc
		Turn acclerations into velocities
			Read xAcc, yAcc, xVel, yVel for ID
				ID.xVel += ID.xAcc * timeStep;
				ID.yVel += ID.yAcc * timeStep;
			Write xVel, yVel
		Process Universal stuff
			Optional max veloctity: Read xVel, yVel -> Write xVel, yVel for All
				if( xVel > UNIVERSAL_X ) xVel = UNIVERSAL_X

			Optional friction: Read xVel, yVel -> Write xVel, yVel for All
		Return Vel
		Force and Acc are not remembered
	
	*/


	void setWriter(Forces & f) {
		for (auto & x : f) {
			frame.addForce(x.id, x.x, x.y);
		}
	}

	void calculate_acc_x() {
		for (std::size_t i = 0; i < frame.xForce.size(); ++i) {
			frame.xAcc[i] = frame.xForce[i] / frame.mass[i];
		}
	}

	void calculate_acc_y() {
		for (std::size_t i = 0; i < frame.yForce.size(); ++i) {
			frame.yAcc[i] = frame.yForce[i] / frame.mass[i];
		}
	}

	void calculate_vel_x(float timeStep) {
		for (auto & x : frame.ids) {
			if (auto val = game.hasId(x.first); val) {
				game.xVel[*val] += frame.xAcc[x.second] * timeStep;
			}
		}
	}

	void calculate_vel_y(float timeStep) {
		for (auto & x : frame.ids) {
			if (auto val = game.hasId(x.first); val) {
				game.yVel[*val] += frame.yAcc[x.second] * timeStep;
			}
		}
	}


	void set_mass(EntityID id, float mass) {
		if (auto pos = game.hasId(id); pos) {
			game.mass[id] = mass;
		} 
		else {
			// Create the entries
			game.ids.push_back(id);
			game.xVel.push_back(0.f);
			game.yVel.push_back(0.f);
			game.entityMap[id] = game.ids.size()-1;
		}
	}

	void update_positions() {
		auto entries = Position::get(Temp( game.ids )).get();
		for (unsigned i = 0; i < entries.size(); ++i) {
			float xPos = entries[i].x.asFloat();
			float yPos = entries[i].y.asFloat();
			xPos += game.xVel[i];
			yPos += game.yVel[i];
			entries[i].x = Game::Coord(xPos);
			entries[i].y = Game::Coord(yPos);
		}

		Position::set(entries);
		
	}
}