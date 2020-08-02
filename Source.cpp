#include <SDL.h>
#include <SDL_image.h>

#define SCALING_FACTOR (4)
#define ERROR_LOGGING
#include "SDLG.h"

#include "AbstractedAccess.h"
#include "Generic.h"

using namespace SDLG;

int main(int argc, char* argv[]) { return StartSDL(); }

template<int w, int h>
struct paletteImage {
	Uint8 data[w * h];
	SDL_Palette* palette = NULL;
};

SDL_FPoint GetPointInRect(SDL_FRect rect, float x, float y) {
	return { rect.x + rect.w * x, rect.y + rect.h * y };
}

SDL_FRect GetFrameRect(frame* f) {
	SDL_FRect parentRect;

	if (f->parent == NULL) parentRect = { 0, 0, (float)windowWidth, (float)windowHeight };
	else parentRect = GetFrameRect(f->parent);

	float width = f->relativeScale.x * parentRect.w + f->absoluteScale.x;
	float height = f->relativeScale.y * parentRect.h + f->absoluteScale.y;
	float x = parentRect.x + parentRect.w * f->parentRelativeOrigin.x - width * f->selfRelativeOrigin.x + f->absoluteOffset.x;
	float y = parentRect.y + parentRect.h * f->parentRelativeOrigin.y - height * f->selfRelativeOrigin.y + f->absoluteOffset.y;

	return { x,y,width,height };
}

SDL_Colour tobyColours[3] = { {0,0,0,0}, {0,0,0,255}, {255,255,255,255} };

paletteImage<20, 19> Toby{
	{
		0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0, //1
		0,1,2,1,2,2,2,2,1,2,1,0,0,0,0,0,0,0,0,0, //2
		0,1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0, //3
		0,1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,1,0, //4
		1,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,1,2,1, //5
		1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,1,2,1, //6
		1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,2,1, //7
		1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1, //8
		1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1, //9
		1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1, //10
		1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1, //11
		1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1, //12
		1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1, //13
		1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0, //14
		0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0, //15
		0,1,2,2,1,1,2,2,1,1,1,1,2,2,1,1,2,2,1,0, //16
		0,1,2,2,1,1,2,2,1,0,0,1,2,2,1,1,2,2,1,0, //17
		0,1,2,1,0,1,2,1,0,0,0,1,2,1,0,1,2,1,0,0, //18
		0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0, //19
	}
};

paletteImage<6, 5> TobyFace{
	{
		0,1,0,0,1,0,
		0,0,0,0,0,0,
		0,0,1,1,0,0,
		1,0,1,0,0,1,
		0,1,1,1,1,0,
	}
};

paletteImage<11, 8> ScaredTobyFace{
	{
		0,1,1,1,0,0,0,1,1,1,0,
		1,2,2,2,1,0,1,2,2,2,1,
		1,2,1,2,1,0,1,2,1,2,1,
		1,2,2,2,1,0,1,2,2,2,1,
		0,1,1,1,0,0,0,1,1,1,0,
		0,0,0,0,0,1,1,0,0,0,0,
		0,0,0,1,0,1,0,0,1,0,0,
		0,0,0,0,1,1,1,1,0,0,0
	}
};

template<int w, int h>
SDL_Surface* ConstructSurface(paletteImage<w, h>& source) {
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(source.data, w, h, 8, w, 0, 0, 0, 0);
	SDL_SetSurfacePalette(surface, source.palette);

	SDL_Surface* surface2 = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_FillRect(surface2, NULL, 0);
	SDL_Rect src{ 0,0,w,h };
	SDL_BlitSurface(surface, &src, surface2, &src);

	SDL_FreeSurface(surface);
	return surface2;
}

