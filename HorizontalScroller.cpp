#include "HorizontalScoller.h"


HorizontalScroller::HorizontalScroller()
{}

// Game Functions
void HorizontalScroller::OnInit(SDL_Renderer* renderer, GameMap* map)
{
	_rend = renderer;
	_map = map;
	BlockPosition.x = 0;
	BlockPosition.y = 0;
	ScrollPosition.x = 0;
	ScrollPosition.y = 0;
	
}

void HorizontalScroller::OnLoop()
{
	if (ScrollPosition.x > _map->Setup.BlockSize + _map->Setup.BlockSpacing)
	{
		ScrollPosition.x = ScrollPosition.x % (_map->Setup.BlockSpacing + _map->Setup.BlockSpacing);
		if (++BlockPosition.x > _map->Setup.Cols - _map->Setup.DisplayCols - 1)
		{
			BlockPosition.x = 0;
			ScrollPosition.x = 0;
			//LevelDone = true;
		}
	}
	ScrollPosition.x += ScrollSpeed;
}

void HorizontalScroller::OnRender()
{
	if (_rend == nullptr) return;

	SDL_RenderSetClipRect(_rend, &_map->Setup.DisplayRect);
	SDL_SetRenderDrawColor(_rend,
		_map->Setup.Background.r,
		_map->Setup.Background.g,
		_map->Setup.Background.b,
		_map->Setup.Background.a
	);
	SDL_RenderFillRect(_rend, &_map->Setup.DisplayRect);
		
	_map->OnRender(BlockPosition, ScrollPosition);
	
	SDL_RenderSetClipRect(_rend, nullptr);
}

void HorizontalScroller::OnCleanUp()
{

}