#include "cell_grid.hpp"
#include "string_canvas.hpp"

#include <algorithm>
#include <random>

auto App::CellGrid::makeFromStringCanvas() -> CellGrid {
	CellGridKey currentLocation{0, 0};

	std::vector<CellGridKey> workerTargetLocations;

	CellGrid grid(10, 78);
	for (CellGridKey::Value rowIndex{0}; rowIndex < grid.getRowCount(); ++rowIndex) {
		for (CellGridKey::Value columnIndex{0}; columnIndex < grid.getColumnCount(); ++columnIndex) {
			CellGridKey::Value const flatIndex{
				/* leading newline */1 + rowIndex * (grid.getColumnCount() + /* trailing newline */1) + columnIndex
			};

			switch (stringCanvas.at(flatIndex)) {
				case 'x':
					workerTargetLocations.push_back({rowIndex, columnIndex});
					break;
			}
		}
	}

	std::mt19937 randomNumberGenerator((std::random_device()()));
	std::shuffle(workerTargetLocations.begin(), workerTargetLocations.end(), randomNumberGenerator);

	grid.addWorkerCellTargets(workerTargetLocations);
	return grid;
}
