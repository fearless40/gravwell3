#pragma once


namespace Graphics::Generic::IndexSize {
	struct Size16 {
		constexpr std::size_t size() { return 16; }
	};

	struct Size32 {
		constexpr std::size_t size() { return 32; }
	};
}