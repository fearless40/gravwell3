#pragma once

namespace engine::visuals {
	
	class VisualID {
		unsigned int value{ 0 };
	public:
		bool operator == (VisualID id) { return value == id.value; }
		unsigned int get() { return value; }
	};

	class TagID {
		unsigned int value{ 0 };
	public:
		bool operator == (TagID id) { return value == id.value; }
		unsigned int get() { return value; }
	};

	template <typename Model, typename Shader, typename Texture>
	struct Visual {
		Model * model;
		Shader * shader;
		Texture * texture;
	};

	template <typename VisualType>
	VisualID createVisual(VisualType type) {
		return createVisualTypeErased(gsl::span<std::byte>{&type, sizeof(VisualType)});
	}

	//type erased function
	VisualID createVisualTypeErased(gsl::span<std::byte> data);

	TagID createTag(unsigned int tagValue);

	void addNameToVisual(VisualID id, std::string name);
	void addTagToVisual(VisualID vid, TagID tid);
}