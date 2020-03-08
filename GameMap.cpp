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

void GameMap::OnRender()
{
	if (Setup.Cols < 1) return;

	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
	SDL_RenderSetClipRect(_rend, &Setup.DisplayRect);
	SDL_Rect dRect = { 0,0, Setup.BlockSize, Setup.BlockSize };
	

	int ctd = Setup.DisplayCols; // Columns to display
	int csi = BlockPosition.x;				// Column start index
	int cdi = 0;							// Column display index
	if (BlockPosition.x < 0)
	{
		ctd = Setup.DisplayCols + BlockPosition.x;
		if (ctd < 0) ctd = 0;
		csi = 0;
		cdi = Setup.DisplayCols - ctd;
	}
	if (BlockPosition.x > Setup.Cols - Setup.DisplayCols)
	{
		ctd = Setup.Cols - BlockPosition.x;
		if (ctd > Setup.DisplayCols) ctd = Setup.DisplayCols;
		if (ctd < 0) ctd = 0;
		csi = Setup.Cols - ctd;
		cdi = 0;
	}

	int rtd = Setup.DisplayRows;	// Rows to display
	int rsi = BlockPosition.y;					// Row start index
	int rdi = 0;								// Row display index
	if (BlockPosition.y < 0)
	{
		rtd = Setup.DisplayRows + BlockPosition.y;
		if (rtd < 0) rtd = 0;
		rsi = 0;
		rdi = Setup.DisplayRows - rtd;
	}
	if (BlockPosition.y > Setup.Rows - Setup.DisplayRows)
	{
		rtd = Setup.Rows - BlockPosition.y;
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

			if ((ViewMode == EngineViewMode::Editor && !tile.Visible) || (ViewMode == EngineViewMode::Game && !tile.InView)) continue;

			int spx = BlockPosition.x + Setup.DisplayCols > -1 ? ScrollPosition.x : -1;
			dRect.x = Setup.DisplayRect.x + ((xc + cdi) * (Setup.BlockSize + Setup.BlockSpacing) - spx + Setup.BlockSpacing);
			dRect.y = Setup.DisplayRect.y + ((yc + rdi) * (Setup.BlockSize + Setup.BlockSpacing) + ScrollPosition.y + Setup.BlockSpacing);

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

void GameMap::OnLoop(void)
{
	if (ScrollPosition.x++ >= Setup.BlockSize + Setup.BlockSpacing)
	{
		ScrollPosition.x = 0;
		if (++BlockPosition.x > Setup.Cols + ScrollXOutDelay)
		{
			LevelDone = true;
			ResetScroller();
		}
	}
}

void GameMap::OnCleanUp()
{
	if(TileMap != nullptr) free(TileMap);
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

void GameMap::ResetScroller(void)
{
	BlockPosition.x = -Setup.DisplayCols + ScrollXInDelay;
	ScrollPosition = { 0, 0 };
}

void GameMap::ResetInView(void)
{
	if (Setup.Cols < 1) return;

	for (auto x = 0; x < Setup.Cols; x++)
	{
		for (auto y = 0; y < Setup.Rows; y++)
		{
			if (!TileMap[x][y].Visible) continue;
			TileMap[x][y].InView = true;
		}
	}
}

void GameMap::SaveMap(string filename)
{
	std::ofstream theFile;
	theFile.open(filename);
	if (!theFile.is_open())
	{
		int nn = 0;
	}

	theFile << "<- Zehnfinger Tilemap-Description" << endl << endl;;
	theFile << "<- Map-Setup" << endl;
	theFile << "#SET#" << endl;
	theFile << "C:" << to_string(Setup.Cols) << endl;
	theFile << "R:" << to_string(Setup.Rows) << endl;
	theFile << "DC:" << to_string(Setup.DisplayCols) << endl;
	theFile << "DR:" << to_string(Setup.DisplayRows) << endl;
	theFile << "BD:" << to_string(Setup.BlockSize) << endl;
	theFile << "BS:" << to_string(Setup.BlockSpacing) << endl;
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
				<< to_string((int)x) << ":"
				<< to_string((int)y) << ":"
				<< to_string((int)tile.Type) << ":"
				<< to_string(tile.ResourceIndex) << ":"
				<< to_string(tile.TileIndex) << ":"
				<< to_string(tile.FillColor) << ":"
				<< to_string(tile.BorderColor) << endl;
		}
	}

	theFile.close();
}

GameMap GameMap::LoadMap(SDL_Renderer* rend, string filename)
{
	GameMap newMap;
	newMap.OnInit(rend);
	newMap.ColorPallete.clear();
	std::fstream fr;
	fr.open(filename.c_str());
	if (!fr.is_open()) return GameMap();
	int tilecount = 0;
	string component="";
	string line = "";
	TileMapTextureResource tr;
	while (getline(fr, line))
	{
		if (line.length() < 2) continue;
		if (line.substr(0, 2) == "<-") continue;

		if (component == "SET")
		{
			list<string> splittedStrings = SplitString(line, ':');
			auto item = splittedStrings.begin();
			if (*item == "C")
			{				
				newMap.Setup.Cols = stoi(*next(item, 1));
			}
			if (*item == "R")
			{
				newMap.Setup.Rows = stoi(*next(item, 1));
			}
			if (*item == "DC")
			{
				newMap.Setup.DisplayCols = stoi(*next(item, 1));
			}
			if (*item == "DR")
			{
				newMap.Setup.DisplayRows = stoi(*next(item, 1));
			}
			if (*item == "BD")
			{
				newMap.Setup.BlockSize = stoi(*next(item, 1));
			}
			if (*item == "BS")
			{
				newMap.Setup.BlockSpacing = stoi(*next(item, 1));
			}
			if (*item == "BGR")
			{
				newMap.Setup.Background.r = stoi(*next(item, 1));
			}
			if (*item == "BGG")
			{
				newMap.Setup.Background.g = stoi(*next(item, 1));
			}
			if (*item == "BGB")
			{
				newMap.Setup.Background.b = stoi(*next(item, 1));
			}
			if (*item == "BGA")
			{
				newMap.Setup.Background.a = stoi(*next(item, 1));
				newMap.InitMap();

				newMap.Setup.DisplayRect =
				{
					newMap.Setup.ScreenOffsX,
					 200,
					(newMap.Setup.BlockSize + newMap.Setup.BlockSpacing) * newMap.Setup.DisplayCols + 1,
					(newMap.Setup.BlockSize + newMap.Setup.BlockSpacing) * newMap.Setup.DisplayRows + 1
				};

			}
		}		
		if (component == "RES")
		{
			list<string> splittedStrings = SplitString(line, ':');
			auto item = splittedStrings.begin();
			if (*item == "P")
			{
				tr.Path = *next(item, 1);
			}
			if (*item == "C")
			{
				tr.Cols = stoi(*next(item, 1));
			}
			if (*item == "R")
			{
				tr.Rows = stoi(*next(item, 1));
			}
			if (*item == "M")
			{
				tr.MaxIndex = stoi(*next(item, 1));
			}
			if (*item == "T")
			{
				SDL_Surface* surf = IMG_Load(tr.Path.c_str());
				tr.Texture = SDL_CreateTextureFromSurface(rend, surf);
				SDL_FreeSurface(surf);
				tr.Tilesize.w = stoi(*next(item, 1));
				tr.Tilesize.h = stoi(*next(item, 2));
				newMap.TextureResources.push_back(tr);
			}			
		}
		if (component == "COL")
		{
			list<string> splittedStrings = SplitString(line, ':');
			auto item = splittedStrings.begin();
			if (*item == "C")
			{
				SDL_Color c =
				{
					stoi(*next(item, 1)),
					stoi(*next(item, 2)),
					stoi(*next(item, 3)),
					stoi(*next(item, 4))
				};
				newMap.ColorPallete.push_back(c);
			}			
		}
		if (component == "TLM")
		{
			list<string> splittedStrings = SplitString(line, ':');
			auto item = splittedStrings.begin();
			if (*item == "T")
			{
				TilemapTile tile;
				int x = stoi(*next(item, 1));
				int y = stoi(*next(item, 2));
				tile.Type = (TileType)stoi(*next(item, 3));
				tile.ResourceIndex = stoi(*next(item, 4));
				tile.TileIndex = stoi(*next(item, 5));
				tile.FillColor = stoi(*next(item, 6));
				tile.BorderColor= stoi(*next(item, 7));
				tile.Visible = true;
				tile.InView = true;

				newMap.TileMap[x][y] = tile;
				tilecount++;
			}
			
		}

		if (line.substr(0, 1) == "#")
		{
			component = line.substr(1, line.length() - 2);
			if (component == "RES") tr = TileMapTextureResource();
		}
	}

	fr.close();

	return newMap;
}

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
