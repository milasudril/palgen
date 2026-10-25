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
		(base_index + 7)%12,
		static_cast<size_t>(3),
		static_cast<size_t>(0),
		static_cast<size_t>(8)
	};

	std::uniform_int_distribution<int> swap_dist{0, 1};
	if(swap_dist(rng) == 1)
	{ std::swap(indices[0], indices[2]);}

	static constexpr std::array color_names{
		"analog-color-a",
		"primary-color",
		"analog-color-b",
		"complement-color",
		"warning-note-color",
		"prohib-note-color",
		"mandatory-note-color"
	};

	static constexpr std::array intensity_names{
		"dark",
		"mid",
		"light",
	};

	for(size_t color_index = 0; color_index != std::size(color_names); ++color_index)
	{
		for(size_t intesity_index = 0; intesity_index != std::size(intensity_names); ++intesity_index)
		{
			auto& cmap = colormaps[intesity_index];
			auto const ci = indices[color_index];
			printf(
				"--%s-%s: %s;\n",
				color_names[color_index],
				intensity_names[intesity_index],
				to_string(convert<palgen::srgb_tag>(cmap(static_cast<float>(ci)/12.0f))).c_str()
			);
		}
		putchar('\n');
	}
}
