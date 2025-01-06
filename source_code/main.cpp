
#ifndef EMSCRIPTEN
#include <libcob.h>
#else
#include "libcob.h"
#endif

#include "extern.hpp"
#include <iostream>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#ifdef __EMSCRIPTEN_PTHREADS__
#error "Should not use POSIX threads for this program."
#endif
#endif

#include "cell_grid.hpp"

#include "main_context.hpp"

namespace App {
	static CellGrid cellGrid(CellGrid::makeFromStringCanvas());

	static void mainLoop(void) {
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
		
		MainContext::drawCellGrid(cellGrid);

		SDL_RenderPresent(MainContext::renderer);
	}
}

int main(int const argCount, char **const argList) {
	#if true
	cob_init(argCount, argList);
	#else
	// Don't pass command line arguments.
	cob_init(int{0}, nullptr);
	#endif

	App::MainContext::initialize();

	int const exitCode = cobol_main();

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

	// Start the main loop.
	#ifdef __EMSCRIPTEN__
	/*
		Note to self:

		`simulate_infinite_loop` is `true`, so will not continue execution after this function ends.

		"...if simulate_infinite_loop is false, and you created an object on the stack,
		it will be cleaned up before the main loop is called for the first time."
		(https://emscripten.org/docs/api_reference/emscripten.h.html#id3)
	*/
	emscripten_set_main_loop(&mainLoop, -1, true);
	#else
	while (true) App::mainLoop();
	cob_tidy();
	#endif

	return exitCode;
}
