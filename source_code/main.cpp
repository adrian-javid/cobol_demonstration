#include "main.hpp"

#ifndef EMSCRIPTEN
#include <libcob.h>
#else
#include "libcob.h"
#endif


#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#ifdef __EMSCRIPTEN_PTHREADS__
#error "Should not use POSIX threads for this program."
#endif
#endif

#include <iostream>

namespace App::MainContext {
	CellGrid cellGrid(CellGrid::makeFromStringCanvas());
	Timer cellGridUpdateTimer(Timer::oneSecond / 64u);
}

int App_processEvents(void) {
	for (SDL_Event event; SDL_PollEvent(&event);) switch (event.type) {
		case SDL_KEYDOWN: switch (event.key.keysym.sym) {
			case SDLK_BACKQUOTE:
				break;
			case SDLK_ESCAPE:
				break;
		} break;
		case SDL_WINDOWEVENT: switch (event.window.event) {
			case /* (user initiated resize) */SDL_WINDOWEVENT_RESIZED:
			case SDL_WINDOWEVENT_SIZE_CHANGED:
				static_cast<void>(event.window.data1);
				static_cast<void>(event.window.data2);
				break;
		} break;
		case SDL_QUIT:
			std::exit(EXIT_SUCCESS);
			break;
	}

	return {};
}

int App_processKeyboardInput(void) {
	App::CellGridKey directionVector{0, 0};
	if (App::MainContext::keyboardState[SDL_SCANCODE_UP   ]) directionVector.value0 -= 1;
	if (App::MainContext::keyboardState[SDL_SCANCODE_DOWN ]) directionVector.value0 += 1;
	if (App::MainContext::keyboardState[SDL_SCANCODE_LEFT ]) directionVector.value1 -= 1;
	if (App::MainContext::keyboardState[SDL_SCANCODE_RIGHT]) directionVector.value1 += 1;

	App::Direction direction{App::Direction::none};
	if (std::abs(directionVector.value0) > 0) {
		if (directionVector.value0 < 0) direction = App::Direction::north;
		else direction = App::Direction::south;
	} else if (std::abs(directionVector.value1) > 0) {
		if (directionVector.value1 < 0) direction = App::Direction::west;
		else direction = App::Direction::east;
	}

	App::MainContext::cellGrid.setPlayerMovement(direction);

	return {};
}

int App_updateCellGrid(void) {
	Uint64 const deltaTimeMilliseconds{App::MainContext::updateDeltaTime()};
	if (App::MainContext::cellGridUpdateTimer.update(deltaTimeMilliseconds)) App::MainContext::cellGrid.update();
	return {};
}

int App_drawCellGrid(void) {
	App::MainContext::drawCellGrid(App::MainContext::cellGrid);
	return {};
}

int App_renderCanvas(void) {
	SDL_RenderPresent(App::MainContext::renderer);
	return {};
}

int main(int const argCount, char **const argList) {
	cob_init(argCount, argList);
	App::MainContext::initialize();
	std::atexit(+[]() -> void { cob_tidy(); });

	#ifdef __EMSCRIPTEN__
	/*
		Notify JavaScript that exported application functions can be called.
		Note: only use single quotes `'` instead of double quotes `"` when using `EM_ASM` macro.
	*/
	if (int const success{
			EM_ASM_INT(
				{
					if (typeof onApplicationInitialized === 'function') {
						onApplicationInitialized();
						return true; // success
					} else {
						return false; // failure
					}
				},
				/* no arguments */
			)
		};
		not success
	) {
		std::cerr <<
			"JavaScript function `onApplicationInitialized` was not called successfully. "
			"Check if it's defined.\n";
	}
	#endif

	#ifdef __EMSCRIPTEN__
	emscripten_set_main_loop(+[]() -> void { App_mainLoop(); }, -1, true);
	#else
	while (true) App_mainLoop();
	#endif

	return EXIT_SUCCESS;
}
