#include "player_cell.hpp"

auto App::PlayerCell::makeRequest(CellGridKey const &currentLocation) -> Request {
	switch (this->movement) {
		case Direction::none:
			return Request{.movement=this->movement};
		default:
			return Request{.movement=this->movement, .createWorkerCell=true};
	}
}
