#pragma once

#include "cell.hpp"

namespace App {class WorkerCell final : public Cell {
	public:
		[[nodiscard]]
		inline explicit WorkerCell(CellGridKey const targetLocationParam):
			targetLocation(targetLocationParam)
		{}

	private:
		CellGridKey targetLocation;

		[[nodiscard]]
		Request makeRequest(CellGridKey const &currentLocation) override;
};}
