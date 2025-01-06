#include "cell_grid.hpp"
#include "string_canvas.hpp"

#include <algorithm>
#include <random>

auto App::CellGrid::makeFromStringCanvas() -> CellGrid {
	CellGridKey currentLocation{0, 0};

	std::vector<CellGridKey> workerTargetLocations;

	for (
		auto valueIterator = stringCanvas.cbegin() + 1;
		valueIterator != stringCanvas.cend();
		++valueIterator
	) {
		char const value{*valueIterator};
		switch (value) {
			case 'x':
				workerTargetLocations.push_back(currentLocation);
				break;
			case '\n':
				currentLocation.value0 = -1;
				++currentLocation.value1;
				break;
		}

		++currentLocation.value0;
	}

	std::mt19937 randomNumberGenerator((std::random_device()()));

	std::shuffle(workerTargetLocations.begin(), workerTargetLocations.end(), randomNumberGenerator);

	CellGrid grid(10, 78);
	return grid;
}
