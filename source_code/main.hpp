#pragma once

#include "cell_grid.hpp"
#include "timer.hpp"
#include "main_context.hpp"

namespace App::MainContext {
	extern CellGrid cellGrid;
	extern Timer cellGridUpdateTimer;
}

namespace FromCobol {
	extern "C" int App_mainLoop(void);
}

extern "C" {
	int App_processEvents(void);

	int App_processKeyboardInput(void);

	int App_updateCellGrid(void);

	int App_drawCellGrid(void);

	int App_renderCanvas(void);
}
