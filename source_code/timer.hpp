#pragma once

namespace App { struct Timer; }

#include "SDL.h"
#include <cstdint>

struct App::Timer {
	static constexpr Uint64 oneSecond{1'000u};

	Uint64 accumulatedTime, interval;

	[[nodiscard]] constexpr explicit Timer(Uint64 const initInterval):
		accumulatedTime{0u}, interval{initInterval}
	{}

	enum struct Modifier : std::uint_least8_t { none = 0u, compensateLostTime = 1u };

	/*
		Return `true` if spanned interval, otherwise `false`.
	*/
	template <Modifier modifier=Modifier::none>
	constexpr bool update(Uint64 const deltaTime) {
		accumulatedTime += deltaTime;

		if (accumulatedTime >= interval) {
			if constexpr (modifier == Modifier::compensateLostTime) {
				accumulatedTime -= interval;
			} else {
				accumulatedTime = 0u;
			}
			return true;
		}

		return false;
	}
};
