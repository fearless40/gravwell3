#pragma once

namespace Events {
	template <class Msg>
	class Event {
	public:
		using callback = std::function< void(const Msg &)>;

		struct Token {
			std::size_t value;
		};

		static const Token Listen(callback cb) {
			mRecievers.push_back(cb);
			return Token{ mRecievers.size() - 1 };
		}

		static void Fire(Msg msg) {
			for (auto & i : mRecievers) {
				std::invoke(i, msg);
			}
		}

		// todo: change void to some form of task
		static void FireAsync(Msg msg) {
			// Do nothing for now
		}

		static Remove(const Token tk) {
			mRecievers.erase(mRecievers.begin() + tk.value);
		}

	private:
		static std::vector<callback> mRecievers;
	};

	template<class T>
	std::vector<Message<T>::callback> Message<T>::mRecievers;
}