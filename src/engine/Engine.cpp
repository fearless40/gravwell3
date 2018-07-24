#include "../stdafx.h"
#include "Engine.h"
#include "Event.h"


namespace Engine {
	//std::shared_ptr<spdlog::logger> mLogger;
	
	Util::CommandLineParameters Initalize(std::wstring_view cmdLine) {
		auto clp = Util::ParseCommandLine(cmdLine);
		//mLogger = spdlog::create<spdlog::stdout_color_st>("default_log");

		//Create the events that we need
		//Events::Event<Engine::events::ShutDownNow>::Listen([](auto x) { Engine::Shutdown(); });
		return clp;
	}
	
	void Shutdown() {
		Events::Event<Engine::events::ShutDownNow>::Fire({});
	}

	/*std::shared_ptr<spdlog::logger> GetDefaultLog() {
		return mLogger;
	}*/
}