//@ {"target":{"name":"interp.test"}}

#include "./interp.hpp"

#include <testfwk/testfwk.hpp>

#include <array>

TESTCASE(palgen_interp)
{
	static constexpr std::array lut{
		1.0f,
		2.0f,
		4.0f,
		8.0f
	};

	EXPECT_EQ(
		interp(lut, 0.0f, palgen::periodic_boundary_sampling_policy{}),
		1.0f
	);

	EXPECT_EQ(
		interp(lut, 0.5f, palgen::periodic_boundary_sampling_policy{}),
		1.5f
	);

	EXPECT_EQ(
		interp(lut, 1.0f, palgen::periodic_boundary_sampling_policy{}),
		2.0f
	);

	EXPECT_EQ(
		interp(lut, 1.5f, palgen::periodic_boundary_sampling_policy{}),
		3.0f
	);

	EXPECT_EQ(
		interp(lut, 2.0f, palgen::periodic_boundary_sampling_policy{}),
		4.0f
	);

	EXPECT_EQ(
		interp(lut, 2.5f, palgen::periodic_boundary_sampling_policy{}),
		6.0f
	);

	EXPECT_EQ(
		interp(lut, 3.0f, palgen::periodic_boundary_sampling_policy{}),
		8.0f
	);

	EXPECT_EQ(
		interp(lut, 3.5f, palgen::periodic_boundary_sampling_policy{}),
		4.5f
	);

	EXPECT_EQ(
		interp(lut, 4.0f, palgen::periodic_boundary_sampling_policy{}),
		1.0f
	);

	EXPECT_EQ(
		interp(lut, 8.0f, palgen::periodic_boundary_sampling_policy{}),
		1.0f
	);
}
