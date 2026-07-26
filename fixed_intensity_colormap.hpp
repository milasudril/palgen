#ifndef PALGEN_FIXED_INTENSITY_COLORMAP_HPP
#define PALGEN_FIXED_INTENSITY_COLORMAP_HPP

#include "./color.hpp"
#include "./interp.hpp"
#include <array>

namespace palgen
{
	class fixed_intensity_colormap
	{
	public:
		constexpr explicit fixed_intensity_colormap(perceptual_color_intensity intensity):
			m_lut{
				normalize(basic_colors[0], intensity),
				normalize(basic_colors[1], intensity),
				normalize(basic_colors[2], intensity),
				normalize(basic_colors[3], intensity),
				normalize(basic_colors[4], intensity),
				normalize(basic_colors[5], intensity),
			}
		{}

		static constexpr auto make_pastels(
			fixed_intensity_colormap const& src,
			perceptual_color_intensity intensity
		)
		{
			fixed_intensity_colormap ret{};
			for(size_t k = 0; k != std::size(basic_colors); ++k)
			{ ret.m_lut[k] = brighten(src.m_lut[k], intensity); }

			return ret;
		}

		constexpr auto operator()(float t) const
		{ return interp(m_lut, static_cast<float>(std::size(m_lut))*t, wrap_around_at_boundary{}); }

	private:
		fixed_intensity_colormap() = default;

		static constexpr std::array basic_colors{
			linear_color{1.0f, 0.0f, 0.0f},
			linear_color{1.0f, 0.333f, 0.0f},
			linear_color{1.0f, 1.0f, 0.0f},
			linear_color{0.0f, 1.0f, 0.0f},
			linear_color{0.0f, 0.25f, 1.0f},
			linear_color{0.333f, 0.0f, 1.0f},
		};

		std::array<linear_color, std::size(basic_colors)> m_lut;
	};
}
#endif