template<int w, int h>
SDL_Texture* RenderWithPalette(paletteImage<w, h>& source) {
	SDL_Surface* surface = ConstructSurface(source);

	SDL_Texture* finalTexture = SDL_CreateTextureFromSurface(gameRenderer, surface);
	SDL_SetTextureBlendMode(finalTexture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
	return finalTexture;
}

/*
..#.####.#..........
.# #    # #.........
.#        #.........
.#         #......#.
#  #  #    ##....# #
#            ##..# #
#   ##         ### #
# # #  #           #
#  ####            #
#                  #
#                  #
#                  #
#                 #.
.#                #.
.#  ##  ####  ##  #.
.#  ##  #..#  ##  #.
.# #.# #...# #.# #..
..#...#.....#...#...

0,0,1,0,1,1,1,1,0,1,0,0,0,0,0,0,0,0,0,0,
0,1,2,1,2,2,2,2,1,2,1,0,0,0,0,0,0,0,0,0,
0,1,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,0,0,0,
0,1,2,2,2,2,2,2,2,2,2,1,0,0,0,0,0,0,1,0,
1,2,2,2,2,2,2,2,2,2,2,1,1,0,0,0,0,1,2,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,0,0,1,2,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,2,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,
0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,
0,1,2,2,1,1,2,2,1,1,1,1,2,2,1,1,2,2,1,0,
0,1,2,2,1,1,2,2,1,0,0,1,2,2,1,1,2,2,1,0,
0,1,2,1,0,1,2,1,0,0,0,1,2,1,0,1,2,1,0,0,
0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,
*/

/*
.#..#.
......
..##..
#.#..#
.####.

0,1,0,0,1,0,
0,0,0,0,0,0,
0,0,1,1,0,0,
1,0,1,0,0,1,
0,1,1,1,1,0,
*/

SDL_Texture* TobyTexture;
SDL_Texture* TobyFaceTexture;
SDL_Texture* ScaredFaceTexture;

float gradient;

SDL_Palette* palette;

void SDLG::OnStart() {
	SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
	palette = SDL_AllocPalette(3);
	SDL_SetPaletteColors(palette, tobyColours, 0, 3);

	Toby.palette = palette;
	TobyFace.palette = palette;
	ScaredTobyFace.palette = palette;

	TobyTexture = RenderWithPalette(Toby);
	TobyFaceTexture = RenderWithPalette(TobyFace);
	ScaredFaceTexture = RenderWithPalette(ScaredTobyFace);

	gradient = SDL_tanf(22.5 / 180 * M_PI);

	if (SDL_IsShapedWindow(gameWindow) == SDL_TRUE) {
		SDL_Surface* tobyShape1 = ConstructSurface(Toby);
		SDL_Surface* tobyShape2 = SDL_CreateRGBSurfaceWithFormat(0, windowWidth, windowHeight, 32, SDL_PIXELFORMAT_RGBA32);
		SDL_FillRect(tobyShape2, NULL, 0);
		SDL_Rect src{ 0,0,20,19 };
		SDL_Rect dst{ 0,0,20 * SCALING_FACTOR - 1,19 * SCALING_FACTOR - 1 };
		SDL_BlitScaled(tobyShape1, &src, tobyShape2, &dst);
		SDL_WindowShapeMode shapemode = {
			WindowShapeMode::ShapeModeBinarizeAlpha
		};
		shapemode.parameters.binarizationCutoff = 255;
		SDL_SetWindowShape(gameWindow, tobyShape2, &shapemode);

		SDL_FreeSurface(tobyShape1);
		SDL_FreeSurface(tobyShape2);
	}
}

#define FOV_GRADIENT 0.2

int windowDragOffsetX;
int windowDragOffsetY;

bool tobyHeld = false;

frame tobyFrame = {
	{0.5,0.5},
	{0.5,0.5},

	{0,0},

	{20 * SCALING_FACTOR, 19 * SCALING_FACTOR},
	{0,0}
};

frame tobyFaceFrame = {
	{6.0 / 20.0, 6.5 / 19.0},
	{0.5,0.5},

	{0,0},

	{6 * SCALING_FACTOR, 5 * SCALING_FACTOR},
	{0,0},

	&tobyFrame
};

frame scaredFaceFrame = {
	{5.5 / 20.0, 5.0 / 19.0},
	{0.5,0.5},

	{0,0},

	{11 * SCALING_FACTOR, 8 * SCALING_FACTOR},
	{0,0},

	&tobyFrame
};

void SDLG::OnFrame() {
	if (SDL_IsShapedWindow(gameWindow) == SDL_TRUE) SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
	else SDL_SetRenderDrawColor(gameRenderer, 63, 63, 63, 255);
	SDL_RenderClear(gameRenderer);

	SDL_FRect tobyRect = GetFrameRect(&tobyFrame);
	SDL_FPoint center = GetPointInRect(tobyRect, 5.0 / 20.0, 6.5 / 19.0);

	float offsetX = mouseX - center.x;
	float offsetY = mouseY - center.y;

	float directionX = (abs(offsetX) < abs(offsetY) * FOV_GRADIENT ? 0 : SCALING_FACTOR) * (offsetX < 0 ? -1 : 1);
	float directionY = (abs(offsetY) < abs(offsetX) * FOV_GRADIENT ? 0 : SCALING_FACTOR) * (offsetY < 0 ? -1 : 1);

	// Gross
	//float magnitude = sqrtf(offsetX * offsetX + offsetY * offsetY);
	//float directionX = offsetX / magnitude * SCALING_FACTOR;
	//float directionY = offsetY / magnitude * SCALING_FACTOR;

	tobyFaceFrame.absoluteOffset = { 0, 0 };

	if (InBounds(ToRect(GetFrameRect(&tobyFaceFrame)), mouseX, mouseY)) {
		directionX = 0;
		directionY = 0;
	}
	else if(InBounds(ToRect(GetFrameRect(&tobyFrame)),mouseX,mouseY)) {
		if (buttonPressed(SDL_BUTTON_LEFT)) {
			windowDragOffsetX = mouseX;
			windowDragOffsetY = mouseY;

			tobyHeld = true;
		}
	}

	if (tobyHeld) {
		if (buttonDown(SDL_BUTTON_LEFT)) {
			int cx, cy;
			SDL_GetGlobalMouseState(&cx, &cy);
			SDL_SetWindowPosition(gameWindow,
				cx - windowDragOffsetX,
				cy - windowDragOffsetY
			);
		}
		else {
			tobyHeld = false;
		}
	}

	if (keyDown(SDLK_ESCAPE)) gameRunning = false;

	tobyFaceFrame.absoluteOffset = { directionX, directionY };

	SDL_FRect tobyFaceRect = GetFrameRect(&tobyFaceFrame);

	SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
	SDL_RenderCopyF(gameRenderer, TobyTexture, NULL, &tobyRect);
	SDL_RenderCopyF(gameRenderer, TobyFaceTexture, NULL, &tobyFaceRect);

	SDL_RenderPresent(gameRenderer);
}

void SDLG::OnQuit() {
	if (SDL_IsShapedWindow(gameWindow) == SDL_TRUE) {
		SDL_Rect srcToby{ 0,0,20,19 };
		SDL_Rect dstToby{ 0,0,20 * SCALING_FACTOR, 19 * SCALING_FACTOR };

		SDL_Rect srcFace{ 0,0,11,8 };
		SDL_Rect dstFace{ 0,1 * SCALING_FACTOR, 11 * SCALING_FACTOR, 8 * SCALING_FACTOR };

		SDL_Surface* toby = ConstructSurface(Toby);
		SDL_Surface* tobyFace = ConstructSurface(ScaredTobyFace);
		SDL_Surface* tobyShape = SDL_CreateRGBSurfaceWithFormat(0, windowWidth, windowHeight, 32, SDL_PIXELFORMAT_RGBA32);
		SDL_FillRect(tobyShape, NULL, 0);

		SDL_BlitScaled(toby, &srcToby, tobyShape, &dstToby);
		SDL_BlitScaled(tobyFace, &srcFace, tobyShape, &dstFace);
		SDL_WindowShapeMode shapemode = {
			WindowShapeMode::ShapeModeBinarizeAlpha
		};
		shapemode.parameters.binarizationCutoff = 1;
		SDL_SetWindowShape(gameWindow, tobyShape, &shapemode);

		SDL_FreeSurface(toby);
		SDL_FreeSurface(tobyFace);
		SDL_FreeSurface(tobyShape);
	}
	SDL_FreePalette(palette);

	SDL_FRect tobyRect = GetFrameRect(&tobyFrame);
	SDL_FRect faceRect = GetFrameRect(&scaredFaceFrame);

	SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);
	SDL_RenderCopyF(gameRenderer, TobyTexture, NULL, &tobyRect);
	SDL_RenderCopyF(gameRenderer, ScaredFaceTexture, NULL, &faceRect);
	SDL_RenderPresent(gameRenderer);

	SDL_DestroyTexture(ScaredFaceTexture);
	SDL_DestroyTexture(TobyTexture);
	SDL_DestroyTexture(TobyFaceTexture);

	SDL_Delay(750); // Comedy
}