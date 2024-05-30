//#include "../stdafx.h"
#include <vector>
#include <span>
#include <functional>
#include <optional>
#include "Game.h"
//#include "Entity.h"
//#include "LinearMotion.h"

#include <algorithm>
#include "../engine/Visuals.h"
#include "../procedural/Geometry.h"
#include "../engine/Event.h"
#include "../engine/GameEvent.h"
#include "../engine/Camera.h"
#include "../game-gwell/crossfire.h"


const unsigned short VK_LEFT = 0x25;
const unsigned short VK_UP = 0x26;
const unsigned short VK_RIGHT = 0x27;
const unsigned short VK_DOWN = 0x28;


namespace Game {

	Engine::Visuals::Basic::Visual box_visual;
	float rotAngle = 0.f;
	Engine::Camera	cam;

	crossfire::Entity theOne; 

	struct CrossFireVisual {
		Engine::Visuals::Basic::Visual visualId{};
		float x = 0;
		float y = 0;
	};
	
	std::vector<CrossFireVisual> frame1;
		
	void onKeyPress(Engine::KeyEvent evt) {
		if (!evt.isKeyUp) return;
		
		switch (evt.vkCode) {
		case VK_RIGHT:
			crossfire::linear::changeHeading(theOne, crossfire::Heading::Right);
			break;
		case VK_LEFT:
			crossfire::linear::changeHeading(theOne, crossfire::Heading::Left);
			break;
		case VK_UP:
			crossfire::linear::changeHeading(theOne, crossfire::Heading::Up);
			break;
		case VK_DOWN:
			crossfire::linear::changeHeading(theOne, crossfire::Heading::Down);
			break;

		}
	}

	void Initalize() {
		Events::Event<Engine::NextLogicFrame>::Listen(&onLogicEvent);
		Events::Event<Engine::NextRenderFrame>::Listen(&onRenderEvent);
		Events::Event<Engine::GameInitalizeData>::Listen(&onGameInitalizeEvent);
		Events::Event<Engine::KeyEvent>::Listen(&onKeyPress);
		
		theOne = crossfire::entities::create();
		crossfire::linear::create(theOne, crossfire::Heading::Right, 0, 0); 
	}


	void onLogicEvent(const Engine::NextLogicFrame& frame) {
		auto values = crossfire::linear::run(1.0f);
		frame1.clear();
		frame1.reserve(values.size()); 
		for (int i = 0; i < values.size(); ++i) {
			frame1.push_back(
				{
					box_visual,
					static_cast<float>(values.positions[i].x)/100,
					static_cast<float>(values.positions[i].y)/100
				}
			);
		};
	}

	void onRenderEvent(const Engine::NextRenderFrame & frame) {
		namespace vs = Engine::Visuals::Basic;
		namespace math = Engine::Math; 
		vs::RenderState state{ cam };

		for (auto const& renderable : frame1) {
			Engine::Matrix world;
			Engine::fMatrix worldf;
			//Engine::fVector4 rotVector = math::XMVectorSet(0, 1, 1, 0);

			//rotAngle += 0.01;

			//worldf = math::XMMatrixRotationAxis(rotVector, Engine::Math::XMConvertToRadians(rotAngle));
			worldf = math::XMMatrixTranslation(renderable.x, renderable.y, 0);
			math::XMStoreFloat4x4(&world, worldf);

			state.states.push_back({ world, box_visual });
		}
		
		state.GlobalAmbientLight = { 0.1,0.1,0.1,1 };
		state.lights.push_back( Engine::Lights::AnyLight{ 
			Engine::Lights::Point {
				{1,10,10},
				{ 1,1,1,0},
				0.0001,0.02,0.003	}
			});

		state.lights.push_back(Engine::Lights::AnyLight{
			Engine::Lights::Point {
				{-2,-2,-20},
				{ 0.3,1,1,0},
				0.001,0.002,0.003	}
			});

		state.lights.push_back(Engine::Lights::AnyLight{
			Engine::Lights::Point {
				{0,0,-3},
				{ 0.2,0.1,0.33,0},
				0.001,0.002,0.003	}
			});


		vs::Render(std::move(state));
	}

	void onGameInitalizeEvent(const Engine::GameInitalizeData & data) {
		namespace vs = Engine::Visuals::Basic;
		
		Geometry::VertexCollection vbs;
		Geometry::IndexCollection  ids;
		auto screenInfo = vs::Get_ScreenSize();

		Geometry::ComputeBox(vbs, ids, { .5f,.5f,.5f }, false, false);
		//Geometry::ComputeTeapot(vbs, ids, 2, 4, false);

		Engine::MeshView mesh_view{ Engine::make_meshview( vbs,ids ) };
		Engine::Material mat{
			{0,0,0,0},
			{0.9,0.9,0.9,1},
			{0.9,0.9,0.9,1},
			{0.9,1,0.9,1},
			128 };

		box_visual = vs::Create(mesh_view, mat);

		cam.setPerspectiveFOV(Engine::Math::XMConvertToRadians(45), screenInfo.aspect_ratio(), .1f, 500.f);
		cam.lookAt({ 0,0,10,0 }, { 0,0,0,0 }, { 0,1,0,0 });
	}
}