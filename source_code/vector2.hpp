#pragma once

#include <array>
#include <cstdint>

namespace App {
	template <typename ValueT>
	class Vector2 {
		public:
			ValueT value0{}, value1{};

			[[nodiscard]]
			constexpr Vector2() = delete;

			[[nodiscard]]
			constexpr Vector2(ValueT const firstValue, ValueT const secondValue):
				value0{firstValue}, value1{secondValue}
			{}
	};

	using CellGridKey = Vector2<std::int_fast32_t>;
}
