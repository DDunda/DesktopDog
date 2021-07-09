#pragma once

#include <SDL.h>
#include "abstracted access.h"
#include "SDLG.h"

bool InBounds(SDL_Rect bounds, int x, int y);
bool InBounds(SDL_Rect bounds, SDL_Point coord);

SDL_Rect ToRect(SDL_FRect rect);

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

SDL_FPoint GetPointInRect(SDL_FRect rect, float x, float y);

SDL_FRect GetFrameRect(frame f);