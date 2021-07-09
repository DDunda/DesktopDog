#pragma once

#include "Image.h"
#include "SDLG.h"

using namespace SDLG;

PaletteImage8bit::PaletteImage8bit(int _w, int _h, Uint8* _data) : w(_w), h(_h), data(_data) {};
SDL_Surface* PaletteImage8bit::GetSurface() {
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(data, w, h, 8, w, 0, 0, 0, 0);
	SDL_SetSurfacePalette(surface, palette);

	SDL_Surface* surface2 = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_FillRect(surface2, NULL, 0);
	SDL_Rect src{ 0,0,w,h };
	SDL_BlitSurface(surface, &src, surface2, &src);

	SDL_FreeSurface(surface);
	return surface2;
}
SDL_Texture* PaletteImage8bit::GetTexture(SDL_Renderer* r) {
	SDL_Surface* surface = GetSurface();

	SDL_Texture* finalTexture = SDL_CreateTextureFromSurface(r, surface);
	SDL_SetTextureBlendMode(finalTexture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
	return finalTexture;
}

PaletteImage2bit::PaletteImage2bit(int _w, int _h, Uint8* _data) : w(_w), h(_h), data(_data) {};
Uint8* PaletteImage2bit::UncompressData() {
	Uint8* dstDat = new Uint8[w * h];
	int bits[4];
	int bitHead = 4;
	Uint8* tmp = data;
	for (int i = 0; i < w * h; i++) {
		if (bitHead == 4) {
			Uint8 byte = *tmp;
			tmp++;
			for (int j = 0; j < 4; j++) {
				bits[j] = byte & 0b11;
				byte >>= 2;
			}
			bitHead = 0;
		}
		dstDat[i] = bits[bitHead];
		bitHead++;
	}
	return dstDat;
}
SDL_Surface* PaletteImage2bit::GetSurface() {
	Uint8* uncompressedData = UncompressData();

	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(uncompressedData, w, h, 8, w, 0, 0, 0, 0);
	SDL_SetSurfacePalette(surface, palette);

	SDL_Surface* surface2 = SDL_CreateRGBSurfaceWithFormat(0, w, h, 32, SDL_PIXELFORMAT_RGBA32);
	SDL_FillRect(surface2, NULL, 0);
	SDL_Rect src{ 0,0,w,h };
	SDL_BlitSurface(surface, &src, surface2, &src);

	SDL_FreeSurface(surface);
	delete[] uncompressedData;
	return surface2;
}
SDL_Texture* PaletteImage2bit::GetTexture(SDL_Renderer* r) {
	SDL_Surface* surface = GetSurface();

	SDL_Texture* finalTexture = SDL_CreateTextureFromSurface(r, surface);
	SDL_SetTextureBlendMode(finalTexture, SDL_BlendMode::SDL_BLENDMODE_BLEND);
	SDL_FreeSurface(surface);
	return finalTexture;
}

Sprite::Sprite(ImageSource* _source) {
	source = _source;
}

FullSprite::FullSprite(ImageSource* _source, frame _frm) : Sprite::Sprite(_source), frm(_frm) {};
FullSprite::~FullSprite() {
	if (txt != NULL) SDL_DestroyTexture(txt);
}
void FullSprite::Render(SDL_Renderer* r) {
	if (txt != NULL) SDL_DestroyTexture(txt);
	txt = source->GetTexture(r);
}
void FullSprite::Draw(SDL_Renderer* r) {
	if (txt == NULL) Render(r);
	SDL_FRect dst = GetFrameRect(frm);
	SDL_RenderCopyF(r, txt, NULL, &dst);
}

void SheetSprite::Render(SDL_Renderer* r) {
	if (txt != NULL) SDL_DestroyTexture(txt);
	txt = source->GetTexture(r);
}
void SheetSprite::Draw(SDL_Renderer* r) {
	if (txt == NULL) Render(r);
	SDL_FRect dst = GetFrameRect(frm);
	SDL_RenderCopyF(r, txt, &src, &dst);
}