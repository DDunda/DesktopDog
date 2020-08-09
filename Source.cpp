#include <SDL.h>
#include <SDL_image.h>

#define SCALING_FACTOR (4)
#define ERROR_LOGGING
#include "SDLG.h"

#include "AbstractedAccess.h"
#include "Generic.h"

#include "bark.hpp"

using namespace SDLG;

int main(int argc, char* argv[]) { return StartSDL(); }

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

// Toby's body
/*
....##..########..##....................
..##  ##        ##  ##..................
..##                ##..................
..##                  ##............##..
##                    ####........##  ##
##                        ####....##  ##
##                            ######  ##
##                                    ##
##                                    ##
##                                    ##
##                                    ##
##                                    ##
##                                  ##..
..##                                ##..
..##    ####    ########    ####    ##..
..##    ####    ##....##    ####    ##..
..##  ##..##  ##......##  ##..##  ##....
....##......##..........##......##......
*/
Uint8 TobyData[20 * 19] {
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
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,
	1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,
	0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,
	0,1,2,2,1,1,2,2,1,1,1,1,2,2,1,1,2,2,1,0,
	0,1,2,2,1,1,2,2,1,0,0,1,2,2,1,1,2,2,1,0,
	0,1,2,1,0,1,2,1,0,0,0,1,2,1,0,1,2,1,0,0,
	0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,
};

// Toby's face
/*
..##....##..
............
....####....
##..##....##
..########..
*/
Uint8 FaceData[6 * 5] {
	0,1,0,0,1,0,
	0,0,0,0,0,0,
	0,0,1,1,0,0,
	1,0,1,0,0,1,
	0,1,1,1,1,0,
};

// Toby's scared face
/*
..######......######..
##      ##..##      ##
##  ##  ##..##  ##  ##
##      ##..##      ##
..######......######..
..........####........
......##..##....##....
........########......
*/
Uint8 ScaredData[11 * 8]{
	0,1,1,1,0,0,0,1,1,1,0,
	1,2,2,2,1,0,1,2,2,2,1,
	1,2,1,2,1,0,1,2,1,2,1,
	1,2,2,2,1,0,1,2,2,2,1,
	0,1,1,1,0,0,0,1,1,1,0,
	0,0,0,0,0,1,1,0,0,0,0,
	0,0,0,1,0,1,0,0,1,0,0,
	0,0,0,0,1,1,1,1,0,0,0
};

// Toby's barking face
/*
..##....##..
............
....####....
##..##....##
..########..
....####....
*/
Uint8 BarkData[6 * 6]{
	0,1,0,0,1,0,
	0,0,0,0,0,0,
	0,0,1,1,0,0,
	1,0,1,0,0,1,
	0,1,1,1,1,0,
	0,0,1,1,0,0,
};

paletteImage tobyIm{
	20,19,TobyData
};
paletteImage faceIm{
	6,5,FaceData
};
paletteImage scaredIm{
	11,8,ScaredData
};
paletteImage barkIm{
	6, 6, BarkData
};

// Frames area construct used to dynamically shape rects to their parents.
// If the window could be resized, frames can change shape along with it.
frame bodyFrame = {
	{0.5,0.5},
	{0.5,0.5},

	{0,0},

	{20 * SCALING_FACTOR, 19 * SCALING_FACTOR},
	{0,0}
};
frame faceFrame = {
	{6.0 / 20.0, 6.5 / 19.0},
	{0.5,0.5},

	{0,0},

	{6 * SCALING_FACTOR, 5 * SCALING_FACTOR},
	{0,0},

	&bodyFrame
};
frame scaredFrame = {
	{5.5 / 20.0, 5.0 / 19.0},
	{0.5,0.5},

	{0,0},

	{11 * SCALING_FACTOR, 8 * SCALING_FACTOR},
	{0,0},

	&bodyFrame
};
frame barkFrame = {
	{6.0 / 20.0, 7.0 / 19.0},
	{0.5,0.5},

	{0,0},

	{6 * SCALING_FACTOR, 6 * SCALING_FACTOR},
	{0,0},

	&bodyFrame
};

sprite tobySprite{
	&tobyIm, &bodyFrame
};
sprite faceSprite{
	&faceIm, &faceFrame
};
sprite scaredSprite{
	&scaredIm, &scaredFrame
};
sprite barkSprite{
	&barkIm, &barkFrame
};

// The gradient of Toby's field of view.
float FOV_gradient = SDL_tanf(10.0 / 180.0 * M_PI);

bool tobyHeld = false;

// When toby is being held, this is the offset of the cursor to the position of the window
// If the window position were just set to the mouse's, it would seem that you are grabbing the corner
int windowDragOffsetX;
int windowDragOffsetY;

std::vector<sprite*> sprites {&tobySprite, &faceSprite, &scaredSprite, &barkSprite};

