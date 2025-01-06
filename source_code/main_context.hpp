#pragma once

#include "cell_grid.hpp"
#include "SDL.h"

namespace App::MainContext {
	extern SDL_Window *window;
	inline constexpr int windowWidth{800}, windowHeight{600};
	extern SDL_Renderer *renderer;
	extern Uint8 const *keyboardState;

	void initialize();
	auto drawCellGrid(CellGrid const &cellGrid) -> void;

	void drawQuadrilateral(
		SDL_FPoint const &northwestPoint, SDL_FPoint const &northeastPoint,
		SDL_FPoint const &southwestPoint, SDL_FPoint const &southeastPoint,
		SDL_Color  const &northwestColor, SDL_Color  const &northeastColor,
		SDL_Color  const &southwestColor, SDL_Color  const &southeastColor
	);

	Uint64 updateDeltaTime();
}
