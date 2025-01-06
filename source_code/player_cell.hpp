#pragma once

#include "cell.hpp"

namespace App {class PlayerCell final : public Cell {
	public:
		[[nodiscard]]
		inline explicit PlayerCell() {};

		Direction movement{Direction::none};

	private:
		[[nodiscard]]
		Request makeRequest(CellGridKey const &currentLocation) override;
};}
