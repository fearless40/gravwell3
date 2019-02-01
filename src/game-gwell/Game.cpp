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
	float rotAngle = 0.f;
	Engine::Camera	cam;

	void Initalize() {
		Events::Event<Engine::NextLogicFrame>::Listen(&onLogicEvent);
		Events::Event<Engine::NextRenderFrame>::Listen(&onRenderEvent);
		Events::Event<Engine::GameInitalizeData>::Listen(&onGameInitalizeEvent);
	}


	void onLogicEvent(const Engine::NextLogicFrame & frame) {

	}

	void onRenderEvent(const Engine::NextRenderFrame & frame) {
		namespace vs = Engine::Visuals::Basic;
		Engine::Matrix world;
		Engine::fMatrix worldf;
		Engine::fVector4 rotVector = Engine::Math::XMVectorSet(0, 1, 1, 0);
		
		rotAngle += 0.1;

		worldf = Engine::Math::XMMatrixRotationAxis(rotVector, Engine::Math::XMConvertToRadians(rotAngle));
		Engine::Math::XMStoreFloat4x4(&world, worldf);

		vs::RenderState state{ cam };
		state.states.push_back({ world, box_visual });

		vs::Render(std::move(state));
	}

	void onGameInitalizeEvent(const Engine::GameInitalizeData & data) {
		namespace vs = Engine::Visuals::Basic;
		
		Geometry::VertexCollection vbs;
		Geometry::IndexCollection  ids;
		auto screenInfo = vs::Get_ScreenSize();

		//Geometry::ComputeBox(vbs, ids, { .5f,.5f,.5f }, false, false);
		Geometry::ComputeTeapot(vbs, ids, 2, 4, false);

		Engine::MeshView mesh_view{ Engine::make_meshview( vbs,ids ) };
		Engine::Material mat;

		box_visual = vs::Create(mesh_view, mat);

		cam.setPerspectiveFOV(Engine::Math::XMConvertToRadians(45), screenInfo.aspect_ratio(), .1f, 500.f);
		cam.lookAt({ 3,5,10,0 }, { 0,0,0,0 }, { 0,1,0,0 });
	}
}