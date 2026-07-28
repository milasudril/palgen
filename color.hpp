#ifndef PALGEN_COLOR_HPP
#define PALGEN_COLOR_HPP

#include <cmath>
#include <algorithm>
#include <type_traits>
#include <format>

namespace palgen
{
	struct linear_tag{};

	template<class DomainTag>
	struct color
	{
		float r;
		float g;
		float b;

		static constexpr auto is_linear = std::is_same_v<DomainTag, linear_tag>;

		friend constexpr auto operator+(color lhs, color rhs) requires is_linear
		{ return color{lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b}; }

		friend constexpr auto operator-(color lhs, color rhs) requires is_linear
    { return color{lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b }; }

		constexpr color& operator+=(color rhs) requires is_linear
		{
			r += rhs.r;
			g += rhs.g;
			b += rhs.b;
			return *this;
		}

		constexpr color& operator-=(color rhs) requires is_linear
		{
			r -= rhs.r;
			g -= rhs.g;
			b -= rhs.b;
			return *this;
		}

		friend constexpr auto operator*(color c, float scalar)
		{ return color{c.r*scalar, c.g*scalar, c.b*scalar }; }

		friend constexpr auto operator*(float scalar, color c)
		{ return c*scalar; }

		friend constexpr auto operator/(color c, float scalar)
		{ return color{c.r/scalar, c.g/ scalar, c.b/scalar }; }

		constexpr color& operator*=(float scalar)
		{
			r *= scalar;
			g *= scalar;
			b *= scalar;
			return *this;
		}

		constexpr color& operator/=(float scalar)
		{
			r /= scalar;
			g /= scalar;
			b /= scalar;
			return *this;
		}

		constexpr auto operator+() const
		{ return *this; }

		constexpr auto operator-() const
    { return -1.0f*(*this); }

    constexpr bool operator==(color const& c) const = default;
	};

	template<class DomainTag>
	inline std::string to_string(color<DomainTag> c)
	{ return std::format("unknown_rgb({}, {}, {})", c.r, c.g, c.b); }

	template<class DomainTag>
	inline constexpr auto largest_component(color<DomainTag> c)
	{ return std::max(std::max(c.r, c.g), c.b); }

	using linear_color = color<linear_tag>;

	inline constexpr linear_color white{
		.r = 1.0f,
		.g = 1.0f,
		.b = 1.0f
	};

	inline std::string to_string(linear_color c)
	{ return std::format("linrgb({}, {}, {})", c.r, c.g, c.b); }

	inline constexpr linear_color black{};

	struct srgb_tag{};

	constexpr float linear_to_srgb(float x)
	{
		return x <= 0.00031308f? 12.92f*x: (x == 1.0f? 1.0f : 1.055f*std::pow(x, 1.0f/2.4f) - 0.055f);
	}

	template<class TargetTag>
	constexpr color<TargetTag> convert(linear_color input) = delete;

	template<>
	constexpr color<srgb_tag> convert<srgb_tag>(linear_color input)
	{
		return color<srgb_tag>{
			.r = linear_to_srgb(input.r),
			.g = linear_to_srgb(input.g),
			.b = linear_to_srgb(input.b)
		};
	}

	inline std::string to_string(color<srgb_tag> c)
	{
		return std::format(
			"#{:02X}{:02X}{:02X}",
			static_cast<int>(255.0f*c.r),
			static_cast<int>(255.0f*c.g),
			static_cast<int>(255.0f*c.b)
		);
	}

	struct intensity_weights
	{
    constexpr explicit intensity_weights(float r, float g, float b):
        r{r/(r + g + b)},
        g{g/(r + g + b)},
        b{b/(r + g + b)}
    {}

    float const r;
    float const g;
    float const b;
	};

	template<intensity_weights Weights>
	class intensity
	{
	public:
		static constexpr auto weights = Weights;

		constexpr explicit intensity(float value):
			m_value{value}
		{}

		constexpr explicit intensity(linear_color color):
			m_value{weights.r*color.r + weights.g*color.g + weights.b*color.b}
		{}

		constexpr operator float() const
		{ return m_value; }

		constexpr auto operator<=>(intensity const&) const = default;

		friend constexpr auto operator*(intensity i, float scalar)
		{ return intensity{i.m_value*scalar}; }

		friend constexpr auto operator*(float scalar, intensity i)
		{ return i*scalar; }

		friend constexpr auto operator/(intensity i, float scalar)
		{ return intensity{i.m_value/scalar}; }

	private:
		float m_value;
	};

	template<intensity_weights Weights>
	std::string to_string(intensity<Weights> i)
	{ return std::format("{}", static_cast<float>(i)); }

	using perceptual_color_intensity = intensity<
		intensity_weights{
			0.5673828125f,
			1.0f,
			0.060546875f
		}
	>;

	using unweighted_intensity = intensity<
		intensity_weights{
			1.0f,
			1.0f,
			1.0f
		}
	>;

	template<intensity_weights Weights>
	constexpr linear_color brighten(linear_color input, intensity<Weights> target_intensity)
	{
		intensity<Weights> const input_intensity{input};
		auto const intensity_white = intensity<Weights>{white};
		target_intensity = std::clamp(target_intensity, input_intensity, intensity_white);
		auto const t  = (target_intensity - input_intensity)/(intensity_white - input_intensity);
		return t*white + (1.0f - t)*input;
	}

	template<intensity_weights Weights>
	constexpr linear_color normalize(linear_color input, intensity<Weights> target_intensity)
	{
		using intensity_type = intensity<Weights>;
		auto const tmp = input*(target_intensity/intensity_type{input});
		auto const maxval = largest_component(tmp);
		if(maxval > 1.0f)
		{ return brighten(tmp/maxval, target_intensity); }
		return tmp;
	}

	template<class IntensityType>
	constexpr linear_color desaturate(linear_color input, float amount)
	{
		IntensityType const i{input};
		auto const scaled_gray = i*white;
		return amount*scaled_gray + (1.0f - amount)*input;
	}
};

#endif
