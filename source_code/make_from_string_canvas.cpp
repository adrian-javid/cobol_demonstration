#include "cell_grid.hpp"
#include "string_canvas.hpp"

#include <algorithm>
#include <random>

auto App::CellGrid::makeFromStringCanvas() -> CellGrid {
	CellGridKey currentLocation{0, 0};

	std::vector<CellGridKey> workerTargetLocations;

	static constexpr std::size_t rowCount{[]() constexpr -> std::size_t {
		std::size_t newlineCount{0};
		for (char const value : stringCanvas) if (value == '\n') ++newlineCount;

		if (newlineCount == 0) throw std::logic_error("There should be at least the leading newline.");

		return newlineCount - 1;
	}()};

	static constexpr std::size_t columnCount{[]() constexpr -> std::size_t {
		std::size_t spaceCount{0};
		for (
			auto iterator = stringCanvas.begin() + 1;
			iterator != stringCanvas.end() and *iterator != '\n';
			++iterator, ++spaceCount
		);

		return spaceCount;
	}()};

	CellGrid grid(rowCount, columnCount);
	for (CellGridKey::Value rowIndex{0}; rowIndex < grid.getRowCount(); ++rowIndex) {
		for (CellGridKey::Value columnIndex{0}; columnIndex < grid.getColumnCount(); ++columnIndex) {
			std::size_t const flatIndex{
				/* leading newline */std::size_t{1} + static_cast<std::size_t>(rowIndex) * (grid.getColumnCount() + /* trailing newline */std::size_t{1}) + static_cast<std::size_t>(columnIndex)
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
