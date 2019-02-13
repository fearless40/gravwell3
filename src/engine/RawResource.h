#pragma once

#include <filesystem>
#include <variant>
#include <memory>

namespace Engine::Resources {
	using RawID = std::variant<uint64_t, std::filesystem::path>;

	struct RawResource {
		std::unique_ptr<std::byte> data;
		std::size_t size;
		RawID id;
	};

	class RawLoader {
	public:
		virtual ~RawLoader() = 0;
		virtual RawResource read_item(RawID id) = 0;
		virtual bool		has_item(RawID id) = 0;
	};


	RawResource LoadFile(uint32_t raw_id);
	RawResource LoadFile(std::string file_name);

	void		SetLoader(std::unique_ptr<RawLoader> loader, int priority);
}