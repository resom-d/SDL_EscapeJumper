#include "RenderPlay.h"

void RenderPlay::OnInit(SDL_Renderer* rend, path image)
{
	_rend = rend;

	SDL_Texture* tex = SDL_LoadTexture(rend, image.string().c_str());
	ScreenItem item = { {0,0}, tex };
	int w, h;
	SDL_QueryTexture(item.Tex, nullptr, nullptr, &w, &h);
	item.TexSize = { w,h };
	item.OPoint = { 0,0 };
	Items.push_back(item);

	item = { {0,0}, tex};
	SDL_QueryTexture(item.Tex, nullptr, nullptr, &w, &h);
	item.TexSize = { w,h };
	item.OPoint = { DisplayRect.w, 0 };
	Items.push_back(item);

}

void RenderPlay::OnLoop()
{
	for (auto item = Items.begin(); item != Items.end(); item++)
	{
		item->OPoint.x--;
		if (item->OPoint.x <= -DisplayRect.w) item->OPoint.x = DisplayRect.w;
	}

}

void RenderPlay::OnRender()
{
	SDL_Point ScreenSize = { DisplayRect.w, DisplayRect.h };

	for (auto item = Items.begin(); item != Items.end(); item++)
	{
		SDL_Rect sRect = { 0,0, item->TexSize.w, item->TexSize.h };
		SDL_Rect dRect = { item->OPoint.x, DisplayRect.y, item->TexSize.w, item->TexSize.h };

		SDL_RenderSetClipRect(_rend, &DisplayRect);
		SDL_RenderCopy(_rend, item->Tex, &sRect, &dRect);
	}

}

void RenderPlay::OnCleanup()
{
	for (auto item = Items.begin(); item != Items.end(); item++)
	{
		SDL_DestroyTexture(item->Tex);
	}
}

void RenderPlay::OnEvent(SDL_Event* event)
{
}
