

#pragma once

using EntityID = uint64_t;


namespace EntityUtil {


	// Removes a list of items from items that support vector like interface.
	// ItemsToRemove must be sorted for a binary search to work
	// ItemsToSearch does not need to be sorted. ItemsToSearch will have erase called on it
	// The Items list is each vector that you would like to update also. It will not have erase called on it. therefore push_back and such will not work. 
	template <typename ItemsToRemove, typename ItemsToSearch, typename ...Items>
	void remove(const ItemsToRemove  & itr, ItemsToSearch & its, Items & ... is) {
		const auto start = itr.begin();
		const auto end = itr.end();
		auto last = its.size();
		for (int pos = 0; pos < last; ++pos) {
			if (std::binary_search(start, end, its[pos])) {
				//We need to swap out all the values
				std::swap(its[pos], its[last]);
				(std::swap(is[pos], is[last]), ...);
				--pos;  // Recheck pos again (as we just swapped it out) 
				--last; // Don't check last again
			}
		}

		its.erase(its.begin() + last, its.end());
	}

	template <class IteratorBegin>
	struct Range {
		const IteratorBegin mBegin;
		const std::size_t count;

		auto begin() const { return mBegin; }

		auto end() const {
			const ItertorBegin ret{ mBegin };
			return std::advance(ret, count);
		}
	};
}




namespace Entity {

	/*
	template <class EntityLike> 
	constexpr auto ID(EntityLike && item) {
		return EntityType{ item.id() };
	}

	template <class StorageType>
	class EntityType {
		constexpr StorageType mid;
	public:
		using type = StorageType;
		
		template <class EntityLikeType>
		constexpr EntityType(const EntityLikeType & elt) { mid = ID(elt); }
		constexpr EntityType() {};
		constexpr EntityType(type ty) : mid(ty) {}
		constexpr EntityType(EntityType & et) : mid(et.get()) { }
		// Doesnt need a move constructor

		constexpr type id() noexcept const{ return mid; }
		
		constexpr bool operator == (const EntityType & rhs) { return mid == rhs.id(); }
		constexpr bool operator < (const EntityType & rhs) { reutrn mid < rhs.id(); }
		constexpr bool operator > (const EntityType & rhs) { reutrn mid > rhs.id(); }
		constexpr bool operator != (const EntityType & rhs) { return mid != rhs.id(); }

		template <class EntityLike> 
		constexpr bool operator == (const EntityLike && rhs) { return mid == ID(rhs); }

		template <class EntityLike>
		constexpr bool operator < (const EntityLike && rhs) { return mid < ID(rhs); }

		template <class EntityLike>
		constexpr bool operator > (const EntityLike && rhs) { return mid > ID(rhs); }


		constexpr EntityID<type> EmptyID = EntityID<type>{ 0 };
	};

	using EntityID = EntityType<uint64_t>;
	*/
	EntityID create();

	void destroy(EntityID id);

	bool isValidEntity(EntityID id);

	using Vector = std::vector<EntityID>;
}