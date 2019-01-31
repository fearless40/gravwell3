#pragma once

namespace Graphics::Generic
{
	namespace ColorLayout {
		struct RGBA {
			static constexpr unsigned int red = 0;
			static constexpr unsigned int green = 1;
			static constexpr unsigned int blue = 2;
			static constexpr unsigned int alpha = 3;
		};

		struct BGRA {
			static constexpr unsigned int red = 2;
			static constexpr unsigned int green = 1;
			static constexpr unsigned int blue = 0;
			static constexpr unsigned int alpha = 3;
		};
	};
	
	namespace detail {
		struct Color {
			float colors[4];

			const float * cdata() const noexcept { return colors; }

			float * begin() { return colors; }
			float * end() { return &colors[5]; }
		};
	}

	template<typename ColorOrder>
	struct Color : public detail::Color {
		using color_layout = ColorOrder;

		template<typename DifferntColorOrder>
		explicit Color(const DifferntColorOrder & c) {
			red() = c.red();
			green() = c.green();
			blue() = c.blue();
			alpha() = c.alpha();
		}

		float & red()	noexcept { return &colors[color_layout::red]; }
		float & green() noexcept { return &colors[color_layout::green]; }
		float & blue()	noexcept { return &colors[color_layout::blue]; }
		float & alpha() noexcept { return &colors[color_layout::alpha]; }

		constexpr float red()	const noexcept { return colors[color_layout::red]; }
		constexpr float green()	const noexcept { return colors[color_layout::green]; }
		constexpr float blue()	const noexcept { return colors[color_layout::blue]; }
		constexpr float alpha()	const noexcept { return colors[color_layout::alpha]; }

		constexpr void red(float v)	noexcept { colors[color_layout::red] = v; }
		constexpr void green(float v)noexcept { colors[color_layout::green] = v; }
		constexpr void blue(float v)	noexcept { colors[color_layout::blue] = v; }
		constexpr void alpha(float v)noexcept { colors[color_layout::alpha] = v; }


		Color() {
			colors[0] = 0.0f;
			colors[1] = 0.0f;
			colors[2] = 0.0f;
			colors[3] = 0.0f;
		}

		Color(float r, float g, float b, float a) {
			red(r);
			green(g);
			blue(b);
			alpha(a);
		}

		
	};

	

/*	template<typename ColorInput, typename PreferredColorType>
	Color<PreferredColorType> convert(const ColorInut & ci) {
		return { ci };
	}*/

	using RGBA = Color<ColorLayout::RGBA>;

}