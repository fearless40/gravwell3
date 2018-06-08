#pragma once
#include <cstdlib>

namespace Game {
	class Coord {
	public:
		static const unsigned char DECIMAL_BITS = 4;
		static const unsigned int scaling_factor = 16;
		using int_type = __int64;
		using dec_type = unsigned char;

		Coord(int int_value) {
			value = make_value(int_value, 0);
		}

		explicit Coord(int_type int_value) {
			value = make_value(int_value, 0);
		}
		
		explicit Coord(int_type int_value, dec_type dec_value) {
			value = make_value(int_value, dec_value);
		}

		explicit Coord(double value) {
			value = llround(value * scaling_factor);
		}

		explicit Coord(float value) {
			value = lround(value * scaling_factor);
		}

		//Coord(const Coord & rhs) = default;
				
		//Coord & operator = (const Coord & rhs) = default;
		

		Coord & operator += (const Coord & other) {
			value += other.value;
			return *this;
		}

		Coord & operator -= (const Coord & other) {
			value -= other.value;
			return *this;
		}

		
		constexpr bool operator < (const Coord & rhs)  const noexcept { return value < rhs.value; }
		constexpr bool operator > (const Coord & rhs)  const noexcept { return value > rhs.value; }
		constexpr bool operator == (const Coord & rhs) const noexcept { return value == rhs.value; }
		constexpr bool operator != (const Coord & rhs) const noexcept { return value != rhs.value; }
		constexpr bool operator >= (const Coord & rhs) const noexcept { return value >= rhs.value; }
		constexpr bool operator <= (const Coord & rhs) const noexcept { return value <= rhs.value; }
		//Other operators explicitly excluded. There is not point to have multiplication and division. 

		int_type asInt() {
			return value >> DECIMAL_BITS;
		}

		double asDouble() {
			double fixed = value >> DECIMAL_BITS;
			fixed += static_cast<double>(value & 0xF) / 16;
			return fixed;
		}

		float asFloat() {
			float fixed = value >> DECIMAL_BITS;
			fixed += static_cast<float>(value & 0xF) / 16;
			return fixed;
		}

	private:
		int_type make_value(int_type iT, dec_type dV) {
			return (iT << DECIMAL_BITS) | (dV & 0xF);
		}

		
		int_type value = 0;
	};

	static Coord operator + (Coord lhs, const Coord & rhs) {
		return lhs += rhs;
	}

	static Coord operator - (Coord lhs, const Coord & rhs) {
		return lhs -= rhs;
	}
}