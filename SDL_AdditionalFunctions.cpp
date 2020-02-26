#include "SDL_AdditionalFunctions.h"

map<char, SDL_Texture*> SDL_AdditionalFunctions::GetTexturesFromString(SDL_Renderer* rend, string aString, TTF_Font* font)
{
	map<char, SDL_Texture*> chars;


	for (auto iter = aString.begin(); iter != aString.end(); iter++)
	{
		char c = *iter;
		char txt[2] = { c, '\0' };

		SDL_Surface* surf = TTF_RenderText_Solid(font, txt, { 0,0,0, 255 });
		SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);
		chars.insert({ c, tex });
	}

	return chars;
}

void SDL_AdditionalFunctions::SDL_RenderStringAt(SDL_Renderer* rend, string text, SDL_Point p, map<char, SDL_Texture*> chars, Uint16 size)
{
	SDL_Rect destRect{ p.x, p.y, 0,0 };
	SDL_Rect srcRect = { 0,0,0,0 };

	for (auto iter = text.begin(); iter != text.end(); iter++)
	{
		char c = *iter;
		Uint32 f;
		int a,w, h;
		SDL_QueryTexture(chars[c],&f,  &a, &w, &h);
		destRect.w = size;
		destRect.h = size;
		srcRect = { 0,0,w,h };
		SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
		SDL_RenderSetClipRect(rend, &destRect);
		SDL_RenderCopy(rend, chars[c], &srcRect, &destRect);
		destRect.x += size;
	}
	SDL_RenderSetClipRect(rend, nullptr);
}
