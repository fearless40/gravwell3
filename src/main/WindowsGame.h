#pragma once
#include "Window.h"
#include "TimeStep.h"


namespace Graphics::D3D11 {
	class Driver;
}

class WindowsGame :  Window {
public:
	WindowsGame(HINSTANCE hInstance, Util::CommandLineParameters clp);
	~WindowsGame();

	void run();
	void shutdown();

private:

	void next_frame(); 
	void next_game_step(float since_last_frame);
	void next_render_step(float since_last_frame);

	void onSize(stdBounds newSize) override;

	HINSTANCE mInst;
	HACCEL	  mAccelTable;
	Util::TimeStep timer;
	bool mRunning;
	std::unique_ptr<Graphics::D3D11::Driver> graphics_driver{ nullptr };

};