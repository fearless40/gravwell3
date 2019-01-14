#pragma once
//#include <spdlog/spdlog.h>
#include "../util/CmdLineParser.h"


namespace Engine {
	
	namespace events {
		struct RequestShutDown{};
		struct ShutDownNow {};
		struct SleepNow{};
		struct LoadData{};
	}


	Util::CommandLineParameters Initalize(std::wstring_view cmdLine);
	void Shutdown();
	//std::shared_ptr<spdlog::logger> GetDefaultLog();
	
	template <typename RenderT>
	void SetRender(RenderT & t) {
		render = t;
	}

	template<typename RenderT>
	static RenderT render;

}


