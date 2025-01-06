#include "cell_grid.hpp"

#include <utility>
#include "player_cell.hpp"

App::CellGrid::CellGrid(
	std::size_t const rowCountParam,
	std::size_t const columnCountParam
):
	table(rowCountParam * columnCountParam),
	rowCount{rowCountParam},
	columnCount{columnCountParam}
{
	(*this)[{5, 5}].emplace(CellIdentifier::player);
}

void App::CellGrid::setPlayerMovement(Direction const direction) {
	this->playerCell.movement = direction;
}

auto App::CellGrid::putNewWorkerCell(CellGridKey const &workerStartLocation, CellGridKey const &workerCellTargetLocation) -> void {
	CellIdentifier const cellIdentifier{availableWorkerCellIdentifierValue++};
	this->workerCellRegistry.try_emplace(cellIdentifier, workerCellTargetLocation);
	(*this)[workerStartLocation].emplace(cellIdentifier);
}

void App::CellGrid::update() {
	for (std::size_t rowIndex{0}; rowIndex < this->rowCount; ++rowIndex) {
		for (std::size_t columnIndex{0}; columnIndex < this->columnCount; ++columnIndex) {
			CellGridKey const currentLocation{
				static_cast<CellGridKey::Value>(rowIndex),
				static_cast<CellGridKey::Value>(columnIndex)
			};

			auto &cellGroup = (*this)[currentLocation];

			for (
				auto cellIdentifierIterator = cellGroup.begin();
				cellIdentifierIterator != cellGroup.end();
			) {
				CellIdentifier const cellIdentifier(*cellIdentifierIterator);
				Cell &cell = this->getCell(cellIdentifier);

				if (cell.getColor() == this->currentCellColor) {
					Cell::Request const request = cell.update(currentLocation);
					
					if (request.movement != Direction::none) {
						CellGridKey const targetLocation(currentLocation + CellGridKey::getDirection(request.movement));
						(*this)[targetLocation].emplace(cellIdentifier);
						cellIdentifierIterator = cellGroup.erase(cellIdentifierIterator);
					} else {
						++cellIdentifierIterator;
					}

					continue;
				}

				++cellIdentifierIterator;
			}

		}
	}

	this->currentCellColor = Cell::getNextColor(this->currentCellColor);
}

App::CellGrid::TableT::value_type const & App::CellGrid::operator[](CellGridKey const &key) const {
	static constexpr auto wrapValue = [](
		CellGridKey::Value value, std::size_t const length
	) constexpr -> CellGridKey::Value {
		value %= length;
		if (value < 0) value += length;
		return value;
	};

	CellGridKey wrappedKey{
		wrapValue(key.value0, this->getRowCount()),
		wrapValue(key.value1, this->getColumnCount())
	};

	std::size_t const flatIndex{getFlatIndex(wrappedKey, this->columnCount)};
	return this->table.at(flatIndex);
}

App::CellGrid::TableT::value_type & App::CellGrid::operator[](CellGridKey const &key) {
	return const_cast<App::CellGrid::TableT::value_type &>(std::as_const(*this)[key]);
}

auto App::CellGrid::getCell(CellIdentifier const cellIdentifier) -> Cell & {
	switch (cellIdentifier) {
		case CellIdentifier::player:
			return this->playerCell;
		default:
			return this->workerCellRegistry.at(cellIdentifier);
	}
}
