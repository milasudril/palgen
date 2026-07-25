//@	{"target":{"name":"color.test"}}

#include "./color.hpp"

#include <testfwk/testfwk.hpp>

TESTCASE(palgen_convert_linear_to_srgb)
{
	{
		auto const res = convert<palgen::srgb_tag>(palgen::black);
		EXPECT_EQ(res.r, 0.0f);
		EXPECT_EQ(res.g, 0.0f);
		EXPECT_EQ(res.b, 0.0f);
	}


	{
		auto const res = convert<palgen::srgb_tag>(
			palgen::linear_color{
				.r = 0.0031308f,
				.g = 0.0031308f,
				.b = 0.0031308f
			}
		);
		EXPECT_EQ(static_cast<int>(255.0f*res.r), 10);
		EXPECT_EQ(static_cast<int>(255.0f*res.g), 10);
		EXPECT_EQ(static_cast<int>(255.0f*res.b), 10);
	}

	{
		auto const res = convert<palgen::srgb_tag>(
			palgen::linear_color{
				.r = 0.5f,
				.g = 0.5f,
				.b = 0.5f
			}
		);
		EXPECT_EQ(static_cast<int>(255.0f*res.r), 187);
		EXPECT_EQ(static_cast<int>(255.0f*res.g), 187);
		EXPECT_EQ(static_cast<int>(255.0f*res.b), 187);
	}

	{
		auto const res = convert<palgen::srgb_tag>(
			palgen::linear_color{
				.r = 1.0f,
				.g = 0.0f,
				.b = 0.0f
			}
		);

		EXPECT_EQ(static_cast<int>(255.0f*res.r), 255);
		EXPECT_EQ(res.g, 0.0f);
		EXPECT_EQ(res.b, 0.0f);
	}

	{
		auto const res = convert<palgen::srgb_tag>(
			palgen::linear_color{
				.r = 0.0f,
				.g = 1.0f,
				.b = 0.0f
			}
		);

		EXPECT_EQ(res.r, 0.0f);
		EXPECT_EQ(static_cast<int>(255.0f*res.g), 255);
		EXPECT_EQ(res.b, 0.0f);
	}

	{
		auto const res = convert<palgen::srgb_tag>(
			palgen::linear_color{
				.r = 0.0f,
				.g = 0.0f,
				.b = 1.0f
			}
		);

		EXPECT_EQ(res.r, 0.0f);
		EXPECT_EQ(res.g, 0.0f);
		EXPECT_EQ(static_cast<int>(255.0f*res.b), 255);
	}
}

TESTCASE(palgen_brighten_target_intensity_within_range)
{
	using intensity = palgen::intensity<palgen::linear_color{1.0f, 0.5f, 0.25f}>;

	auto const input = palgen::linear_color{
		.r = 0.25f,
		.g = 0.5f,
		.b = 0.75f
	};

	intensity const i_in{input};
	EXPECT_EQ(i_in, 0.25f + 0.25f + 0.1875);

	auto const res = brighten(
		input,
		intensity{1.0f}
	);

	intensity const i_out{res};
	EXPECT_EQ(i_out, 1.0f);
	EXPECT_EQ(
		res,
		(
			palgen::linear_color{
				.r = 0.47058824f,
				.g = 0.64705884f,
				.b = 0.82352936f
			}
		)
	);
}

TESTCASE(palgen_brighten_target_intensity_below_input_intensity_keeps_value)
{
	using intensity = palgen::intensity<palgen::linear_color{1.0f, 0.5f, 0.25f}>;

	auto const input = palgen::linear_color{
		.r = 0.25f,
		.g = 0.5f,
		.b = 0.75f
	};

	intensity const i_in{input};
	EXPECT_EQ(i_in, 0.25f + 0.25f + 0.1875);

	auto const res = brighten(input, 0.5f*i_in);

	intensity const i_out{res};
	EXPECT_EQ(i_out, i_in);
	EXPECT_EQ(res, input);
}

TESTCASE(palgen_brighten_target_intensity_above_white_intensity_clamps_to_white)
{
	using intensity = palgen::intensity<palgen::linear_color{1.0f, 0.5f, 0.25f}>;

	auto const input = palgen::linear_color{
		.r = 0.25f,
		.g = 0.5f,
		.b = 0.75f
	};

	intensity const i_in{input};
	EXPECT_EQ(i_in, 0.25f + 0.25f + 0.1875);

	intensity const i_white{palgen::white};

	{
		auto const res = brighten(input, i_white);
		intensity const i_out{res};
		EXPECT_EQ(res, palgen::white);
	}

	{
		auto const res = brighten(input, 1.25f*i_white);
		intensity const i_out{res};
		EXPECT_EQ(res, palgen::white);
	}
}
