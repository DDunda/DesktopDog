#pragma once

#include <SDL.h>
#include "SDLG.h"
#include "Generic.h"

class ImageSource {
public:
	virtual SDL_Surface* GetSurface() = 0;
	virtual SDL_Texture* GetTexture(SDL_Renderer*) = 0;
};

class PaletteImage {
public:
	SDL_Palette* palette = NULL;
};

class PaletteImage8bit : public ImageSource, public PaletteImage {
public:
	int w, h;
	Uint8* data = NULL;

	PaletteImage8bit(int _w, int _h, Uint8* _data);

	SDL_Surface* GetSurface();
	SDL_Texture* GetTexture(SDL_Renderer* r);
};

class PaletteImage2bit : public ImageSource, public PaletteImage {
public:
	int w, h;
	Uint8* data = NULL;

	PaletteImage2bit(int _w, int _h, Uint8* _data);

	Uint8* UncompressData();

	SDL_Surface* GetSurface();
	SDL_Texture* GetTexture(SDL_Renderer*);
};

class Drawable {
public:
	virtual void Draw(SDL_Renderer*) = 0;
};

class Sprite {
public:
	ImageSource* source;
	SDL_Texture* txt = NULL;

	Sprite(ImageSource* _source);

	virtual void Render(SDL_Renderer*) = 0;
};

class FullSprite : public Drawable, public Sprite {
public:
	frame frm;

	FullSprite(ImageSource* _source, frame _frm);
	~FullSprite();

	void Render(SDL_Renderer*);
	void Draw(SDL_Renderer*);
};

class SheetSprite : public Drawable, public Sprite {
public:
	frame frm;
	SDL_Rect src;

	void Render(SDL_Renderer*);
	void Draw(SDL_Renderer*);
};