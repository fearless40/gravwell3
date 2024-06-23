#pragma once
#include <cstdlib>

namespace Game {
	class FixedFunctionFloat {
	public:
		static const unsigned char DECIMAL_BITS = 4;
		static const unsigned int scaling_factor = 2 << DECIMAL_BITS;
		using int_type = __int64;
		using dec_type = unsigned char;

		FixedFunctionFloat(int int_value) {
			value = make_value(int_value, 0);
		}

		explicit FixedFunctionFloat(int_type int_value) {
			value = make_value(int_value, 0);
		}
		
		explicit FixedFunctionFloat(int_type int_value, dec_type dec_value) {
			value = make_value(int_value, dec_value);
		}

		explicit FixedFunctionFloat(double value) {
			value = llround(value * scaling_factor);
		}

		explicit FixedFunctionFloat(float value) {
			value = lround(value * scaling_factor);
		}

		//Coord(const Coord & rhs) = default;
				
		//Coord & operator = (const Coord & rhs) = default;
		

		FixedFunctionFloat & operator += (const FixedFunctionFloat & other) {
			value += other.value;
			return *this;
		}

		FixedFunctionFloat & operator -= (const FixedFunctionFloat & other) {
			value -= other.value;
			return *this;
		}

		
		constexpr bool operator < (const FixedFunctionFloat & rhs)  const noexcept { return value < rhs.value; }
		constexpr bool operator > (const FixedFunctionFloat & rhs)  const noexcept { return value > rhs.value; }
		constexpr bool operator == (const FixedFunctionFloat & rhs) const noexcept { return value == rhs.value; }
		constexpr bool operator != (const FixedFunctionFloat & rhs) const noexcept { return value != rhs.value; }
		constexpr bool operator >= (const FixedFunctionFloat & rhs) const noexcept { return value >= rhs.value; }
		constexpr bool operator <= (const FixedFunctionFloat & rhs) const noexcept { return value <= rhs.value; }
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

	static FixedFunctionFloat operator + (FixedFunctionFloat lhs, const FixedFunctionFloat & rhs) {
		FixedFunctionFloat ret{ lhs };
		return ret += rhs;
	}

	static FixedFunctionFloat operator - (FixedFunctionFloat lhs, const FixedFunctionFloat & rhs) {
		FixedFunctionFloat ret{ lhs };
		return ret -= rhs;
	}
}