#pragma once

#include <SDL.h>
#include "AbstractedAccess.h"

static constexpr bool InBounds(SDL_Rect bounds, int x, int y) {
	return
		x >= bounds.x &&
		x < bounds.x + bounds.w &&
		y >= bounds.y &&
		y < bounds.y + bounds.h;
}
static constexpr bool InBounds(SDL_Rect bounds, SDL_Point coord) { return InBounds(bounds, coord.x, coord.y); }

SDL_Rect ToRect(SDL_FRect rect) {
	return { (int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h };
}

template <class T>
using ep = EasyPointer<T>;

struct frame {
	SDL_FPoint parentRelativeOrigin; // Origin point within parent
	SDL_FPoint selfRelativeOrigin;   // Origin point within own rect

	SDL_FPoint relativeScale;        // Scale compared to parent

	SDL_FPoint absoluteScale;        // A constant scale, added to the relative scale
	SDL_FPoint absoluteOffset;       // Rect offset from origin

	frame* parent = NULL;
};

struct sprite {
	SDL_Rect src;
	SDL_Texture** texture;
};