SDL_WindowShapeMode defaultShapeMode = {
	WindowShapeMode::ShapeModeDefault
};

// Renders an image using the built-in palette feature of SDL_Surfaces
SDL_Surface* ConstructSurface(paletteImage& source) {
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(source.data, source.w, source.h, 8, source.w, 0, 0, 0, 0);
	SDL_SetSurfacePalette(surface, source.palette);

	SDL_Surface* surface2 = SDL_CreateRGBSurfaceWithFormat(0, source.w, source.h, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_FillRect(surface2, NULL, 0);
	SDL_Rect src{ 0,0,source.w,source.h };
	SDL_BlitSurface(surface, &src, surface2, &src);

	SDL_FreeSurface(surface);
	return surface2;
}
SDL_Texture* RenderWithPalette(paletteImage& source) {
	SDL_Surface* surface = ConstructSurface(source);

	SDL_Texture* finalTexture = SDL_CreateTextureFromSurface(gameRenderer, surface);
	SDL_SetTextureBlendMode(finalTexture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
	return finalTexture;
}

void RenderSprite(sprite& spr) {
	if (spr.txt == NULL) spr.txt = RenderWithPalette(*(spr.source));
}
void DrawSprite(sprite& spr) {
	SDL_FRect dst = GetFrameRect(spr.frm);
	SDL_RenderCopyF(gameRenderer, spr.txt, NULL, &dst);
}

// Converts texture to surface
SDL_Surface* ToSurface(SDL_Texture* txt) {
	int w, h;
	int access;
	Uint32 format;
	SDL_QueryTexture(txt, &format, &access, &w, &h);

	if ((access & SDL_TEXTUREACCESS_TARGET) == 0) return NULL;
	SDL_Texture* target = SDL_GetRenderTarget(gameRenderer);
	SDL_SetRenderTarget(gameRenderer, txt);

	int depth = SDL_BYTESPERPIXEL(format);

	Uint8* pixels = new Uint8[w * h * depth];
	int pitch = w * depth;
	SDL_RenderReadPixels(gameRenderer, NULL, format, pixels, pitch);

	SDL_Surface* s = SDL_CreateRGBSurfaceWithFormatFrom(pixels, w, h, depth * 8, pitch, format);

	SDL_SetRenderTarget(gameRenderer, target);

	return s;
}

// Using the alpha of the surface, this changes the border shape of the window.
int SetWindowShape(SDL_Surface* s) {
	return SDL_SetWindowShape(gameWindow, s, &defaultShapeMode);
}
int SetWindowShape(SDL_Texture* t) {
	SDL_Surface* s = ToSurface(t);

	if (s != NULL) {
		int result = SetWindowShape(s);
		SDL_FreeSurface(s);
		if (result == 0) return 0;
	}

	// Binarized surface must match the resolution of the window
	// This takes the texture and forcibly spreads it over the whole window
	SDL_Texture* t2 = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
	SDL_SetTextureBlendMode(t2, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(gameRenderer, t2);
	SDL_RenderCopy(gameRenderer, t, NULL, NULL);
	SDL_RenderPresent(gameRenderer);
	SDL_SetRenderTarget(gameRenderer, NULL);

	s = ToSurface(t2);

	SDL_DestroyTexture(t2);

	int result = SetWindowShape(s);
	SDL_FreeSurface(s);
	return result;
}

bool barking = false;
signed short* barkStart = barkAudioData;
signed short* barkEnd = barkAudioData + 13440;
signed short* barkIter = barkStart;

float faceRadius = 5 * SCALING_FACTOR;

void Bark() {
	if (!barking) {
		barkIter = barkStart;
		barking = true;
	}
}

void SDLG::AudioCallback(void* userdata, Uint8* stream, int len) {
	int i = 0;
	if (barking) {
		for (; i < SOUND_BUFFER_SIZE; i++) {
			soundBuffer[i] = (*barkIter) / 32767.0;
			barkIter++;
			if (barkIter > barkEnd) break;
		}
		if (i != SOUND_BUFFER_SIZE) {
			barking = false;
			for (; i < SOUND_BUFFER_SIZE; i++) {
				soundBuffer[i] = 0.0f;
			}
		}
	}
	else {
		for (; i < SOUND_BUFFER_SIZE; i++) {
			soundBuffer[i] = 0.0f;
		}
	}
	SDL_memcpy(stream, soundBuffer, len);
}

bool OnFace(float sqrDist) {
	return sqrDist < faceRadius* faceRadius;
}
bool OnFace(float x, float y) {
	return OnFace(x * x + y * y);
}

void SDLG::OnStart() {
	SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);

	SDL_Palette* palette = SDL_AllocPalette(sizeof(tobyColours) / sizeof(SDL_Colour));
	SDL_SetPaletteColors(palette, tobyColours, 0, sizeof(tobyColours) / sizeof(SDL_Colour));

	for(int i = 0; i < sprites.size(); i++)
	{
		sprites[i]->source->palette = palette;
		RenderSprite(*(sprites[i]));
	}

	SDL_FreePalette(palette);

	// Sets canvas shape to the shape of toby's body
	if (SDL_IsShapedWindow(gameWindow) == SDL_TRUE) SetWindowShape(tobySprite.txt);
	SDL_PauseAudio(0);
}

void SDLG::OnFrame() {
	if (SDL_IsShapedWindow(gameWindow) == SDL_TRUE) SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
	else SDL_SetRenderDrawColor(gameRenderer, 63, 63, 63, 255);
	SDL_RenderClear(gameRenderer);

	// Kills Toby
	if (keyDown(SDLK_ESCAPE)) {
		gameRunning = false;
		return; // Skip rendering, straight to the kill animation.
	}

	// Picks Toby up
	if (InBounds(ToRect(GetFrameRect(&bodyFrame)), mouseX, mouseY)) {
		if (buttonPressed(SDL_BUTTON_LEFT)) {
			windowDragOffsetX = mouseX;
			windowDragOffsetY = mouseY;

			tobyHeld = true;
		}
	}
	if (tobyHeld) {
		// Toby is still being held
		if (buttonDown(SDL_BUTTON_LEFT)) {
			int cx, cy;
			SDL_GetGlobalMouseState(&cx, &cy);
			SDL_SetWindowPosition(gameWindow,
				cx - windowDragOffsetX,
				cy - windowDragOffsetY
			);
		}
		// ... Or not
		else {
			tobyHeld = false;
		}
	}

	SDL_FRect tobyRect = GetFrameRect(&bodyFrame);
	SDL_FPoint center = GetPointInRect(tobyRect, 5.0 / 20.0, 6.5 / 19.0);

	float offsetX = mouseX - center.x;
	float offsetY = mouseY - center.y;

	// Sets the direction toby's face faces, depending which directional 'cones' the mouse is in
	/*
	Example: The cursor is in upper cone, so he faces up...
	     ./\/.
	     .\x\.
	      .\.
	      ./. 
	       .
	      ./.
	      .\.
	     .\/\.
	     ./\/.
	
	 However it is in neither horizontal cone, so he only looks *directly* up
	.             .
	\..    x    ..\
	/\/..     ../\/
	\/\/ .. ..\/\/\
	/\/\/\/./\/\/\/
	\/\/\.. ..\/\/\
	/\/..     ../\/
	\..         ..\
	.             .
	*/
	float directionX = (abs(offsetX) < abs(offsetY) * FOV_gradient ? 0 : SCALING_FACTOR) * (offsetX < 0 ? -1 : 1);
	float directionY = (abs(offsetY) < abs(offsetX) * FOV_gradient ? 0 : SCALING_FACTOR) * (offsetY < 0 ? -1 : 1);

	// If the cursor is directly on his face, he doesn't look in any direction
	// Alternatively, he doesn't look around when you pick him up.
	bool onFace = OnFace(offsetX, offsetY);
	if (onFace || tobyHeld) directionX = directionY = 0;
	if (onFace && buttonPressed(SDL_BUTTON_LEFT)) Bark();

	DrawSprite(tobySprite);

	if (barking) {
		barkFrame.absoluteOffset = { directionX, directionY };
		DrawSprite(barkSprite);
	}
	else {
		faceFrame.absoluteOffset = { directionX, directionY };
		DrawSprite(faceSprite);
	}

	SDL_RenderPresent(gameRenderer);
}

void SDLG::OnQuit() {
	SDL_CloseAudio();

	SDL_Texture* shapeTxt = SDL_CreateTexture(gameRenderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, windowWidth, windowHeight);
	SDL_SetRenderTarget(gameRenderer, shapeTxt);
	SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 0);
	SDL_RenderClear(gameRenderer);
	SDL_RenderPresent(gameRenderer);

	// Layer the face and body onto a single texture
	DrawSprite(tobySprite);
	DrawSprite(scaredSprite);

	// Draw the layered texture
	SDL_SetRenderTarget(gameRenderer, NULL);
	SDL_RenderCopy(gameRenderer, shapeTxt, NULL, NULL);

	SDL_RenderPresent(gameRenderer);

	// Window shape must be set again because these eyes bulge outside of the body; they are cut off without this shape
	if (SDL_IsShapedWindow(gameWindow) == SDL_TRUE) SetWindowShape(shapeTxt);

	for (int i = 0; i < sprites.size(); i++) SDL_DestroyTexture(sprites[i]->txt);
	SDL_DestroyTexture(shapeTxt);

	SDL_RenderPresent(gameRenderer);

	SDL_Delay(750); // Comedy
}