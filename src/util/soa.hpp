#pragma once

#include <tuple>
#include <array>
#include <type_traits>
#include <span>



namespace util::soa
{
		template<unsigned int MAX_CAPACITY>
		struct ProtocolFixedArray {

			static const bool is_fixed_capacity = true;
			static const std::size_t capacity = MAX_CAPACITY;

			using index_t = std::size_t;

			template<typename data_t>
			using value_t = std::array<data_t, MAX_CAPACITY>;

			template<typename data_t>
			using reference_t = std::decay_t<data_t>&;

			template<typename data_t>
			using pointer_t = std::add_pointer_t<data_t>;

			template<typename data_t>
			static value_t<data_t> allocate() {
				return {};
			}

			template<typename data_t>
			//requires (data_t) { data_t.end() }
			static constexpr auto end(const data_t& value) {
				return value.end();
			}

			template<typename data_t>
			//requires ( data_t.begin() )
			static constexpr auto begin(const data_t& value) {
				return value.begin();
			}

			template<typename data_t>
			//requires ( data_t.operator [] )
			static constexpr typename data_t::reference at(data_t& value, index_t index) {
				return value[index];
			}

			template<typename data_t>
			//requires ( data_t.operator [] )
			static constexpr typename data_t::pointer pointer_at(data_t& value, index_t index) {
				return &value[index];
			}

			template<typename data_t>
			//requires ( data_t.operator [] )
			static constexpr void set(data_t& array, index_t index, const data_t::value_type& item_value) {
				array[index] = item_value;
			}

			template<typename data_t>
			//requires (data_t.data() )
			static constexpr typename data_t::pointer get_pointer(data_t& value) {
				return value.data();
			}

		};

		namespace detail {
			struct DynamicCapacity {
				std::size_t mCapacity;
				constexpr auto capacity() {
					return mCapacity;
				}

				constexpr void add_capacity(std::size_t amount) {
					mCapacity += amount;
				}

				constexpr void remove_capacity(std::size_t amount) {
					mCapacity -= amount;
				}
			};

			template<std::size_t amount>
			struct FixedCapacity {
				constexpr auto capacity() {
					return amount;
				}

				constexpr void add_capacity(std::size_t amount_value) {
					return;
				}

				constexpr void remove_capacity(std::size_t amount_value) {
					return;
				}
			};

			template<typename policy_t>
			using Capacity = std::conditional_t<policy_t::is_fixed_capacity, detail::FixedCapacity<policy_t::capacity>, detail::DynamicCapacity>;
		}

		template<typename protocol_t, typename ... structures_t>
		class SOA : public detail::Capacity<protocol_t> {
		public:
			using storage_t = std::tuple<typename protocol_t::template value_t<structures_t>...>;
			using value_t = std::tuple<typename protocol_t::template reference_t<structures_t>...>;
			using value_ref_t = std::tuple<typename protocol_t::template reference_t<structures_t>...>;
			using pointer = std::tuple<typename protocol_t::template pointer_t<structures_t>...>;

			template<typename T>
			using array_storage_t = typename protocol_t::template value_t<std::decay_t<T>>;

			using index_t = std::size_t;

			using detail::Capacity<protocol_t>::capacity;

		private:
			index_t mSize{ 0 };
			storage_t mArrays{  };

			template<typename structure_t>
			constexpr array_storage_t<structure_t>& get_array() {
				return std::get<array_storage_t<structure_t>>(mArrays);
			}

		public:

			class Iterator
			{
			public:

				using iterator_category = std::random_access_iterator_tag;
				using value_type = pointer;
				using difference_type = std::ptrdiff_t;
				using pointer = pointer;
				using reference = value_ref_t;
				using iterator_concept = std::contiguous_iterator_tag;

			private:
				std::size_t mIndex{ 0 };
				SOA* mSOA{ nullptr };

			public:

