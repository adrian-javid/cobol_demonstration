#pragma once

#include "cell_grid.hpp"
#include "SDL.h"

namespace App::MainContext {
	extern SDL_Window *window;
	inline constexpr int windowWidth{800}, windowHeight{600};
	extern SDL_Renderer *renderer;

	void initialize();
	auto drawCellGrid(CellGrid const &cellGrid) -> void;
}
