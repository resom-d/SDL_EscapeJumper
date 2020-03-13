#include "SimpleScroller.h"

void SimpleScroller::OnInit(SDL_Renderer* rend, path image)
{
	_rend = rend;

	Tex = SDL_LoadTexture(rend, image.string().c_str());
	SDL_QueryTexture(Tex, nullptr, nullptr, &TexSize.x, &TexSize.y);

	Items.push_back({ 0,0 });
	Items.push_back({ DisplayRect.w, 0 });

}

void SimpleScroller::OnLoop()
{
	for (auto item = Items.begin(); item != Items.end(); item++)
	{
		item->x--;
		if (item->x <= -DisplayRect.w) item->x = DisplayRect.w;
	}

}

void SimpleScroller::OnRender()
{
	SDL_Point ScreenSize = { DisplayRect.w, DisplayRect.h };

	SDL_RenderSetClipRect(_rend, &DisplayRect);
	for (auto item = Items.begin(); item != Items.end(); item++)
	{
		SDL_Rect sRect = { 0,0, TexSize.x, TexSize.y };
		SDL_Rect dRect = { item->x, DisplayRect.y, TexSize.x, TexSize.y };

		SDL_RenderCopy(_rend, Tex, &sRect, &dRect);
	}

}

void SimpleScroller::OnCleanup()
{	
		SDL_DestroyTexture(Tex);	
}

void SimpleScroller::OnEvent(SDL_Event* event)
{
}
