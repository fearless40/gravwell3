#include "../stdafx.h"
#include "Game.h"
//#include "Entity.h"
//#include "LinearMotion.h"
#include "../engine/Visuals.h"
#include "../procedural/Geometry.h"
#include "../engine/Event.h"
#include "../engine/GameEvent.h"
#include "../engine/Camera.h"

namespace Game {

	Engine::Visuals::Basic::Visual box_visual;
	Camera	cam;

	void Initalize() {
		Events::Event<Engine::NextLogicFrame>::Listen(&onLogicEvent);
		Events::Event<Engine::NextRenderFrame>::Listen(&onRenderEvent);
		Events::Event<Engine::GameInitalizeData>::Listen(&onGameInitalizeEvent);

		//cam.setUpVector({ 0.f,1.f,0.f });
		cam.setPosition({ 0.f,0.f,-10.f });
		cam.lookAt({ 0.f,0.f,0.f });
		//cam.setPerspective()
	}


	void onLogicEvent(const Engine::NextLogicFrame & frame) {

	}

	void onRenderEvent(const Engine::NextRenderFrame & frame) {
		namespace vs = Engine::Visuals::Basic;
		Engine::Matrix world;
		Engine::Math::XMStoreFloat4x4(&world, Engine::Math::XMMatrixIdentity());

		vs::VisualState state;
		state.states.push_back({ world, box_visual });
		Engine::Math::XMStoreFloat4x4(&state.view, cam.getView());
		vs::Render(std::move(state));
	}

	void onGameInitalizeEvent(const Engine::GameInitalizeData & data) {
		namespace vs = Engine::Visuals::Basic;
		
		Geometry::VertexCollection vbs;
		Geometry::IndexCollection  ids;

		Geometry::ComputeBox(vbs, ids, { .5f,.5f,.5f }, false, false);

		Engine::MeshView mesh_view{ Engine::make_meshview( vbs,ids ) };
		Engine::Material mat;

		box_visual = vs::Create(mesh_view, mat);
		
	}
}