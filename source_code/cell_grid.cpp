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
			auto const &celllist = (*this)[{rowIndex, columnIndex}];
			// Cell::Request const &request = cellPointer->getRequest();
		}
	}
}

auto App::CellGrid::operator[](CellGridKey const &key) -> decltype(table)::value_type & {
	std::size_t const flatIndex{getFlatIndex(key, this->columnCount)};
	return this->table[flatIndex];
}
