#include "GameMap.h"
#include <fstream>

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
	ColorPallete.push_back({ 0, 200, 10,255 });
	ColorPallete.push_back({ 200, 0, 130,255 });
	ColorPallete.push_back({ 0, 13, 230, 255 });
	ColorPallete.push_back({ 125, 80, 200,255 });
}

GameMap::~GameMap()
{}

void GameMap::OnInit(SDL_Renderer* rend)
{
	_rend = rend;
	InitMap();
	ViewMode = EngineViewMode::Editor;
}

void GameMap::OnRender(SDL_Point blockpos, SDL_Point scrollpos)
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	SDL_RenderSetClipRect(_rend, &Setup.DisplayRect);

	SDL_Rect dRect = { 0,0, Setup.BlockSize, Setup.BlockSize };

	int ctd = Setup.DisplayCols; // Columns to display
	int csi = blockpos.x;				// Column start index
	int cdi = 0;							// Column display index
	if (blockpos.x < 0)
	{
		ctd = Setup.DisplayCols + blockpos.x;
		if (ctd < 0) ctd = 0;
		csi = 0;
		cdi = Setup.DisplayCols - ctd;
	}
	if (blockpos.x > Setup.Cols - Setup.DisplayCols)
	{
		ctd = Setup.Cols - blockpos.x;
		if (ctd > Setup.DisplayCols) ctd = Setup.DisplayCols;
		if (ctd < 0) ctd = 0;
		csi = Setup.Cols - ctd;
		cdi = 0;
	}

	int rtd = Setup.DisplayRows;	// Rows to display
	int rsi = blockpos.y;					// Row start index
	int rdi = 0;								// Row display index
	if (blockpos.y < 0)
	{
		rtd = Setup.DisplayRows + blockpos.y;
		if (rtd < 0) rtd = 0;
		rsi = 0;
		rdi = Setup.DisplayRows - rtd;
	}
	if (blockpos.y > Setup.Rows - Setup.DisplayRows)
	{
		rtd = Setup.Rows - blockpos.y;
		rsi = 0;
	}
	if (rtd < 0) rtd = 0;
	if (rtd > Setup.DisplayRows) ctd = Setup.DisplayRows;

	int yc = -1;
	int xc = -1;
	for (int x = csi; x <= csi + ctd; x++)
	{
		yc = -1;
		xc++;
		for (int y = rsi; y < rtd; y++)
		{
			yc++;
			if (ctd < Setup.DisplayCols)
			{
				int xll = 0;
				xll++;
			}
			TilemapTile tile = TileMap[x][y];

			if ((ViewMode == EngineViewMode::Editor && !tile.Visible) || (ViewMode== EngineViewMode::Game && !tile.InView)) continue;

			int spx = blockpos.x  + Setup.DisplayCols > -1 ? scrollpos.x : -1;
			dRect.x = Setup.DisplayRect.x + ((xc + cdi) * (Setup.BlockSize + Setup.BlockSpacing) - spx + Setup.BlockSpacing);
			dRect.y = Setup.DisplayRect.y + ((yc + rdi) * (Setup.BlockSize + Setup.BlockSpacing) + scrollpos.y + Setup.BlockSpacing);

			if (tile.ResourceIndex < 1 || tile.ResourceIndex > TextureResources.size() || tile.TileIndex < 1)
			{
				ColorPalette::iterator ci = ColorPallete.begin();
				advance(ci, tile.FillColor);
				SDL_SetRenderDrawColor(_rend, ci->r, ci->g, ci->b, ci->a);
				SDL_RenderFillRect(_rend, &dRect);

				ci = ColorPallete.begin();
				advance(ci, tile.BorderColor);
				SDL_SetRenderDrawColor(_rend, ci->r, ci->g, ci->b, ci->a);
				SDL_RenderDrawBorder(_rend, dRect, 1, { ci->r, ci->g, ci->b,ci->a });
			}
			else
			{
				auto tex = TextureResources.begin();
				advance(tex, tile.ResourceIndex - 1);
				SDL_Rect sRect =
				{
					((tile.TileIndex - 1) % tex->Cols) * tex->Tilesize.w,
					((tile.TileIndex - 1) / tex->Cols) * tex->Tilesize.h,
					tex->Tilesize.w,
					tex->Tilesize.h
				};
				SDL_RenderCopy(_rend, tex->Texture, &sRect, &dRect);

			}
		}

	}
}

void GameMap::OnCleanUp()
{
	free(TileMap);
}

void GameMap::InitMap()
{
	TileMap = (TilemapTile**)malloc((Setup.Cols + 1) * sizeof(TilemapTile*));
	for (int i = 0; i < Setup.Cols + 1; i++)
	{
		TileMap[i] = (TilemapTile*)malloc((Setup.Rows + 1) * sizeof(TilemapTile));
	}

	for (int x = 0; x < Setup.Cols + 1; x++)
	{
		for (int y = 0; y < Setup.Rows + 1; y++)
		{
			TilemapTile tile;
			tile.Type = TileType::Background;
			tile.ResourceIndex = 0;
			tile.TileIndex = 0;
			tile.FillColor = 0;
			tile.BorderColor = 0;
			TileMap[x][y] = tile;
		}
	}
}

