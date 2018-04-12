

#pragma once


using EntityID = __int64;

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
				--pos;  // Recheck pos again (as we just swapped it out) 
				--last; // Don't check last again
				(std::swap(is[pos], is[last]), ...);
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

	enum class Hint {
		Temporary,
		LongTerm
	};

	EntityID create(Hint ht = Hint::LongTerm);

	void destroy(EntityID id);

	bool isValidEntity(EntityID id);
}