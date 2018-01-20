#pragma once
#include "stdafx.h"

// VECTOR
namespace geo
{
	template <typename T, size_t N, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
	struct vector {
		template <typename ...R>
		vector(const R&&... args) : elements{ static_cast<T>(args)... } {}

		vector(vector&& args) = default;
		vector(const vector& args)
		{
			this->elements = args.elements;
		}

		vector& operator=(const vector& args)
		{
			this->elements = args.elements;
			return *this;
		}

		// OVERLOADS
		template <typename T, typename my_type, size_t N, size_t my_size>
		using common_vector_t = vector<std::common_type_t<T, my_type>, std::max(N, my_size)>;

		template <typename my_type, size_t my_size>
		constexpr auto operator +(const vector<my_type, my_size>& other) -> common_vector_t<T, my_type, N, my_size>
		{
			common_vector_t<T, my_type, N, my_size> vec = *this;

			for (std::size_t i = 0; i < std::min(N, my_size); ++i)
				vec.elements[i] = (i >= this->elements.size() ? 0 : this->elements[i]) + (i >= other.elements.size() ? 0 : other.elements[i]);

			return vec;
		}
		constexpr auto operator +(T arg) -> vector<T, N>
		{
			auto vec = *this;

			for (std::size_t i = 0; i < N; ++i)
				vec.elements[i] += arg;

			return vec;
		}
		template <typename my_type, size_t my_size>
		constexpr auto operator -(const vector<my_type, my_size>& other) -> common_vector_t<T, my_type, N, my_size>
		{
			common_vector_t<T, my_type, N, my_size> vec = *this;

			for (std::size_t i = 0; i < std::min(N, my_size); ++i)
				vec.elements[i] = (i >= this->elements.size() ? 0 : this->elements[i]) - (i >= other.elements.size() ? 0 : other.elements[i]);

			return vec;
		}
		constexpr auto operator -(T arg) -> vector<T, N>
		{
			auto vec = *this;

			for (std::size_t i = 0; i < N; ++i)
				vec.elements[i] -= arg;

			return vec;
		}
		template <typename my_type, size_t my_size>
		constexpr auto operator *(const vector<my_type, my_size>& other) -> common_vector_t<T, my_type, N, my_size>
		{
			common_vector_t<T, my_type, N, my_size> vec = *this;

			for (std::size_t i = 0; i < std::min(N, my_size); ++i)
				vec.elements[i] = (i >= this->elements.size() ? 0 : this->elements[i]) * (i >= other.elements.size() ? 0 : other.elements[i]);

			return vec;
		}
		template <typename my_type, size_t my_size>
		constexpr auto operator *=(const vector<my_type, my_size>& other) -> vector<T, N>
		{
			*this = *this * other;
			return *this;
		}
		constexpr auto operator *(T arg) -> vector<T, N>
		{
			auto vec = *this;

			for (std::size_t i = 0; i < N; ++i)
				vec.elements[i] *= arg;

			return vec;
		}
		constexpr auto operator *=(T arg) -> vector<T, N>
		{
			*this = *this * arg;
			return *this;
		}

		template <typename my_type, size_t my_size>
		constexpr auto operator /(const vector<my_type, my_size>& other) -> common_vector_t<T, my_type, N, my_size>
		{
			common_vector_t<T, my_type, N, my_size> vec = *this;

			for (std::size_t i = 0; i < std::min(N, my_size); ++i)
				vec.elements[i] = (i >= this->elements.size() ? 1 : this->elements[i]) / (i >= other.elements.size() ? 1 : other.elements[i]);

			return vec;
		}
		constexpr auto operator /(T arg) -> vector<T, N>
		{
			auto vec = *this;

			for (std::size_t i = 0; i < N; ++i)
				vec.elements[i] /= arg;

			return vec;
		}
		constexpr auto operator /=(T arg) -> vector<T, N>
		{
			for (std::size_t i = 0; i < N; ++i)
				this->elements[i] /= arg;

			return *this;
		}

		// HELPERS
		template <typename = std::enable_if_t<N >= 1>>
		auto x() -> T&
		{
			return elements[0];
		}
		template <typename = std::enable_if_t<N >= 2>>
		auto y() -> T&
		{
			return elements[1];
		}
		template <typename = std::enable_if_t<N >= 3>>
		auto z() -> T&
		{
			return elements[2];
		}
		template <typename = std::enable_if_t<N >= 4>>
		auto w() -> T&
		{
			return elements[3];
		}
		
