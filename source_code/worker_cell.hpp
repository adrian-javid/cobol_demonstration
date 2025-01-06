#pragma once

#include "cell.hpp"

namespace App {class WorkerCell final : public Cell {
	private:
		CellGridKey targetLocation;

		[[nodiscard]]
		Request makeRequest(CellGridKey const &currentLocation) override;
};}
