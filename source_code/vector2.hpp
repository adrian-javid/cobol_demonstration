#pragma once

#include "direction.hpp"
#include <array>
#include <cstdint>

namespace App {
	template <typename ValueT>
	class Vector2 {
		public:
			static_assert(std::is_signed_v<ValueT>);
			using Value = ValueT;

			Value value0{}, value1{};

			[[nodiscard]]
			constexpr Vector2() = delete;

			[[nodiscard]]
			constexpr Vector2(Value const firstValue, Value const secondValue):
				value0{firstValue}, value1{secondValue}
			{}

			[[nodiscard]]
			constexpr Vector2 operator+(Vector2 const &vector) const {
				return {this->value0 + vector.value0, this->value1 + vector.value1};
			}

			[[nodiscard]]
			constexpr Vector2 operator-(Vector2 const &vector) const {
				return {this->value0 - vector.value0, this->value1 - vector.value1};
			}

			[[nodiscard]]
			static constexpr Vector2 getDirection(Direction const direction) {
				switch (direction) {
					using enum Direction;

					/**/                              case north: return {-1, 0};
					/**/
					/**/ case west: return {0, -1};   case none: return  { 0, 0};   case east: return {0, +1};
					/**/
					/**/                              case south: return {+1, 0};
				}

				throw std::logic_error("Invalid direction.");
			}
	};

	using CellGridKey = Vector2<std::int_fast32_t>;
}
