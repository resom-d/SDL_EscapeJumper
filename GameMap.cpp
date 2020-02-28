#include "GameMap.h"

GameMap::GameMap()
{
	// make some default colors
	ColorPallete.push_back({ 255,255,255,0 }); // Default with zero alpha at position 0
	ColorPallete.push_back({ 255,255,255,255 });
	ColorPallete.push_back({ 0,0,0,255 });
	ColorPallete.push_back({ 255,0,0,255 });
	ColorPallete.push_back({ 0,255,0,255 });
	ColorPallete.push_back({ 0,0,255,255 });
	ColorPallete.push_back({ 255, 238, 0,255 });
	ColorPallete.push_back({ 7, 230, 170,255 });
	ColorPallete.push_back({ 7, 208, 230,255 });
	ColorPallete.push_back({ 255, 3, 230, 255 });
	ColorPallete.push_back({ 255, 183, 0,255 });
	ColorPallete.push_back({ 255, 238, 0,255 });
	ColorPallete.push_back({ 7, 230, 170,255 });
	ColorPallete.push_back({ 7, 208, 230,255 });
	ColorPallete.push_back({ 255, 3, 230, 255 });
	ColorPallete.push_back({ 255, 183, 0,255 });
}

GameMap::~GameMap()
{}

void GameMap::OnInit(SDL_Renderer* rend)
{
	_rend = rend;
	InitMap();
}

void GameMap::OnRender(SDL_Point blockpos, SDL_Point scrollpos)
{

	SDL_RenderSetClipRect(_rend, &Setup.DisplayRect);
	SDL_Rect dRect = { 0,0, Setup.BlockSize, Setup.BlockSize };
	for (int x = 0; x <= Setup.DisplayCols; x++)
	{
		for (int y = 0; y < Setup.DisplayRows; y++)
		{
			if (_map[x + blockpos.x][y + blockpos.y].Visible)
			{
				dRect.x = Setup.DisplayRect.x + (x * (Setup.BlockSize + Setup.BlockSpacing) - scrollpos.x);
				dRect.y = Setup.DisplayRect.y + (y * (Setup.BlockSize + Setup.BlockSpacing) + scrollpos.y);

				ColorPalette::iterator ci = ColorPallete.begin();
				advance(ci, _map[x + blockpos.x][y + blockpos.y].FillColor);
				SDL_SetRenderDrawColor(_rend, ci->r, ci->g, ci->b, ci->a);
				SDL_RenderFillRect(_rend, &dRect);

				ci = ColorPallete.begin();
				advance(ci, _map[x + blockpos.x][y + blockpos.y].BorderColor);
				SDL_SetRenderDrawColor(_rend, ci->r, ci->g, ci->b, ci->a);
				SDL_Extras::SDL_RenderDrawBorder(_rend, dRect, 1, { ci->r, ci->g, ci->b,ci->a });
			}
		}
	}
}

void GameMap::OnCleanUp()
{
	free(_map);
}

void GameMap::InitMap()
{
	_map = (MatrixTile**)malloc(Setup.Cols * sizeof(MatrixTile*));
	for (int i = 0; i < Setup.Cols; i++)
	{
		_map[i] = (MatrixTile*)malloc(Setup.Rows * sizeof(MatrixTile));
	}

	for (int x = 0; x < Setup.Cols; x++)
	{
		for (int y = 0; y < Setup.Rows; y++)
		{
			MatrixTile tile;
			_map[x][y] = tile;
		}
	}
}

void GameMap::SetTileInMap(MapCoords cords, MatrixTile settings)
{
	_map[cords.first][cords.second].BorderColor;
	_map[cords.first][cords.second].Type = settings.Type;
	_map[cords.first][cords.second].TileIndex = settings.TileIndex;
	_map[cords.first][cords.second].Visible = settings.Visible;
	_map[cords.first][cords.second].BorderColor = settings.BorderColor;
	_map[cords.first][cords.second].FillColor = settings.FillColor;

}

void GameMap::FillArea(MapCoords p1, MapCoords p2, MapCoords offs, MatrixTile org)
{
	int minX, minY, maxX, maxY;

	minX = p1.first > p2.first ? p2.first : p1.first;
	minY = p1.second > p2.second ? p2.second : p1.second;
	maxX = p1.first < p2.first ? p2.first : p1.first;
	maxY = p1.second < p2.second ? p2.second : p1.second;


	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			_map[x + offs.first][y + offs.second].Type = org.Type;
			_map[x + offs.first][y + offs.second].TileIndex = org.TileIndex;
			_map[x + offs.first][y + offs.second].FillColor = org.FillColor;
			_map[x + offs.first][y + offs.second].BorderColor = org.BorderColor;
			_map[x + offs.first][y + offs.second].Visible = org.Visible;
		}
	}
}