				Iterator() {}
				Iterator(std::size_t index, SOA* soa) : mIndex(index), mSOA(soa) {}
				auto operator*() const { return mSOA->pointer_at(mIndex); }
				auto& operator++() { ++mIndex; return *this; }
				auto  operator++(int) { auto tmp = *this; ++(*this); return tmp; }
				auto& operator--() { --mIndex; return *this; }
				auto  operator--(int) { auto tmp = *this; --(*this); return tmp; }
				auto& operator+=(difference_type amount) { mIndex += amount; return *this; }
				auto& operator-=(difference_type amount) { mIndex -= amount; return *this; }

				friend Iterator operator +(const Iterator& left, difference_type right) {
					Iterator tmp(left);
					return tmp += right;
				}
				friend Iterator operator +(difference_type right, const Iterator& left) {
					Iterator tmp(left);
					return tmp += right;
				}


				friend Iterator operator +(const Iterator& left, const Iterator& right) {
					Iterator tmp(left);
					return tmp += right;
				}
				friend difference_type operator -(const Iterator& left, const Iterator& right) {
					return left.mIndex - right.mIndex;
				}
				friend Iterator operator -(const Iterator& left, difference_type right) {
					Iterator tmp(left);
					return tmp -= right;
				}

				Iterator::pointer operator  [](const difference_type amount) {
					return mSOA->pointer_at(mIndex + amount);
				}

				constexpr std::strong_ordering operator<=>(const Iterator& other) const {
					return mIndex <=> other.mIndex;
				}
				bool operator == (const Iterator& other) const = default;




				std::size_t index() { return mIndex; }

				//static_assert(std::destructible<Iterator>);
				//static_assert(std::random_access_iterator<Iterator>);
			};

			constexpr std::size_t size() {
				return mSize;
			}


			void push_back(const structures_t & ... items) {
				if (capacity() < mSize + 1) {
					// Then Allocate
				}


				(protocol_t::set(get_array<structures_t>(), mSize, items), ...);
				++mSize;
			}

			value_ref_t at(index_t index) {
				return value_ref_t{
					protocol_t::at(get_array<structures_t>(), index)...
				};
			}

			pointer pointer_at(index_t index) {
				return pointer{
					protocol_t::pointer_at(get_array<structures_t>(), index)...
				};
			}

			template<typename row_t>
			auto row() {
				return protocol_t::get_pointer(get_array<row_t>());
			}

			template<typename row_t>
			std::span<row_t> row_span() {
				return std::span<row_t>(protocol_t::get_pointer(get_array<row_t>()), mSize);
			}

			template<typename row_t>
			auto row_begin() {
				return protocol_t::begin(get_array<row_t>());
			}

			template<typename row_t>
			auto row_end() {
				return protocol_t::end(get_array<row_t>());
			}


			constexpr Iterator begin() {
				return Iterator(0, this);
			}

			constexpr Iterator end() {
				return Iterator(mSize, this);
			}

			template<typename T>
			constexpr Iterator find_first(const T& to_find) {
				const auto to_search = row_span();
				for (index_t i = 0; i < mSize; ++i) {
					if (to_search[i] == to_find) return Iterator(i, this);
				}
				return end();
			}

		};
	}
	
/*
	int main() {

		using data_t = util::SOA<util::ProtocolFixedArray<64>, int, float>;
		data_t data;

		static_assert(std::same_as<std::iter_reference_t<data_t::Iterator>, data_t::Iterator::pointer> == true);
		static_assert(std::random_access_iterator<typename data_t::Iterator>);


		data.push_back(1, 0.1f);
		data.push_back(2, 0.2f);
		data.push_back(3, 0.3f);

		//auto f1 = std::find_if(data.begin(),data.end(),[](auto & val){ val == 3;});
		auto v = data.at(0);

		auto row = data.row_span<float>();
		for (auto t : data) {
			std::cout << *std::get<0>(t) << ":" << *std::get<1>(t) << '\n';
		}

		std::cout << std::get<0>(v) << " " << std::get<1>(v) << '\n';
		std::cout << data.capacity() << " " << data.size() << " ";
}


*/
