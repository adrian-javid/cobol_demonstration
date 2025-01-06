#pragma once

#include "vector2.hpp"

namespace App {class Cell {
	public:
		struct Request {
			CellGridKey targetPosition;
		};

		[[nodiscard]]
		Request const & update();
};}
