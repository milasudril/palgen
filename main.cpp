//@	{"target":{"name":"main-cli.o"}}

#include "./fixed_intensity_colormap.hpp"
#include "./color.hpp"

#include <random>
#include <cstdio>

constexpr auto make_colormaps()
{
	std::array ret{
		palgen::fixed_intensity_colormap{palgen::perceptual_color_intensity{0.5f/3.0f}},
		palgen::fixed_intensity_colormap{palgen::perceptual_color_intensity{1.0f/3.0f}},
		palgen::fixed_intensity_colormap::make_pastels(
			palgen::fixed_intensity_colormap{palgen::perceptual_color_intensity{1.0f/3.0f}},
			palgen::perceptual_color_intensity{2.0f/3.0f}
		)
	};

	return ret;
}

int main()
{
	static constexpr std::array colormaps = make_colormaps();

	std::uniform_int_distribution<size_t> dist{0, 11};
	std::random_device rng{"/dev/urandom"};

	auto const base_index = dist(rng);

	std::array indices{
		base_index,
		(base_index + 1)%12,
		(base_index + 2)%12,
		(base_index + 6)%12
	};

	for(auto const index : indices)
	{
		for(auto const& cmap: colormaps)
		{
			printf("%s ", to_string(convert<palgen::srgb_tag>(cmap(static_cast<float>(index)/12.0f))).c_str());
		}
		putchar('\n');
	}
}