void GameMap::SaveMap(string filename)
{
	ofstream theFile;
	theFile.open(filename + ".txt");
	theFile << "<- Zehnfinger Tilemap-Description" << endl << endl;;

	theFile << "<- Map-Setup" << endl;
	theFile << "#SET#" << endl;
	theFile << "C:" << to_string(Setup.Cols) << endl;
	theFile << "R:" << to_string(Setup.Rows) << endl;
	theFile << "DC:" << to_string(Setup.DisplayCols) << endl;
	theFile << "DR:" << to_string(Setup.DisplayRows) << endl;
	theFile << "BD:" << to_string(Setup.BlockSize) << endl;
	theFile << "BS::" << to_string(Setup.BlockSpacing) << endl;
	theFile << "BGR:" << to_string(Setup.Background.r) << endl;
	theFile << "BGG:" << to_string(Setup.Background.g) << endl;
	theFile << "BGB:" << to_string(Setup.Background.b) << endl;
	theFile << "BGA:" << to_string(Setup.Background.a) << endl << endl;

	theFile << "<- Texture Resources" << endl;
	if (TextureResources.size() > 0)
	{
		for (auto iter = TextureResources.begin(); iter != TextureResources.end(); iter++)
		{
			theFile << "#RES#" << endl;
			theFile << "P:" << iter->Path << endl;
			theFile << "C:" << to_string(iter->Cols) << endl;
			theFile << "R:" << to_string(iter->Rows) << endl;
			theFile << "M:" << to_string(iter->MaxIndex) << endl;
			theFile << "T:" << to_string(iter->Tilesize.w) << ":" << to_string(iter->Tilesize.h) << endl;
		}
	}

	theFile << endl << "<- Color-Pallete" << endl;
	if (this->ColorPallete.size() > 0)
	{
		theFile << "#COL#" << endl;
		for (auto cIter = this->ColorPallete.begin(); cIter != this->ColorPallete.end(); cIter++)
		{
			theFile << "C:" << to_string(cIter->r) << ":" << to_string(cIter->g) << ":" << to_string(cIter->b) << ":" << to_string(cIter->a) << endl;
		}
	}

	theFile << endl << "<- Tilemap-Tiles" << endl;
	theFile << "#TLM#" << endl;
	for (int x = 0; x <= Setup.Cols; x++)
	{
		for (int y = 0; y <= Setup.Rows; y++)
		{
			TilemapTile tile = TileMap[x][y];

			if (!tile.Visible) continue;

			theFile << "T:"
				<< to_string((int)tile.Type) << ":"
				<< to_string(tile.ResourceIndex) << ":"
				<< to_string(tile.TileIndex) << ":"
				<< to_string(tile.FillColor) << ":"
				<< to_string(tile.BorderColor) << endl;
		}
	}

	theFile.close();
}

void GameMap::LoadMap(string filename)
{}

void GameMap::SetTileInMap(SDL_Point cords, TilemapTile settings)
{
	TileMap[cords.x][cords.y].BorderColor;
	TileMap[cords.x][cords.y].Type = settings.Type;
	TileMap[cords.x][cords.y].ResourceIndex = settings.ResourceIndex;
	TileMap[cords.x][cords.y].TileIndex = settings.TileIndex;
	TileMap[cords.x][cords.y].Visible = settings.Visible;
	TileMap[cords.x][cords.y].BorderColor = settings.BorderColor;
	TileMap[cords.x][cords.y].FillColor = settings.FillColor;

}

void GameMap::FillArea(SDL_Point p1, SDL_Point p2, SDL_Point offs, TilemapTile settings)
{
	int minX, minY, maxX, maxY;

	minX = p1.x > p2.x ? p2.x : p1.x;
	minY = p1.y > p2.y ? p2.y : p1.y;
	maxX = p1.x < p2.x ? p2.x : p1.x;
	maxY = p1.y < p2.y ? p2.y : p1.y;


	for (int x = minX; x <= maxX; x++)
	{
		for (int y = minY; y <= maxY; y++)
		{
			TileMap[x + offs.x][y + offs.y].Type = settings.Type;
			TileMap[x + offs.x][y + offs.y].ResourceIndex = settings.ResourceIndex;
			TileMap[x + offs.x][y + offs.y].TileIndex = settings.TileIndex;
			TileMap[x + offs.x][y + offs.y].FillColor = settings.FillColor;
			TileMap[x + offs.x][y + offs.y].BorderColor = settings.BorderColor;
			TileMap[x + offs.x][y + offs.y].Visible = settings.Visible;
		}
	}
}

TilemapTile GameMap::GetTileAt(Uint16 x, Uint16 y)
{
	return *TileMap[x, y];
}
