#pragma once

#include <optional>
#include <filesystem>
#include <variant>
#include <memory>
#include <gsl/span>
#include <filesystem>

namespace Engine::Resources {

	using RawID_IntID = uint64_t;
	using RawID_StringID = std::filesystem::path;

	using RawID = std::variant<uint64_t, std::filesystem::path>;

	struct RawResource {
		std::unique_ptr<std::byte[]> file_data;
		std::size_t size;
		RawID id;

		gsl::span<std::byte> asSpan() {
			return { file_data.get(), static_cast<gsl::span<std::byte>::index_type>(size) };
		}
//		FileInfo * fileinfo;
	};

/*	struct FileInfo {
		std::string file_ending;
		std::size_t file_size;
		bool		isWritable;
	};*/

/*	class FileInfo {
	public:
		virtual ~FileInfo() = 0;
		virtual  FileInfo getFileInformation(RawID id);
	};
*/
	class RawLoader {
	public:
		virtual ~RawLoader() = 0;
		virtual std::optional<RawResource> read_item(RawID id) const = 0;
		virtual bool		has_item(RawID id) const = 0;
	};


}