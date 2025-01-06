#pragma once

#include "cell.hpp"

#include <vector>
#include <unordered_set>
#include <memory>
#include "vector2.hpp"

namespace App {class CellGrid final {
	public:
		[[nodiscard]]
		explicit CellGrid() = delete;

		[[nodiscard]]
		explicit CellGrid(std::size_t const rowCount, std::size_t const columnCount);

		void update();

		[[nodiscard]]
		static constexpr std::size_t getFlatIndex(CellGridKey const &key, std::size_t const columnCount) {
			auto const [rowIndex, columnIndex]{key};
			return rowIndex * columnCount + columnIndex;
		}

	private:
		std::vector<std::unordered_set<std::unique_ptr<Cell>>> table;
		std::size_t rowCount{}, columnCount{};
		Cell::Color currentCellColor{Cell::getFirstColor()};

		[[nodiscard]]
		decltype(table)::value_type & operator[](CellGridKey const &key);
};}
