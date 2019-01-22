#pragma once


namespace Graphics::Generic::IndexSize {
	struct Size16 {
		constexpr std::size_t size() { return 16; }
		using type = uint16_t;
	};

	struct Size32 {
		constexpr std::size_t size() { return 32; }
		using type = uint32_t;
	};
}