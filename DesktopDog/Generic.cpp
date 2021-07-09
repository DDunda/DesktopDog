#include "Generic.h"

#pragma once

#include <SDL.h>
#include "abstracted access.h"
#include "SDLG.h"

bool InBounds(SDL_Rect bounds, int x, int y) {
	return
		x >= bounds.x &&
		x < bounds.x + bounds.w &&
		y >= bounds.y &&
		y < bounds.y + bounds.h;
}
bool InBounds(SDL_Rect bounds, SDL_Point coord) { return InBounds(bounds, coord.x, coord.y); }

SDL_Rect ToRect(SDL_FRect rect) {
	return { (int)rect.x, (int)rect.y, (int)rect.w, (int)rect.h };
}

using namespace SDLG;

SDL_FPoint GetPointInRect(SDL_FRect rect, float x, float y) {
	return { rect.x + rect.w * x, rect.y + rect.h * y };
}

SDL_FRect GetFrameRect(frame f) {
	SDL_FRect parentRect;

	if (f.parent == NULL) parentRect = { 0, 0, (float)windowWidth, (float)windowHeight };
	else parentRect = GetFrameRect(*(f.parent));

	float width = f.relativeScale.x * parentRect.w + f.absoluteScale.x;
	float height = f.relativeScale.y * parentRect.h + f.absoluteScale.y;
	float x = parentRect.x + parentRect.w * f.parentRelativeOrigin.x - width * f.selfRelativeOrigin.x + f.absoluteOffset.x;
	float y = parentRect.y + parentRect.h * f.parentRelativeOrigin.y - height * f.selfRelativeOrigin.y + f.absoluteOffset.y;

	return { x,y,width,height };
}