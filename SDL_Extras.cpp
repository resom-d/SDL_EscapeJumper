#include "SDL_Extras.h"

CharacterTextureMap SDL_GetTexturesFromString(SDL_Renderer* rend, string aString, TTF_Font* font)
{
	unordered_map<char, SDL_Texture*> chars;


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

void SDL_RenderStringAt(SDL_Renderer* rend, string text, SDL_Point p, CharacterTextureMap chars, Uint16 size, SDL_Rect* clipRect)
{
	SDL_Rect destRect{ p.x, p.y, 0,0 };
	SDL_Rect srcRect = { 0,0,0,0 };

	SDL_RenderSetClipRect(rend, clipRect);
	for (auto iter = text.begin(); iter != text.end(); iter++)
	{
		char c = *iter;
		Uint32 f;
		int a, w, h;
		SDL_QueryTexture(chars[c], &f, &a, &w, &h);
		destRect.w = size;
		destRect.h = size;
		srcRect = { 0,0,w,h };
		SDL_SetRenderDrawBlendMode(rend, SDL_BLENDMODE_BLEND);
		SDL_RenderCopy(rend, chars[c], &srcRect, &destRect);
		destRect.x += size;
	}
	SDL_RenderSetClipRect(rend, nullptr);
}

void SDL_RenderSetPixel(SDL_Renderer* renderer, int x, int y, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
	SDL_RenderDrawPoint(renderer, x, y);
}

void SDL_RenderDrawCircle(SDL_Renderer* renderer, int n_cx, int n_cy, int radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	// if the first pixel in the screen is represented by (0,0) (which is in sdl)
	// remember that the beginning of the circle is not in the middle of the pixel
	// but to the left-top from it:

	double error = (double)-radius;
	double x = (double)radius - 0.5;
	double y = (double)0.5;
	double cx = n_cx - 0.5;
	double cy = n_cy - 0.5;

	while (x >= y)
	{
		SDL_RenderSetPixel(renderer, (int)(cx + x), (int)(cy + y), red, green, blue, alpha);
		SDL_RenderSetPixel(renderer, (int)(cx + y), (int)(cy + x), red, green, blue, alpha);

		if (x != 0)
		{
			SDL_RenderSetPixel(renderer, (int)(cx - x), (int)(cy + y), red, green, blue, alpha);
			SDL_RenderSetPixel(renderer, (int)(cx + y), (int)(cy - x), red, green, blue, alpha);
		}

		if (y != 0)
		{
			SDL_RenderSetPixel(renderer, (int)(cx + x), (int)(cy - y), red, green, blue, alpha);
			SDL_RenderSetPixel(renderer, (int)(cx - y), (int)(cy + x), red, green, blue, alpha);
		}

		if (x != 0 && y != 0)
		{
			SDL_RenderSetPixel(renderer, (int)(cx - x), (int)(cy - y), red, green, blue, alpha);
			SDL_RenderSetPixel(renderer, (int)(cx - y), (int)(cy - x), red, green, blue, alpha);
		}

		error += y;
		++y;
		error += y;

		if (error >= 0)
		{
			--x;
			error -= x;
			error -= x;
		}
	}
}

void SDL_RenderFillCircle(SDL_Renderer* renderer, int cx, int cy, int radius, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha)
{
	// Note that there is more to altering the bitrate of this 
	// method than just changing this value.  See how pixels are
	// altered at the following web page for tips:
	//   http://www.libsdl.org/intro.en/usingvideo.html

	for (double dy = 1; dy <= radius; dy += 1.0)
	{
		// This loop is unrolled a bit, only iterating through half of the
		// height of the circle.  The result is used to draw a scan line and
		// its mirror image below it.

		// The following formula has been simplified from our original.  We
		// are using half of the width of the circle because we are provided
		// with a center and we need left/right coordinates.

		double dx = floor(sqrt((2.0 * radius * dy) - (dy * dy)));
		int x = cx - dx;
		SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
		SDL_RenderDrawLine(renderer, cx - dx, cy + dy - radius, cx + dx, cy + dy - radius);
		SDL_RenderDrawLine(renderer, cx - dx, cy - dy + radius, cx + dx, cy - dy + radius);

	}
}

void SDL_RenderDrawBorder(SDL_Renderer* rend, SDL_Rect rect, Uint16 borderWidth, SDL_Color color)
{
	SDL_Rect drect;
	drect.x = rect.x;
	drect.y = rect.y;
	drect.w = rect.w;
	drect.h = rect.h;

	for (int i = 0; i < borderWidth; i++)
	{
		SDL_SetRenderDrawColor(rend, color.r, color.g, color.b, color.a);
		SDL_RenderSetClipRect(rend, &drect);
		SDL_RenderDrawRect(rend, &drect);
		drect.x++;
		drect.y++;
		drect.h -= 2;
		drect.w -= 2;
	}
	SDL_RenderSetClipRect(rend, nullptr);
}

void SDL_RenderSetDrawColor(SDL_Renderer* rend, SDL_Color col)
{
	SDL_SetRenderDrawColor(rend, col.r, col.g, col.b, col.a);
}

void CreateTilemap(path sdir, path ddir, string filename, Uint16 cols, Uint16 rows, Size2D outsize, SDL_Color* fillcolor)
{
	list<path> files = GetFilesInDirectory("Resources/tilemaps/input");

	SDL_Surface* surf = SDL_CreateRGBSurface(0, cols * outsize.w, rows * outsize.h, 32, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000);
	if (fillcolor != nullptr)
	{
		SDL_Rect fillrect = { 0,0,cols * outsize.w, rows * outsize.h };
		SDL_FillRect(surf, nullptr, SDL_MapRGB(surf->format, fillcolor->r, fillcolor->g, fillcolor->b));
	}

	int x = 0, y = 0;
	SDL_Rect sRect = { 0, 0, 0, 0 };
	SDL_Rect dRect = { 0, 0 ,outsize.w, outsize.h };
	SDL_Surface* sourceImg;
	for (auto fiter = files.begin(); fiter != files.end(); fiter++)
	{
		string sif = fiter->string();
		sourceImg = IMG_Load(sif.c_str());
		sRect.w = sourceImg->w;
		sRect.h = sourceImg->h;

		dRect.x = (x % cols) * outsize.w;
		dRect.y = (x / cols) * outsize.h;
		SDL_BlitScaled(sourceImg, &sRect, surf, &dRect);
		x++;
		SDL_FreeSurface(sourceImg);
	}

	string outfile = ddir.string() + "/" + filename;
	IMG_SavePNG(surf, outfile.c_str());
	SDL_FreeSurface(surf);

}

void CreateWidgetTexture(SDL_Renderer* rend, string filePath, SDL_Texture* destTex, SDL_Rect srcRect, SDL_Rect destRect, double rot, SDL_RendererFlip flip)
{
	SDL_Texture* orgTex = SDL_GetRenderTarget(rend);
	SDL_Surface* s = IMG_Load(filePath.c_str());
	SDL_Texture* t = SDL_CreateTextureFromSurface(rend, s);
	SDL_SetTextureBlendMode(destTex, SDL_BLENDMODE_BLEND);
	SDL_SetRenderTarget(rend, destTex);
	SDL_RenderSetClipRect(rend, &destRect);
	SDL_RenderCopyEx(rend, t, &srcRect, &destRect, rot, nullptr, flip);
	SDL_RenderSetClipRect(rend, nullptr);
	SDL_FreeSurface(s);
	SDL_DestroyTexture(t);
	SDL_SetRenderTarget(rend, orgTex);
}

list<string> SplitString(string sourceString, const char splitter)
{
	stringstream ss(sourceString);
	list<string> splittedStrings;
	string item;
	while (getline(ss, item, splitter))
	{
		splittedStrings.push_back(item);
	}
	return splittedStrings;
}

list<path> GetFilesInDirectory(path dir)
{
	list<path> files;
	if (!is_directory(dir)) return files;

	for (directory_iterator itr(dir); itr != directory_iterator(); ++itr)
	{
		if (!is_directory(itr->path()))	files.push_back(itr->path());
	}
	return files;
}
