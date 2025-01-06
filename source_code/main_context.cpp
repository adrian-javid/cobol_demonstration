#include "main_context.hpp"

#include <stdexcept>
#include <cassert>

SDL_Window *App::MainContext::window = nullptr;
SDL_Renderer *App::MainContext::renderer = nullptr;
Uint8 const *App::MainContext::keyboardState = nullptr;

namespace App::MainContext {
	static Uint64 deltaTime{}, lastTime{};
}

Uint64 App::MainContext::updateDeltaTime() {
	Uint64 const thisTime{SDL_GetTicks64()};
	deltaTime = thisTime - lastTime;
	lastTime = thisTime;
	return deltaTime;
}

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
		SDL_WINDOW_SHOWN
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

	keyboardState = SDL_GetKeyboardState(nullptr);
}

void App::MainContext::drawQuadrilateral(
	SDL_FPoint const &northwestPoint, SDL_FPoint const &northeastPoint,
	SDL_FPoint const &southwestPoint, SDL_FPoint const &southeastPoint,
	SDL_Color  const &northwestColor, SDL_Color  const &northeastColor,
	SDL_Color  const &southwestColor, SDL_Color  const &southeastColor
) {
	static constexpr SDL_FPoint zeroPoint{0, 0};

	SDL_Vertex const topLeftVertex{northwestPoint, northwestColor, zeroPoint};
	SDL_Vertex const topRightVertex{northeastPoint, northeastColor, zeroPoint};
	SDL_Vertex const bottomLeftVertex{southwestPoint, southwestColor, zeroPoint};
	SDL_Vertex const bottomRightVertex{southeastPoint, southeastColor, zeroPoint};

	static constexpr int vertexCount{2 * (3)};
	std::array<SDL_Vertex, vertexCount> const vertexList{
		// Top left triangle.
		topLeftVertex, topRightVertex, bottomLeftVertex,

		// Bottom right triangle.
		topRightVertex, bottomLeftVertex, bottomRightVertex,
	};

	SDL_RenderGeometry(renderer, nullptr, vertexList.data(), vertexCount, nullptr, 0);
}

auto App::MainContext::drawCellGrid(CellGrid const &cellGrid) -> void {
	float const cellWidth = static_cast<float>(windowWidth) / static_cast<float>(cellGrid.getColumnCount());
	float const cellHeight = static_cast<float>(windowHeight) / static_cast<float>(cellGrid.getRowCount());

	for (std::size_t rowIndex{0}; rowIndex < cellGrid.getRowCount(); ++rowIndex) {
		for (std::size_t colIndex{0}; colIndex < cellGrid.getColumnCount(); ++colIndex) {
			assert(rowIndex >= 0 and rowIndex < cellGrid.getRowCount());
			assert(colIndex >= 0 and colIndex < cellGrid.getColumnCount());

			auto const &cellGroup = cellGrid[{
				static_cast<CellGridKey::Value>(rowIndex),
				static_cast<CellGridKey::Value>(colIndex)
			}];

			static constexpr SDL_Color playerColor{252, 3, 140, 0xFF};
			static constexpr SDL_Color workerColor{3, 19, 252, 0xFF};
			static constexpr SDL_Color backgroundColor{92, 96, 158, 0xFF};

			SDL_Color const rectangleColor([&cellGroup]() -> SDL_Color {
				if (cellGroup.empty()) {
					return backgroundColor;
				} else if (cellGroup.contains(CellGrid::CellIdentifier::player)) {
					return playerColor;
				} else {
					return workerColor;
				}
			}());

			SDL_FPoint const northwestCorner{
				/* x */ static_cast<float>(colIndex) * cellWidth,
				/* y */ static_cast<float>(rowIndex) * cellHeight,
			};

			SDL_FPoint const northeastCorner{northwestCorner.x + cellWidth, northwestCorner.y};

			SDL_FPoint const southwestCorner{northwestCorner.x, northwestCorner.y + cellHeight};

			SDL_FPoint const southeastCorner{northwestCorner.x + cellWidth, northwestCorner.y + cellHeight};

			drawQuadrilateral(
				northwestCorner, northeastCorner,
				southwestCorner, southeastCorner,
				rectangleColor, rectangleColor,
				rectangleColor, rectangleColor
			);
		}
	}
}
