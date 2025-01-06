#pragma once

#include "cell.hpp"

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <memory>
#include <variant>
#include "vector2.hpp"

#include "player_cell.hpp"
#include "worker_cell.hpp"

namespace App {class CellGrid final {
	public:

		[[nodiscard]]
		explicit CellGrid() = delete;

		[[nodiscard]]
		explicit CellGrid(std::size_t const rowCount, std::size_t const columnCount);

		[[nodiscard]]
		static CellGrid makeFromStringCanvas();
		inline std::size_t getRowCount() const { return this->rowCount; }

		inline std::size_t getColumnCount() const {return this->columnCount; }

		void update();

		void putNewWorkerCell(CellGridKey const &workerStartLocation, CellGridKey const &workerCellTargetLocation);

		[[nodiscard]]
		static constexpr std::size_t getFlatIndex(CellGridKey const &key, std::size_t const columnCount) {
			auto const [rowIndex, columnIndex]{key};
			return rowIndex * columnCount + columnIndex;
		}

		enum struct CellIdentifier : std::size_t { player = 0 };
		using TableT = std::vector<std::unordered_set<CellIdentifier>>;

		[[nodiscard]]
		TableT::value_type const & operator[](CellGridKey const &key) const;

	private:
		TableT table;
		std::unordered_map<CellIdentifier, WorkerCell> workerCellRegistry;
		PlayerCell playerCell;
		std::size_t rowCount{}, columnCount{};
		std::underlying_type_t<CellIdentifier> availableWorkerCellIdentifierValue{1};
		Cell::Color currentCellColor{Cell::getFirstColor()};

		[[nodiscard]]
		decltype(table)::value_type & operator[](CellGridKey const &key);

		[[nodiscard]]
		Cell & getCell(CellIdentifier const cellIdentifier);
};}
