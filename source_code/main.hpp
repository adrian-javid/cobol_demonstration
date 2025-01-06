#pragma once

#include "cell_grid.hpp"
#include "timer.hpp"
#include "main_context.hpp"

namespace App::MainContext {
	extern CellGrid cellGrid;
	extern Timer cellGridUpdateTimer;
}

extern "C" int App_mainLoop(void);

extern "C" {
	void App_processEvents(void);

	void App_processKeyboardInput(void);

	void App_updateCellGrid(void);

	void App_drawCellGrid(void);

	void App_renderCanvas(void);
}
