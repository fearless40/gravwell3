#pragma once
#include <spdlog/spdlog.h>
#include "../util/CmdLineParser.h"


namespace Engine {
	
	namespace Events {
		struct RequestShutDown{};
		struct ShutDownNow {};
		struct SleepNow{};
	}


	Util::CommandLineParameters Initalize(std::wstring_view cmdLine);
	void Shutdown();
	std::shared_ptr<spdlog::logger> GetDefaultLog();
	
}


