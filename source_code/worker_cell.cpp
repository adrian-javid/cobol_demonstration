#include "worker_cell.hpp"

auto App::WorkerCell::makeRequest(CellGridKey const &currentLocation) -> Request {
	if (currentLocation == targetLocation) return Request{.movement=Direction::none};

	CellGridKey const directionVector(targetLocation - currentLocation);

	Direction const direction{[
		rowIndex=directionVector.value0,
		columnIndex=directionVector.value1
	]() constexpr -> Direction {
		if (std::abs(columnIndex) >= std::abs(rowIndex)) {
			if (columnIndex < 0) return Direction::west;
			else return Direction::east;
		} else {
			if (rowIndex < 0) return Direction::north;
			else return Direction::south;
		}
	}()};

	return Request{.movement=direction};
}
