#include "SDL_AdditionalFunctions.h"

map<char, SDL_Texture*> SDL_AdditionalFunctions::GetTexturesFromString(SDL_Renderer* rend, string aString, TTF_Font* font)
{
	map<char, SDL_Texture*> chars;
	

	for (auto iter = aString.begin(); iter != aString.end(); iter++)
	{
		char c = *iter;
		SDL_Surface* surf = TTF_RenderText_Solid(font, &c, { 0,0,0, 255 });
		SDL_Texture* tex = SDL_CreateTextureFromSurface(rend, surf);
		chars.insert({ c, tex });
	}

	return chars;
}
