#ifndef PALGEN_INTERP_HPP
#define PALGEN_INTERP_HPP

#include <ranges>

namespace palgen
{
	template<std::ranges::random_access_range Range, class BoundarySamplingPolicy>
	constexpr auto interp(Range&& lut, float param, BoundarySamplingPolicy&& bsp)
	{
		using size_type = std::ranges::range_size_t<Range>;
		auto const n = std::size(lut);
		auto const x = bsp(param, n);
		auto const x_0 = static_cast<size_type>(x);
		auto const x_1 = static_cast<size_type>(bsp(x_0 + 1, n));
		auto const left = lut[x_0];
		auto const right = lut[x_1];

		auto const t = x - static_cast<float>(x_0);
		return (1.0f - t)*left + t*right;
	}
}

#endif
