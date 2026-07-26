//@	{"target":{"name":"main-cli.o"}}

#include "./fixed_intensity_colormap.hpp"
#include "./color.hpp"

#include <algorithm>
#include <random>
#include <cstdio>
#include <memory>

struct file_deleter
{
	void operator()(FILE* f) const
	{
		if(f != nullptr)
		{ fclose(f); }
	}
};

int main()
{
	palgen::fixed_intensity_colormap colormap{palgen::perceptual_color_intensity{0.5f}};
	std::array<
		std::pair<size_t, palgen::linear_color>, 60
		>
	colors{};
	for(size_t k = 0; k != std::size(colors); ++k)
	{
		auto const t = static_cast<float>(k)/static_cast<size_t>(std::size(colors));
		colors[k] = std::pair{k, colormap(t)};
	}

	std::shuffle(std::begin(colors), std::end(colors), std::random_device{});
	std::unique_ptr<FILE, file_deleter> color_file{fopen("/dev/shm/colors.txt", "wb")};
	std::unique_ptr<FILE, file_deleter> index_file{fopen("/dev/shm/index_file.txt", "wb")};
	for(auto item: colors)
	{
		fprintf(color_file.get(), "%s\n", to_string(convert<palgen::srgb_tag>(item.second)).c_str());
		fprintf(index_file.get(), "%zu\n", item.first);
	}
}
