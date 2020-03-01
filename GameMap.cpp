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
			TilemapTile tile = _map[x + blockpos.x][y + blockpos.y];

			if (tile.Visible)
			{
				dRect.x = Setup.DisplayRect.x + (x * (Setup.BlockSize + Setup.BlockSpacing) - scrollpos.x + Setup.BlockSpacing);
				dRect.y = Setup.DisplayRect.y + (y * (Setup.BlockSize + Setup.BlockSpacing) + scrollpos.y +Setup.BlockSpacing);

				if (tile.ResourceIndex < 1 || tile.ResourceIndex > TextureResources.size() || tile.TileIndex < 1)
				{
					ColorPalette::iterator ci = ColorPallete.begin();
					advance(ci, _map[x + blockpos.x][y + blockpos.y].FillColor);
					SDL_SetRenderDrawColor(_rend, ci->r, ci->g, ci->b, ci->a);
					SDL_RenderFillRect(_rend, &dRect);

					ci = ColorPallete.begin();
					advance(ci, _map[x + blockpos.x][y + blockpos.y].BorderColor);
					SDL_SetRenderDrawColor(_rend, ci->r, ci->g, ci->b, ci->a);
					SDL_Extras::SDL_RenderDrawBorder(_rend, dRect, 1, { ci->r, ci->g, ci->b,ci->a });
				}
				else
				{
					auto tex = TextureResources.begin();
					advance(tex, tile.ResourceIndex - 1);
					SDL_Rect sRect =
					{
						((tile.TileIndex -1) % tex->Cols)  * tex->Tilesize.w,
						((tile.TileIndex -1) / tex->Cols)  * tex->Tilesize.h,
						tex->Tilesize.w,
						tex->Tilesize.h

							};
					SDL_RenderCopy(_rend, tex->Texture, &sRect, &dRect);

				}
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
	_map = (TilemapTile**)malloc(Setup.Cols * sizeof(TilemapTile*));
	for (int i = 0; i < Setup.Cols; i++)
	{
		_map[i] = (TilemapTile*)malloc(Setup.Rows * sizeof(TilemapTile));
	}

	for (int x = 0; x < Setup.Cols; x++)
	{
		for (int y = 0; y < Setup.Rows; y++)
		{
			TilemapTile tile = {};
			_map[x][y] = tile;
		}
	}
}

void GameMap::SetTileInMap(MapCoords cords, TilemapTile settings)
{
	_map[cords.first][cords.second].BorderColor;
	_map[cords.first][cords.second].Type = settings.Type;
	_map[cords.first][cords.second].ResourceIndex = settings.ResourceIndex;
	_map[cords.first][cords.second].TileIndex = settings.TileIndex;
	_map[cords.first][cords.second].Visible = settings.Visible;
	_map[cords.first][cords.second].BorderColor = settings.BorderColor;
	_map[cords.first][cords.second].FillColor = settings.FillColor;

}

void GameMap::FillArea(MapCoords p1, MapCoords p2, MapCoords offs, TilemapTile settings)
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
			_map[x + offs.first][y + offs.second].Type = settings.Type;
			_map[x + offs.first][y + offs.second].ResourceIndex = settings.ResourceIndex;
			_map[x + offs.first][y + offs.second].TileIndex = settings.TileIndex;
			_map[x + offs.first][y + offs.second].FillColor = settings.FillColor;
			_map[x + offs.first][y + offs.second].BorderColor = settings.BorderColor;
			_map[x + offs.first][y + offs.second].Visible = settings.Visible;
		}
	}
}