#pragma once

#include "vector2.hpp"
#include "direction.hpp"
#include <stdexcept>

namespace App {class Cell {
	public:
		struct Request {
			Direction movement;
		};

		enum struct Color : std::uint_least8_t { red, blue };

		[[nodiscard]]
		inline Request const & update() {
			this->color = getNextColor(this->color);
			return updateRequest();
		}

		[[nodiscard]]
		inline Color getColor() const { return this->color; }

		[[nodiscard]]
		static constexpr Color getFirstColor() { return Color::red; }

		[[nodiscard]]
		static constexpr Color getNextColor(Color const color) {
			switch (color) {
				using enum Color;
				case red: return blue;
				case blue: return red;
			}

			throw std::logic_error("Invalid color.");
		}

		virtual ~Cell() = default;

	protected:
		virtual Request const & updateRequest() = 0;

	private:
		Color color{getFirstColor()};
};}