		// MATHEMATICAL HELPERS
		constexpr auto length() -> double
		{
			T tmp{};
			std::for_each(this->elements.begin(), this->elements.end(), [&tmp](T x) { tmp += x * x; });

			return sqrt(tmp);
		};
		constexpr auto distance(const geo::vector<T, N>& other) -> double
		{
			T tmp{};

			for (size_t i = 0; i < N; i++)
				tmp += (this->elements[i] - other.elements[i]) * (this->elements[i] - other.elements[i]);

			return sqrt(tmp);
		}
		constexpr auto mid_point(const geo::vector<T, N>& other) -> geo::vector<T, N>
		{
			vector<T, N> vec;

			for (size_t i = 0; i < N; i++)
				vec.elements[i] = (this->elements[i] + other.elements[i]) / 2;

			return vec;
		}
		constexpr auto dot_product(const geo::vector<T, N>& other) -> double
		{
			auto product = 0.0;

			for (size_t i = 0; i < N; i++)
				product += this->elements[i] * other.elements[i];

			return product;
		}
		constexpr auto cross_product_2d(geo::vector<T, N>& other) -> double
		{
			return (this->x() * other.y()) - (other.x() * this->y());
		}
		constexpr auto formula(geo::vector<T, N>& other) -> std::string
		{
			auto midpoint = this->mid_point(other);

			auto delta = other - *this;
			auto slope = 1.0 / -(delta.y() / delta.x());

			std::cout << slope << std::endl;

			return "";
		}

		private:
			std::array<T, N> elements{};
	};

	template <typename T>
	struct polygon {

		template <typename ...R>
		polygon(R&&... args) : elements{ args... } {}

		polygon(polygon&& args) = default;
		polygon(polygon& args) = default;

		polygon& operator=(const polygon& args)
		{
			this->elements = args.elements;
			return *this;
		}

		// MATHEMATICAL HELPERS
		constexpr auto midpoints() -> std::vector<T>
		{
			std::vector<T> pts;

			for (size_t i = 0; i < elements.size(); i++)
				pts.emplace_back(this->elements.at(i).mid_point(this->elements.at((i + 1) % elements.size())));

			return pts;
		}
		constexpr auto sides() -> std::vector<double>
		{
			std::vector<double> side_vec;

			for (size_t i = 0; i < elements.size(); i++)
				side_vec.emplace_back(this->elements.at(i).distance(this->elements.at((i + 1) % elements.size())));

			return side_vec;
		}
		constexpr auto side_formulas() -> std::vector<std::string>
		{
			std::vector<std::string> result;

			for (size_t i = 0; i < elements.size(); i++)
			{
				auto current_point = this->elements.at(i);
				auto next_point = this->elements.at((i + 1) % elements.size());

				result.emplace_back(current_point.formula(next_point));
			}
			
			return result;
		}
		constexpr auto area() -> double
		{
			auto result = 0.0;
			auto previous_point = this->elements.at(this->elements.size() - 1);
			for (size_t i = 0; i < elements.size(); i++)
			{
				auto current_point = this->elements.at(i);

				// SHOELACE FORMULA
				result += (previous_point.x() + current_point.x()) * (previous_point.y() - current_point.y());

				previous_point = current_point;

			}
			return abs(result) / 2;
		}
		constexpr auto circumference() -> double
		{	
			return std::accumulate(this->sides().begin(), this->sides().end(), 0);
		}

		constexpr auto angles() -> std::vector<double>
		{
			std::vector<double> angles;

			for (size_t i = 0; i < elements.size(); i++)
			{
				polygon<T> new_triangle{ this->elements[i], this->elements[(i + 1) % elements.size()], this->elements[(i + 2) % elements.size()] };

				const auto side_a = new_triangle.sides()[0], side_b = new_triangle.sides()[1], side_c = new_triangle.sides()[2];

				const auto cos0 = (pow(side_a, 2) + pow(side_b, 2) - pow(side_c, 2)) / (2 * side_a * side_b);
				angles.emplace_back(acos(cos0) * 180 / M_PI);
			}

			return angles;
		}
		
		// TRIANGLES ONLY
		constexpr auto triangle_medians() -> std::vector<double>
		{
			std::vector<double> medians;

			if (elements.size() != 3)
				return medians;

			for (size_t i = 0; i < elements.size(); i++)
			{
				auto side_ab = this->sides().at(i);
				auto side_bc = this->sides().at((i + 1) % elements.size());
				auto side_ac = this->sides().at((i + 2) % elements.size());

				// AM^2 = AB^2/2 + AC^2/2 - BC^2/4
				auto am = sqrt((pow(side_ab, 2)) / 2 + (pow(side_ac, 2)) / 2 - (pow(side_bc, 2)) / 4);

				medians.emplace_back(am);
			}

			return medians;
		}

	private:
		std::vector<T> elements{};
	};
}

using vector2i = geo::vector<int32_t, 2>;
using polyi = geo::polygon<vector2i>;
using vector2d = geo::vector<double, 2>;
using polyd = geo::polygon<vector2d>;