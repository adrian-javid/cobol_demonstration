#pragma once

#include "cell.hpp"

namespace App {class PlayerCell final : public Cell {
	private:
		Direction movement{Direction::none};

		[[nodiscard]]
		Request makeRequest(CellGridKey const &currentLocation) override;
};}
