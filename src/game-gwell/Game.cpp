#include "../stdafx.h"
#include "Game.h"
#include "Entity.h"
#include "LinearMotion.h"

namespace Game {
	void onUpdate() {
		LinearMotion::update();
	}

	void onRender() {

	}
}