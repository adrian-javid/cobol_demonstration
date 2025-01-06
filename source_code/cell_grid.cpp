#include "cell_grid.hpp"

#include "player_cell.hpp"

App::CellGrid::CellGrid(
	std::size_t const rowCountParam,
	std::size_t const columnCountParam
):
	table(rowCountParam * rowCountParam),
	rowCount{rowCountParam},
	columnCount{columnCountParam}
{}

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

auto App::CellGrid::operator[](CellGridKey const &key) -> decltype(table)::value_type & {
	std::size_t const flatIndex{getFlatIndex(key, this->columnCount)};
	return this->table[flatIndex];
}

[[nodiscard]]
auto App::CellGrid::getCell(CellIdentifier const cellIdentifier) -> Cell & {
	switch (cellIdentifier) {
		case CellIdentifier::player:
			return this->playerCell;
		default:
			return this->workerCellRegistry.at(cellIdentifier);
	}
}
