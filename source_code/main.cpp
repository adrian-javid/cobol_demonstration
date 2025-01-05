
#include <stddef.h>
#ifndef EMSCRIPTEN
#include <libcob.h>
#else
#include "libcob.h"
#endif

#include "extern.h"

#include <iostream>

template <int number>
void printNumber() {
	std::cout << number << '\n';
}

#include <cstdio>
#include <unistd.h>

int main(int const argCount, char **const argList) {
	#if true
	cob_init(argCount, argList);
	#else
	// Don't pass command line arguments.
	cob_init(int{0}, nullptr);
	#endif

	// This likely closes standard output
	int const exitCode = cobol_main();

	cob_tidy();

	return exitCode;
}
