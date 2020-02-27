#include "HorizontalScoller.h"


HorizontalScroller::HorizontalScroller()
{}

// Game Functions
void HorizontalScroller::OnInit(SDL_Renderer* renderer, MatrixSetup* setup, MatrixRectItem** map)
{
	_rend = renderer;
	MapSetup = setup;
	MapMatrix = map;
	ScrollPosition = 20;

	// make some default colors
	ColorPalette.push_back({ 255,255,255,0 }); // Default with zero alpha at position 0
	ColorPalette.push_back({ 255,255,255,255 });
	ColorPalette.push_back({ 0,0,0,255 });
	ColorPalette.push_back({ 255,0,0,255 });
	ColorPalette.push_back({ 0,255,0,255 });
	ColorPalette.push_back({ 0,0,255,255 });
	ColorPalette.push_back({ 255, 238, 0,255 });
	ColorPalette.push_back({ 7, 230, 170,255 });
	ColorPalette.push_back({ 7, 208, 230,255 });
	ColorPalette.push_back({ 255, 3, 230, 255 });
	ColorPalette.push_back({ 255, 183, 0,255 });
	ColorPalette.push_back({ 255, 238, 0,255 });
	ColorPalette.push_back({ 7, 230, 170,255 });
	ColorPalette.push_back({ 7, 208, 230,255 });
	ColorPalette.push_back({ 255, 3, 230, 255 });
	ColorPalette.push_back({ 255, 183, 0,255 });

	//// Allocate memory for a 2D matrix of tiles
	//MapMatrix = (MatrixRectItem**)malloc(MapSetup->Cols * sizeof(MatrixRectItem*));
	//for (int cols = 0; cols < MapSetup->Cols; cols++)
	//{
	//	MapMatrix[cols] = (MatrixRectItem*)malloc(MapSetup->Rows * sizeof(MatrixRectItem*));
	//}
	// fill the matrix with alpha 0 to make items invisible
}

void HorizontalScroller::OnLoop()
{
	if (ScrollPosition > MapSetup->BlockSize + MapSetup->BlockSpacing)
	{
		ScrollPosition = ScrollPosition % (MapSetup->BlockSpacing + MapSetup->BlockSpacing);
		if (++ColumnPosition >= MapSetup->Cols - MapSetup->DisplayColumns - 1)
		{
			ColumnPosition = 0;
			ScrollPosition = 0;
			//LevelDone = true;
		}
	}

	ScrollPosition += ScrollSpeed;
}

void HorizontalScroller::OnRender()
{
	if (_rend == nullptr) return;

	SDL_RenderSetClipRect(_rend, &MapSetup->DisplayRect);
	SDL_SetRenderDrawColor(_rend,
		MapSetup->Background.r,
		MapSetup->Background.g,
		MapSetup->Background.b,
		MapSetup->Background.a
	);
	SDL_RenderFillRect(_rend, &MapSetup->DisplayRect);

	SDL_Rect rect;
	rect.w = MapSetup->BlockSize;
	rect.h = MapSetup->BlockSize;

	list<SDL_Color>::iterator iter;

	for (int x = 0; x < MapSetup->DisplayColumns + 1; x++)
	{
		rect.x = MapSetup->DisplayRect.x + (x * (MapSetup->BlockSize + MapSetup->BlockSpacing) - ScrollPosition);
		for (int y = 0; y < MapSetup->DisplayRows; y++)
		{
			iter = ColorPalette.begin();
			advance(iter, MapMatrix[ColumnPosition + x][y].FillColor);
			if (iter->a < 1) continue;

			rect.y = MapSetup->DisplayRect.y + (y * (MapSetup->BlockSize + MapSetup->BlockSpacing));

			SDL_RenderSetClipRect(_rend, &MapSetup->DisplayRect);
			SDL_SetRenderDrawColor(_rend, iter->r, iter->g, iter->b, iter->a);
			SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
			SDL_RenderFillRect(_rend, &rect);
			SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_NONE);

			iter = ColorPalette.begin();
			advance(iter, MapMatrix[ColumnPosition + x][y].BorderColor);
			SDL_Color c = { iter->r, iter->g, iter->b, iter->a };
			SDL_Extras::SDL_RenderDrawBorder(_rend, rect, 2, &c);

		}
	}

	SDL_RenderSetClipRect(_rend, nullptr);
}

void HorizontalScroller::OnCleanUp()
{

}