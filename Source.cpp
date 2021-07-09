#include <SDL.h>

#include "SDLG.h"

#include "AbstractedAccess.h"
#include "Generic.h"

#include "bark.hpp"

#include "Image.h"

#include "spriteData.h"

using namespace SDLG;

int main(int argc, char* argv[]) { return StartSDL(); }

SDL_Colour dogColours[3] = { {0,0,0,0}, {0,0,0,255}, {255,255,255,255} };

PaletteImage8bit dogIm  (20, 19, IdleData8b);
PaletteImage8bit faceIm  ( 6,  5, FaceData8b  );
PaletteImage8bit scaredIm(11,  8, ScaredData8b);
PaletteImage8bit barkIm  ( 6,  6, BarkData8b  );

//PaletteImage2bit dogIm  (20, 19, DogData2b  );
//PaletteImage2bit faceIm   (6,  5, FaceData2b  );
//PaletteImage2bit scaredIm(11,  8, ScaredData2b);
//PaletteImage2bit barkIm  ( 6,  6, BarkData2b  );

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

FullSprite dogSprite(    &dogIm,   bodyFrame);
FullSprite faceSprite(    &faceIm,   faceFrame);
FullSprite scaredSprite(&scaredIm, scaredFrame);
FullSprite barkSprite(    &barkIm,   barkFrame);

// The gradient of Dog's field of view.
float FOV_gradient = SDL_tanf(10.0 / 180.0 * M_PI);

bool dogHeld = false;

// When dog is being held, this is the offset of the cursor to the position of the window
// If the window position were just set to the mouse's, it would seem that you are grabbing the corner
int windowDragOffsetX;
int windowDragOffsetY;

std::vector<PaletteImage*> paletteImages{ &dogIm, &faceIm, &scaredIm, &barkIm };
std::vector<Sprite*> sprites{ &dogSprite, &faceSprite, &scaredSprite, &barkSprite };

SDL_WindowShapeMode defaultShapeMode = {
	WindowShapeMode::ShapeModeDefault
};

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

float faceRadius = 4 * SCALING_FACTOR;

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

constexpr bool OnFace(float sqrDist) {
	return sqrDist < faceRadius * faceRadius;
}
constexpr bool OnFace(float x, float y) {
	return OnFace(x * x + y * y);
}

SDL_Texture* tmp;

void SDLG::OnStart() {
	SDL_SetRenderDrawBlendMode(gameRenderer, SDL_BLENDMODE_BLEND);

	SDL_Palette* palette = SDL_AllocPalette(sizeof(dogColours) / sizeof(SDL_Colour));
	SDL_SetPaletteColors(palette, dogColours, 0, sizeof(dogColours) / sizeof(SDL_Colour));

	for(int i = 0; i < paletteImages.size(); i++)
		paletteImages[i]->palette = palette;

	for (int i = 0; i < sprites.size(); i++)
		sprites[i]->Render(gameRenderer);

	SDL_FreePalette(palette);

	// Sets canvas shape to the shape of dog's body
	if (SDL_IsShapedWindow(gameWindow) == SDL_TRUE) SetWindowShape(dogSprite.txt);
	SDL_PauseAudio(0);
}

void SDLG::OnFrame() {
	if (SDL_IsShapedWindow(gameWindow) == SDL_TRUE) SDL_SetRenderDrawColor(gameRenderer, 0, 0, 0, 255);
	else SDL_SetRenderDrawColor(gameRenderer, 63, 63, 63, 255);
	SDL_RenderClear(gameRenderer);

	// Kills Dog
	if (keyDown(SDLK_ESCAPE)) {
		gameRunning = false;
		return; // Skip rendering, straight to the kill animation.
	}

	// Picks Dog up
	if (InBounds({0,0,windowWidth,windowHeight}, mouseX, mouseY)) {
		if (buttonPressed(SDL_BUTTON_LEFT)) {
			windowDragOffsetX = mouseX;
			windowDragOffsetY = mouseY;

			dogHeld = true;
		}
	}
	if (dogHeld) {
		// Dog is still being held
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
			dogHeld = false;
		}
	}

	SDL_FPoint center = { 6.0 * SCALING_FACTOR, 6.5 * SCALING_FACTOR };

	float offsetX = mouseX - center.x;
	float offsetY = mouseY - center.y;

	// Sets the direction dog's face faces, depending which directional 'cones' the mouse is in
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
	if (onFace || dogHeld) directionX = directionY = 0;
	if (onFace && buttonPressed(SDL_BUTTON_LEFT)) Bark();

	dogSprite.Draw(gameRenderer);

	if (barking) {
		barkSprite.frm.absoluteOffset = { directionX, directionY };
		barkSprite.Draw(gameRenderer);
	}
	else {
		faceSprite.frm.absoluteOffset = { directionX, directionY };
		faceSprite.Draw(gameRenderer);
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
	dogSprite.Draw(gameRenderer);
	scaredSprite.Draw(gameRenderer);

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