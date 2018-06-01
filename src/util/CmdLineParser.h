#pragma once 

namespace Util {
	struct CommandLineParameters {};

	CommandLineParameters ParseCommandLine(std::string_view cmdline);
}