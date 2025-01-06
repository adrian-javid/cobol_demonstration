#include "main_context.hpp"

#include <stdexcept>

SDL_Window *App::MainContext::window = nullptr;
SDL_Renderer *App::MainContext::renderer = nullptr;

auto App::MainContext::initialize() -> void {
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		throw std::logic_error("SDL failed to initialize.");
	}

	std::atexit(+[]() -> void {
		// Free the renderer before freeing the associated window.
		if (renderer) SDL_DestroyRenderer(renderer);
		if (window) SDL_DestroyWindow(window);

		SDL_Quit();
	});

	window = SDL_CreateWindow(
		"COBOL Demonstration",
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
		windowWidth, windowHeight,
		SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
	);

	renderer = SDL_CreateRenderer(
		window,
		/* initialize the first renderer supporting the requested flags */ int{-1},
		#ifdef __linux__
			#ifdef __EMSCRIPTEN__
				#error The Emscripten build does not have to explicitly use a software renderer.
			#endif
			SDL_RENDERER_SOFTWARE
		#else
			SDL_RENDERER_PRESENTVSYNC
		#endif
	);

	if (renderer == nullptr)
		renderer = SDL_CreateRenderer(window, -1, 0u);
}

auto App::MainContext::drawCellGrid(CellGrid const &cellGrid) -> void {
	int const cellWidth = windowWidth / cellGrid.getColumnCount();
	int const cellHeight = windowWidth / cellGrid.getRowCount();

	for (std::size_t rowIndex{0}; rowIndex < cellGrid.getRowCount(); ++rowIndex) {
		for (std::size_t colIndex{0}; colIndex < cellGrid.getColumnCount(); ++colIndex) {

			SDL_Rect const rectangle = {
				colIndex * cellWidth, rowIndex * cellHeight,
				cellWidth,
				cellHeight
			};

			auto const &cellGroup = cellGrid[{rowIndex, colIndex}];

			if (cellGroup.size() > 0) {
				SDL_SetRenderDrawColor(renderer, 64, 224, 208, 255);
			} else {
				SDL_SetRenderDrawColor(renderer, 210, 180, 140, 255);
			}

			// Draw a filled rectangle
			SDL_RenderFillRect(renderer, &rectangle);
		}
	}
}
