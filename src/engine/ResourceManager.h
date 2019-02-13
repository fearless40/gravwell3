
#pragma once

namespace Engine {
	/*
	using RawResourceID = std::variant<uint64_t, std::string>;

	struct RawResource {
		std::uniqueptr<std::byte> data;
		std::size_t size;
		RawResourceID id;
	};
	
	class ResourceLoader {
	public:
		virtual ~ResourceLoader();
		virtual RawResource read_item(RawResourceID id);
		virtual bool		has_item(RawResourceID id);

	};

	//
	struct ResourceTransform {
		// input RawResource -> output engine compatible resource 
	};



	template<typename ResourceType>
	struct ResourceManager {
		using ID = Engine::ID<ResourceManager, uint32_t>;

		struct ResourceMapItem {
			RawResourceID	raw_id;
			ID				expected_id;
		};
		ystem 
		using ResourceMap = std::vector<ResourceMapItem>;

		void addRawResourceLoader(ResourceLoader * loader, int priority);

		const ResourceType & get(ID id);
		const ResourceType & load_get(RawResourceID rid);

		ID			load(RawResourceID rid);
		void		load_ResourceMap(const ResourceMap & map);
		
		ID			create(ResourceType && resource);

		void		unload(ID id);

	};
	*/
}