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

TESTCASE(palgen_intensity_of_white_is_1)
{
	using intensity = palgen::intensity<palgen::intensity_weights{1.0f, 0.5f, 0.25f}>;
	intensity const i{palgen::white};
	EXPECT_LT(std::abs(i - 1.0f), 1.0e-5f);
}

TESTCASE(palgen_brighten_target_intensity_within_range)
{
	using intensity = palgen::intensity<palgen::intensity_weights{1.0f, 0.5f, 0.25f}>;

	auto const input = palgen::linear_color{
		.r = 0.25f,
		.g = 0.5f,
		.b = 0.75f
	};

	intensity const i_in{input};
	auto const expected_input_intensity = (0.25f + 0.25f + 0.1875)/(1.0f + 0.5f + 0.25f);
	EXPECT_GT(i_in, expected_input_intensity - 1.0f/1024.0f);
	EXPECT_LT(i_in, expected_input_intensity + 1.0f/1024.0f);

	auto const res = brighten(
		input,
		intensity{1.0f/(1.0f + 0.5f + 0.25f)}
	);

	intensity const i_out{res};
	EXPECT_EQ(i_out, 1.0f/(1.0f + 0.5f + 0.25f));
	EXPECT_EQ(
		res,
		(
			palgen::linear_color{
				.r = 0.4705882f,
				.g = 0.64705884f,
				.b = 0.8235294f
			}
		)
	);
}

TESTCASE(palgen_brighten_target_intensity_below_input_intensity_keeps_value)
{
	using intensity = palgen::intensity<palgen::intensity_weights{1.0f, 0.5f, 0.25f}>;

	auto const input = palgen::linear_color{
		.r = 0.25f,
		.g = 0.5f,
		.b = 0.75f
	};

	intensity const i_in{input};
	auto const expected_input_intensity = (0.25f + 0.25f + 0.1875)/(1.0f + 0.5f + 0.25f);
	EXPECT_GT(i_in, expected_input_intensity - 1.0f/1024.0f);
	EXPECT_LT(i_in, expected_input_intensity + 1.0f/1024.0f);

	auto const res = brighten(input, 0.5f*i_in);

	intensity const i_out{res};
	EXPECT_EQ(i_out, i_in);
	EXPECT_EQ(res, input);
}

TESTCASE(palgen_brighten_target_intensity_above_white_intensity_clamps_to_white)
{
	using intensity = palgen::intensity<palgen::intensity_weights{1.0f, 0.5f, 0.25f}>;

	auto const input = palgen::linear_color{
		.r = 0.25f,
		.g = 0.5f,
		.b = 0.75f
	};

	intensity const i_in{input};
	auto const expected_input_intensity = (0.25f + 0.25f + 0.1875)/(1.0f + 0.5f + 0.25f);
	EXPECT_GT(i_in, expected_input_intensity - 1.0f/1024.0f);
	EXPECT_LT(i_in, expected_input_intensity + 1.0f/1024.0f);

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

TESTCASE(palgen_normalize_to_intensity_below_max_intensity_scales_linearly)
{
	using intensity = palgen::intensity<palgen::intensity_weights{0.5f, 0.25f, 0.125f}>;

	auto const input = palgen::linear_color{
		.r = 0.25f,
		.g = 0.5f,
		.b = 0.75f
	};

	EXPECT_EQ(2.0f*intensity{input}, intensity{2.0f*input});

	{
		auto const res = normalize(input, 1.25f*intensity{input});
		intensity const i_out{res};
		EXPECT_EQ(
			res,
			(
				palgen::linear_color{
					.r = 0.3125,
					.g = 0.625f,
					.b = 0.9375f
				}
			)
		);

		EXPECT_EQ(res.r/input.r, res.g/input.g);
		EXPECT_EQ(res.g/input.g, res.b/input.b);
		EXPECT_EQ(res.b/input.b, res.r/input.r);
	}
}

TESTCASE(palgen_normalize_to_intensity_above_max_intensity_scales_brightens)
{
	using intensity = palgen::unweighted_intensity;

	auto const input = palgen::linear_color{
		.r = 1.0f,
		.g = 0.0f,
		.b = 0.0f
	};

	{
		auto const res = normalize(input, intensity{2.0f/3.0f});
		intensity const i_out{res};
		EXPECT_EQ(
			res,
			(
				palgen::linear_color{
					.r = 1.0f,
					.g = 0.50000006f,
					.b = 0.50000006f
				}
			)
		);
	}
}

TESTCASE(palgen_desaturate)
{
	auto const res = desaturate<palgen::unweighted_intensity>(
		palgen::linear_color{
			.r = 1.0f,
			.g = 0.0f,
			.b = 0.0f
		},
		0.5f
	);

	EXPECT_EQ(palgen::unweighted_intensity{res}, 1.0f/3.0f);

	EXPECT_EQ(res, (palgen::linear_color{2.0f/3.0f, 1.0f/6.0f, 1.0f/6.0f}));
}
