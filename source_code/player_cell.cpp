#include "player_cell.hpp"

auto App::PlayerCell::makeRequest(CellGridKey const &currentLocation) -> Request {
	return Request{.movement=this->movement};
}
