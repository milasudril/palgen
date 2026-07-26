#ifndef PALGEN_INTERP_HPP
#define PALGEN_INTERP_HPP

#include <ranges>
#include <cmath>

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

	struct wrap_around_at_boundary
	{
		constexpr float operator()(float param, std::size_t n) const noexcept
		{
			auto const fn = static_cast<float>(n);
			auto wrapped = param - std::floor(param / fn) * fn;
			if (wrapped >= fn)
			{ wrapped = 0.0f; }
			return wrapped;
    }

		constexpr std::size_t operator()(std::size_t index, std::size_t n) const noexcept
		{
			return (index >= n) ? (index - n) : index;
		}
	};
}

#endif
