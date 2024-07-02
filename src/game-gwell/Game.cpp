//#include "../stdafx.h"
#include <vector>
#include <span>
#include <functional>
#include <optional>
#include <array>
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
	Engine::Visuals::Basic::Visual row_visual;
	Engine::Visuals::Basic::Visual col_visual;
	float rotAngle = 0.f;
	Engine::Camera	cam;

	crossfire::Entity theOne; 
	crossfire::Heading desiredHeading; 

	struct CrossFireVisual {
		Engine::Visuals::Basic::Visual visualId{};
		float x = 0;
		float y = 0;

		static constexpr float gameUnitToFloat(unsigned int value) {
			return (static_cast<float>(value)); 
		}
	};
	
	std::vector<CrossFireVisual> dynamic_elements;
	std::vector<CrossFireVisual> static_elements; 
		
	void onKeyPress(Engine::KeyEvent evt) {
		if (!evt.isKeyUp) return;
		
		switch (evt.vkCode) {
		case VK_RIGHT:
			desiredHeading = crossfire::Heading::Right;
			break;
		case VK_LEFT:
			desiredHeading = crossfire::Heading::Left;
			break;
		case VK_UP:
			desiredHeading = crossfire::Heading::Up;
			break;
		case VK_DOWN:
			desiredHeading = crossfire::Heading::Down;
			break;

		}
	}

	void Initalize() {
		Events::Event<Engine::NextLogicFrame>::Listen(&onLogicEvent);
		Events::Event<Engine::NextRenderFrame>::Listen(&onRenderEvent);
		Events::Event<Engine::GameInitalizeData>::Listen(&onGameInitalizeEvent);
		Events::Event<Engine::KeyEvent>::Listen(&onKeyPress);
		
		theOne = crossfire::entities::create();
		crossfire::linear::create(theOne, crossfire::Heading::Right, 0, 0, crossfire::Velocity::Quarter); 
	}


	void onLogicEvent(const Engine::NextLogicFrame& frame) {
		crossfire::linear::changeHeading(theOne, desiredHeading);
		//desiredHeading = crossfire::Heading::Stopped;
		crossfire::linear::run(1.0f);
		auto values = crossfire::linear::getEntitiesPositions();
		dynamic_elements.clear();
		dynamic_elements.reserve(values.size()); 
		for (int i = 0; i < values.size(); ++i) {
			dynamic_elements.emplace_back(
					box_visual,
					CrossFireVisual::gameUnitToFloat(values.positions[i].x),
					CrossFireVisual::gameUnitToFloat(values.positions[i].y)
			);
		};
	}

	void onRenderEvent(const Engine::NextRenderFrame & frame) {
		namespace vs = Engine::Visuals::Basic;
		namespace math = Engine::Math; 
		vs::RenderState state{ cam };

		for (auto const& renderable : dynamic_elements) {
			Engine::Matrix world;
			Engine::fMatrix worldf;
			//Engine::fVector4 rotVector = math::XMVectorSet(0, 1, 1, 0);

			//rotAngle += 0.01;

			//worldf = math::XMMatrixRotationAxis(rotVector, Engine::Math::XMConvertToRadians(rotAngle));
			worldf = math::XMMatrixTranslation(renderable.x+5, renderable.y+5, -205);
			//worldf = math::XMMatrixTranslation(0, 0, -200);
			math::XMStoreFloat4x4(&world, worldf);

			state.states.push_back({ world, renderable.visualId });

			state.lights.push_back(Engine::Lights::AnyLight{
			Engine::Lights::Point {
				{renderable.x + 5,renderable.y + 5,-180},
				{ 0,0.3,0,0},
				 0.01,0.02,0.0003	}
			});

		}

		int count = 0;
		for (auto const& renderable : static_elements) {
			//if (count > 7) break; 
			Engine::Matrix world;
			Engine::fMatrix worldf;
			worldf = math::XMMatrixTranslation(renderable.x, renderable.y, -200);
			//worldf = math::XMMatrixTranslation(0, 0, -200.5);
			math::XMStoreFloat4x4(&world, worldf);
			state.states.push_back({ world, renderable.visualId });
			++count;
		}
		
		state.GlobalAmbientLight = { 0.1,0.1,0.1,1 };
		state.lights.push_back(Engine::Lights::Directional{
			{0,0,-1},
			{0.3,0.3,0.3,0}
			});

		

		/*state.lights.push_back(Engine::Lights::AnyLight{
			Engine::Lights::Point {
				{-2,-2,-20},
				{ 0.3,1,1,0},
				0.001,0.002,0.003	}
			});

		state.lights.push_back(Engine::Lights::AnyLight{
			Engine::Lights::Point {
				{0,0,-153},
				{ 0.2,0.1,0.33,0},
				0.001,0.002,0.003	}
			});

		*/
		vs::Render(std::move(state));
	}

	void onGameInitalizeEvent(const Engine::GameInitalizeData & data) {
		namespace vs = Engine::Visuals::Basic;
		
		Geometry::VertexCollection vbs;
		Geometry::IndexCollection  ids;
		auto screenInfo = vs::Get_ScreenSize();

		{
			vbs.clear(); 
			ids.clear();
			Geometry::ComputeBox(vbs, ids, { 10, 10,10 }, false, false);
			//Geometry::ComputeTeapot(vbs, ids, 2, 4, false);

			Engine::MeshView mesh_view{ Engine::make_meshview(vbs,ids) };
			Engine::Material mat{
				{0,0,0,0},
				{0.9,0.9,0.9,1},
				{0.9,0.9,0.9,1},
				{0.9,1,0.9,1},
				128 };
			box_visual = vs::Create(mesh_view, mat);
		}
		{
			vbs.clear();
			ids.clear();
			auto path = crossfire::map::getRowPath(); 
			Geometry::ComputeBox(vbs, ids, { static_cast<float>(path.x2-path.x) , static_cast<float>(path.y2-path.y),.01f}, false, false);
			//Geometry::ComputeBox(vbs, ids, { 10,80,0.1f }, false, false);

			Engine::MeshView mesh_view{ Engine::make_meshview(vbs,ids) };
			Engine::Material mat{
				{0,0,0,0},
				{0.1,0.1,0.5,1},
				{0.9,0.9,0.9,1},
				{0.9,1,0.9,1},
				128 };
			row_visual = vs::Create(mesh_view, mat);
		}
		{
			vbs.clear();
			ids.clear();
			auto path = crossfire::map::getColPath();
			Geometry::ComputeBox(vbs, ids, { static_cast<float>(path.x2 - path.x) , static_cast<float>(path.y2 - path.y),.001f }, false, false);
			//Geometry::ComputeTeapot(vbs, ids, 2, 4, false);

			Engine::MeshView mesh_view{ Engine::make_meshview(vbs,ids) };
			Engine::Material mat{
				{0,0,0,0},
				{0.5,0.1,0.1,1},
				{0.9,0.9,0.9,1},
				{0.9,1,0.9,1},
				128 };
			col_visual = vs::Create(mesh_view, mat);
		}

		auto paths = crossfire::map::getPaths();
		auto isRow = [](const crossfire::map::path& p) -> bool {
			return (p.x2 - p.x) > (p.y2 - p.y);
		};

		for (const auto& p : paths) {
			const auto asRow = isRow(p);
			static_elements.emplace_back(
				asRow ? row_visual : col_visual,
				CrossFireVisual::gameUnitToFloat(p.x) + static_cast<float>(p.x2 - p.x)/2,
				CrossFireVisual::gameUnitToFloat(p.y) + static_cast<float>(p.y2 - p.y)/2
			);
		}


		cam.setPerspectiveFOV(Engine::Math::XMConvertToRadians(45), screenInfo.aspect_ratio(), .1f, 500.f);
		//cam.setPosition({ 0,0,-10,0 });
		//cam.setRotation({ })
		cam.lookAt({ 85,85,50,0 }, { 85,85,-1,0 }, { 0,-1,0,0 });
	}
}