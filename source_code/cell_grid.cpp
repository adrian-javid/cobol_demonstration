#include "cell_grid.hpp"

App::CellGrid::CellGrid(
	std::size_t const rowCountParam,
	std::size_t const columnCountParam
):
	table(rowCountParam * rowCountParam),
	rowCount{rowCountParam},
	columnCount{columnCountParam}
{}

void App::CellGrid::update() {
	for (std::size_t rowIndex{0}; rowIndex < this->rowCount; ++rowIndex) {
		for (std::size_t columnIndex{0}; columnIndex < this->columnCount; ++columnIndex) {
			CellGridKey const currentLocation{
				static_cast<CellGridKey::Value>(rowIndex),
				static_cast<CellGridKey::Value>(columnIndex)
			};

			auto &cellGroup = (*this)[currentLocation];

			for (
				auto cellPointerIterator = cellGroup.begin();
				cellPointerIterator != cellGroup.end();
			) {
				auto const &cellPointer = *cellPointerIterator;
				if (cellPointer == nullptr) throw std::logic_error("Null cell pointer.");

				if (cellPointer->getColor() == this->currentCellColor) {
					Cell::Request const &request = cellPointer->update();
					
					if (request.movement != Direction::none) {
						CellGridKey const targetLocation(currentLocation + CellGridKey::getDirection(request.movement));
						(*this)[targetLocation].emplace(cellPointer);
						cellPointerIterator = cellGroup.erase(cellPointerIterator);
					} else {
						++cellPointerIterator;
					}

					continue;
				}

				++cellPointerIterator;
			}

		}
	}

	this->currentCellColor = Cell::getNextColor(this->currentCellColor);
}

auto App::CellGrid::operator[](CellGridKey const &key) -> decltype(table)::value_type & {
	std::size_t const flatIndex{getFlatIndex(key, this->columnCount)};
	return this->table[flatIndex];
}
