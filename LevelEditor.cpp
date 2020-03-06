#include "LevelEditor.h"

LevelEditor::LevelEditor()
{}

void LevelEditor::OnInit(SDL_Window* win, SDL_Renderer* rend, CharacterTextureMap charMap)
{	
	_appWindow = win;
	_rend = rend;
	_charMap = charMap;
	ResourceIndex = 1;
	_colorIndexBorder = 1;
	_colorIndexFill = 2;
	_blockdrawStart = { -1, -1 };
	_blockdrawEnd = { -1, -1 };

	Map.Setup.Cols = 200;
	Map.Setup.Rows = 20;
	Map.Setup.DisplayCols = 40;
	Map.Setup.DisplayRows = 20;
	Map.Setup.BlockSize = 35;
	Map.Setup.BlockSpacing = 0;
	Map.Setup.ScreenOffsX = 0;
	Map.Setup.DisplayRect =
	{
		Map.Setup.ScreenOffsX,
		 UI_Height,
		(Map.Setup.BlockSize + Map.Setup.BlockSpacing) * Map.Setup.DisplayCols + 1,
		(Map.Setup.BlockSize + Map.Setup.BlockSpacing) * Map.Setup.DisplayRows + 1
	};
	Map.Setup.Background = { 0,0,0, 255 };
	Map.OnInit(_rend);

	TileMapTextureResource res;
	res.Cols = 3;
	res.Rows = 3;
	res.MaxIndex = 9;
	res.Tilesize = Size2D(35, 35);
	res.Path = "Resources/tilemaps/tilemap_001.png";
	SDL_Surface* surf = IMG_Load(res.Path.c_str());
	res.Texture = SDL_CreateTextureFromSurface(_rend, surf);
	SDL_FreeSurface(surf);
	Map.TextureResources.push_back(res);

	// Assign active colors
	_colorIndexFill = 1;
	_colorIndexBorder = 1;

	UI.DisplayRect = { 0,0, DisplayRect.w, UI_Height };
	UI.OnInit(_rend, &Map, _charMap, Map.ColorPallete);
	_confScreen.DisplayRect = DisplayRect;
	_confScreen.OnInit(_rend);
}

void LevelEditor::OnLoop()
{
	UI.OnLoop();
	if(ConfigScreenOn) _confScreen.OnLoop();

	Map.BlockPosition = { ColumnPosition, RowPosition };
	Map.ScrollPosition = { 0,0 };
}

void LevelEditor::OnRender()
{
	if (ConfigScreenOn) _confScreen.OnRender();
	else
	{
		SDL_RenderSetClipRect(_rend, nullptr);
		SDL_SetRenderDrawColor(_rend, Map.Setup.Background.r, Map.Setup.Background.g, Map.Setup.Background.b, Map.Setup.Background.a);
		SDL_RenderFillRect(_rend, &Map.Setup.DisplayRect);

		// Draw a grid
		SDL_SetRenderDrawColor(_rend, 255, 255, 255, 255);
		for (auto x = 0; x <= Map.Setup.DisplayRows; x++)
		{
			SDL_RenderDrawLine(_rend,
				Map.Setup.DisplayRect.x,
				Map.Setup.DisplayRect.y + (x * (Map.Setup.BlockSize + Map.Setup.BlockSpacing)),
				Map.Setup.DisplayRect.x + DisplayRect.w,
				Map.Setup.DisplayRect.y + (x * (Map.Setup.BlockSize + Map.Setup.BlockSpacing))
			);
		}
		for (auto y = 0; y <= Map.Setup.DisplayCols; y++)
		{
			SDL_RenderDrawLine(_rend,
				Map.Setup.DisplayRect.x + (y * (Map.Setup.BlockSize + Map.Setup.BlockSpacing)),
				Map.Setup.DisplayRect.y,
				Map.Setup.DisplayRect.x + (y * (Map.Setup.BlockSize + Map.Setup.BlockSpacing)),
				Map.Setup.DisplayRect.y + DisplayRect.h
			);
		}

		UI.OnRender(ColumnPosition, RowPosition);
		Map.OnRender();

		if (Mode == UI_ACTION::BORDERDRAWMODE && (_drawActive || _eraseActive))
		{
			int minX, minY, maxX, maxY;

			minX = _blockdrawStartScreen.x > _blockdrawEndScreen.x ? _blockdrawEndScreen.x : _blockdrawStartScreen.x;
			minY = _blockdrawStartScreen.y > _blockdrawEndScreen.y ? _blockdrawEndScreen.y : _blockdrawStartScreen.y;
			maxX = _blockdrawStartScreen.x < _blockdrawEndScreen.x ? _blockdrawEndScreen.x : _blockdrawStartScreen.x;
			maxY = _blockdrawStartScreen.y < _blockdrawEndScreen.y ? _blockdrawEndScreen.y : _blockdrawStartScreen.y;

			SDL_Rect r = {
				minX,
				minY,
				maxX - minX,
				maxY - minY
			};

			SDL_RenderSetClipRect(_rend, &r);
			SDL_SetRenderDrawColor(_rend, 0, 200, 0, 255);
			SDL_RenderDrawRect(_rend, &r);
			SDL_RenderSetClipRect(_rend, nullptr);
		}

	}
}

void LevelEditor::OnCleanUp()
{
	UI.OnCleanup();
	_confScreen.OnCleanup();
}

void LevelEditor::OnLoadMap()
{
	GameMap theMap = GameMap::LoadMap(_rend, _confScreen.sFilename + ".txt");
	Map.OnCleanUp();
	Map = theMap;

}

void LevelEditor::OnSaveMap()
{
	Map.SaveMap(_confScreen.sFilename);
}

void LevelEditor::OnClearMap()
{}

void LevelEditor::OnEvent(SDL_Event* event)
{
	if (ConfigScreenOn) _confScreen.OnEvent(event);

	if (event->type == EDITOR_EVENT_TYPE)
	{
		Userdata ud;
		list<SDL_Color>::iterator iter;

		switch (event->user.code)
		{
		case (Sint32)UI_ACTION::GO_EDITOR:
			ConfigScreenOn = false;
			break;

		case (Sint32)UI_ACTION::GO_EDITOR_CONFIG:
			ConfigScreenOn = true;
			break;

		case (Sint32)UI_ACTION::SAVEMAP:
			OnSaveMap();
			break;

		case (Sint32)UI_ACTION::LOADMAP:
			OnLoadMap();
			break;

		case (Sint32)UI_ACTION::SET_TILEINDEX:
			ud = *(Userdata*)event->user.data2;
			TileIndex = ud.TileIndex;
			break;

		case (Sint32)UI_ACTION::DRAWMODE:
			Mode = UI_ACTION::DRAWMODE;
			break;
		case (Sint32)UI_ACTION::BORDERDRAWMODE:
			Mode = UI_ACTION::BORDERDRAWMODE;
			break;

		case (Sint32)UI_ACTION::SCROLL_TO:
			ScrollMap((*(Userdata*)event->user.data2).Scrollposition);
			break;

		case (Sint32)UI_ACTION::SET_FILL_COLOR:
			ud = *(Userdata*)event->user.data2;
			if (ud.ColorIndex < 0 || ud.ColorIndex >Map.ColorPallete.size() - 1) return;
			_colorIndexFill = ud.ColorIndex;
			TileIndex = 0;
			break;

		case (Sint32)UI_ACTION::SET_BORDER_COLOR:
			ud = *(Userdata*)event->user.data2;
			if (ud.ColorIndex < 0 || ud.ColorIndex >Map.ColorPallete.size() - 1) return;
			_colorIndexBorder = ud.ColorIndex;
			TileIndex = 0;
			break;
		}

	}

	switch (event->type)
	{
	case SDL_KEYDOWN:
		OnKeyDown(event->key.keysym.sym, event->key.keysym.mod);
		break;

	case SDL_KEYUP:
		OnKeyUp(event->key.keysym.sym, event->key.keysym.mod);
		break;

	case SDL_MOUSEMOTION:
		OnMouseMove(event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel, (event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0, (event->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0, (event->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
		break;

	case SDL_MOUSEBUTTONDOWN:
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			OnLeftButtonDown(event->button.x, event->button.y);
			break;

		case SDL_BUTTON_RIGHT:
			OnRightButtonDown(event->button.x, event->button.y);
			break;

		case SDL_BUTTON_MIDDLE:
			OnMiddleButtonDown(event->button.x, event->button.y);
			break;

		}
		break;

	case SDL_MOUSEBUTTONUP: {
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT:
			OnLeftButtonUp(event->button.x, event->button.y);
			break;

		case SDL_BUTTON_RIGHT:
			OnRightButtonUp(event->button.x, event->button.y);
			break;

		case SDL_BUTTON_MIDDLE:
			OnMiddleButtonUp(event->button.x, event->button.y);
			break;

		}
		break;
	}

	}

	UI.OnEvent(event);

}

void LevelEditor::OnKeyDown(SDL_Keycode sym, SDL_Keycode mod)
{
	switch (sym)
	{
	case SDLK_LEFT:
		ColumnPosition--;
		if (ColumnPosition < 0) ColumnPosition = 0;
		break;

	case SDLK_RIGHT:
		ColumnPosition++;
		if (ColumnPosition > Map.Setup.Cols - Map.Setup.DisplayCols) ColumnPosition = Map.Setup.Cols - Map.Setup.DisplayCols;
		break;

	case SDLK_UP:
		RowPosition--;
		if (RowPosition < 0) RowPosition = 0;
		break;

	case SDLK_DOWN:
		RowPosition++;
		if (RowPosition > Map.Setup.Rows - Map.Setup.DisplayRows) RowPosition = Map.Setup.Rows - Map.Setup.DisplayRows;
		break;

	case SDLK_d:
		Mode = UI_ACTION::DRAWMODE;
		break;

	case SDLK_e:
		Mode = UI_ACTION::ERASEMODE;
		break;

	case SDLK_1:
		if (--_colorIndexFill < 1) _colorIndexFill = 1;
		break;

	case SDLK_2:
		if (++_colorIndexFill > Map.ColorPallete.size() - 1) _colorIndexFill = Map.ColorPallete.size() - 1;
		break;

	case SDLK_3:
		if (--_colorIndexBorder < 1) _colorIndexBorder = 1;
		break;

	case SDLK_4:
		if (++_colorIndexBorder > Map.ColorPallete.size() - 1) _colorIndexBorder = Map.ColorPallete.size() - 1;
		break;


	case SDLK_0:
		_colorIndexFill = 9;
		break;
	}
}

void LevelEditor::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{}

void LevelEditor::OnLeftButtonDown(int mX, int mY)
{
	if (mX < Map.Setup.DisplayRect.x || mX > Map.Setup.DisplayRect.x + Map.Setup.DisplayRect.w || mY < Map.Setup.DisplayRect.y || mY > Map.Setup.DisplayRect.y + Map.Setup.DisplayRect.h) return;
	int x = (mX - Map.Setup.DisplayRect.x) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (x > Map.Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY - Map.Setup.DisplayRect.y) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (y > Map.Setup.DisplayRows - 1 || y < 0) return;

	_drawActive = true;

	if (Mode == UI_ACTION::DRAWMODE)
	{
		_blockdrawStart = { -1, -1 };
		_blockdrawEnd = { -1, -1 };

		_drawActive = true;
		_eraseActive = false;

		TilemapTile tile;
		tile.Visible = true;
		tile.TileIndex = TileIndex;
		tile.ResourceIndex = ResourceIndex;
		tile.FillColor = _colorIndexFill;
		tile.BorderColor = _colorIndexBorder;

		Map.SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
	}

	if (Mode == UI_ACTION::BORDERDRAWMODE)
	{
		_blockdrawStart.x = x;
		_blockdrawStart.y = y;
		_blockdrawEnd.x = -1;
		_blockdrawEnd.y = -1;

		_blockdrawStartScreen.x = mX;
		_blockdrawStartScreen.y = mY;
		_blockdrawEndScreen.x = mX;
		_blockdrawEndScreen.y = mY;
	}

}

void LevelEditor::OnLeftButtonUp(int mX, int mY)
{
	_drawActive = false;

	if (mX < Map.Setup.DisplayRect.x || mX > Map.Setup.DisplayRect.x + Map.Setup.DisplayRect.w || mY < Map.Setup.DisplayRect.y || mY > Map.Setup.DisplayRect.y + Map.Setup.DisplayRect.h) return;
	int x = (mX - Map.Setup.DisplayRect.x) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (x > Map.Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY - Map.Setup.DisplayRect.y) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (y > Map.Setup.DisplayRows - 1 || y < 0) return;

	if (Mode == UI_ACTION::BORDERDRAWMODE)
	{
		if (_blockdrawStart.x < 0 || _blockdrawStart.y < 0) return;
		_blockdrawEnd.x = x;
		_blockdrawEnd.y = y;

		TilemapTile tile;
		tile.Type = TileType::Background;
		tile.TileIndex = 0;
		tile.FillColor = _colorIndexFill;
		tile.BorderColor = _colorIndexBorder;
		tile.Visible = true;
		tile.TileIndex = TileIndex;
		tile.ResourceIndex = ResourceIndex;

		Map.FillArea({ _blockdrawStart.x,  _blockdrawStart.y }, { _blockdrawEnd.x,  _blockdrawEnd.y }, { ColumnPosition, RowPosition }, tile);
	}

}

void LevelEditor::OnRightButtonDown(int mX, int mY)
{
	_eraseActive = true;
	_drawActive = false;

	if (mX < Map.Setup.DisplayRect.x || mX > Map.Setup.DisplayRect.x + Map.Setup.DisplayRect.w || mY < Map.Setup.DisplayRect.y || mY > Map.Setup.DisplayRect.y + Map.Setup.DisplayRect.h) return;
	int x = (mX - Map.Setup.DisplayRect.x) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (x > Map.Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY - Map.Setup.DisplayRect.y) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (y > Map.Setup.DisplayRows - 1 || y < 0) return;

	if (Mode == UI_ACTION::DRAWMODE)
	{
		TilemapTile tile;
		tile.Type = TileType::Background;
		tile.Visible = false;
		tile.TileIndex = 0;
		tile.ResourceIndex = 0;
		tile.FillColor = 0;
		tile.BorderColor = 0;

		Map.SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
	}

	if (Mode == UI_ACTION::BORDERDRAWMODE)
	{
		_blockdrawStart.x = x;
		_blockdrawStart.y = y;
		_blockdrawEnd.x = -1;
		_blockdrawEnd.y = -1;

		_blockdrawStartScreen.x = mX;
		_blockdrawStartScreen.y = mY;
		_blockdrawEndScreen.x = mX;
		_blockdrawEndScreen.y = mY;
	}
}

void LevelEditor::OnRightButtonUp(int mX, int mY)
{
	_eraseActive = false;
	_drawActive = false;

	if (mX < Map.Setup.DisplayRect.x || mX > Map.Setup.DisplayRect.x + Map.Setup.DisplayRect.w || mY < Map.Setup.DisplayRect.y || mY > Map.Setup.DisplayRect.y + Map.Setup.DisplayRect.h) return;
	int x = (mX - Map.Setup.DisplayRect.x) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (x > Map.Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY - Map.Setup.DisplayRect.y) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (y > Map.Setup.DisplayRows - 1 || y < 0) return;

	for (int row = 0; row < Map.Setup.Cols; row++)
	{
		for (int column = 0; column < Map.Setup.Rows; column++)
		{
			TilemapTile t;
			t = Map.GetTileAt(row, column);
			t.BorderColor = 0;
		}
	}

	if (Mode == UI_ACTION::BORDERDRAWMODE)
	{
		if (_blockdrawStart.x < 0 || _blockdrawStart.y < 0) return;
		_blockdrawEnd.x = x;
		_blockdrawEnd.y = y;

		TilemapTile tile;
		tile.Type = TileType::Background;
		tile.Visible = false;
		tile.TileIndex = 0;
		tile.ResourceIndex = 0;
		tile.FillColor = 0;
		tile.BorderColor = 0;

		Map.FillArea({ _blockdrawStart.x,  _blockdrawStart.y }, { _blockdrawEnd.x,  _blockdrawEnd.y }, { ColumnPosition, RowPosition }, tile);
	}
}

void LevelEditor::OnMiddleButtonDown(int mX, int mY)
{
	_drawActive = false;
	_eraseActive = false;
	_mapScrollMidlleMouse = true;
	_mapScrollDiff.x = mX;
	_mapScrollDiff.y = mY;
}

void LevelEditor::OnMiddleButtonUp(int mX, int mY)
{
	_mapScrollMidlleMouse = false;
}

void LevelEditor::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle)
{
	if (mX < Map.Setup.DisplayRect.x || mX > Map.Setup.DisplayRect.x + Map.Setup.DisplayRect.w || mY < Map.Setup.DisplayRect.y || mY > Map.Setup.DisplayRect.y + Map.Setup.DisplayRect.h) return;

	int x = (mX - Map.Setup.DisplayRect.x) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (x > Map.Setup.DisplayCols - 1 || x < 0) return;
	int y = (mY - Map.Setup.DisplayRect.y) / (Map.Setup.BlockSize + Map.Setup.BlockSpacing);
	if (y > Map.Setup.DisplayRows - 1 || y < 0) return;

	if (Mode == UI_ACTION::DRAWMODE)
	{
		if (_drawActive)
		{
			TilemapTile tile;
			tile.Visible = true;
			tile.ResourceIndex = ResourceIndex;
			tile.TileIndex = TileIndex;
			tile.FillColor = _colorIndexFill;
			tile.BorderColor = _colorIndexBorder;

			Map.SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
		}
		if (_eraseActive)
		{
			TilemapTile tile;
			tile.Visible = false;
			tile.ResourceIndex = 0;
			tile.TileIndex = 0;
			tile.FillColor = 0;
			tile.BorderColor = 0;

			Map.SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
		}
	}

	if (Mode == UI_ACTION::BORDERDRAWMODE)
	{
		_blockdrawEnd = { x, y };
		_blockdrawEndScreen = { mX, mY };
	}

	if (_mapScrollMidlleMouse)
	{
		int diffX = abs(_mapScrollDiff.x - mX);
		int diffY = abs(_mapScrollDiff.y - mY);

		if (diffX >= Map.Setup.BlockSize + Map.Setup.BlockSpacing)
		{
			if (_mapScrollDiff.x - mX < 0) ColumnPosition--;
			else ColumnPosition++;

			if (ColumnPosition < 0) ColumnPosition = 0;
			if (ColumnPosition > Map.Setup.Cols - Map.Setup.DisplayCols) ColumnPosition = Map.Setup.Cols - Map.Setup.DisplayCols;

			_mapScrollDiff.x = mX;
		}

		if (diffY >= Map.Setup.BlockSize + Map.Setup.BlockSpacing)
		{
			if (_mapScrollDiff.y - mY < 0) RowPosition--;
			else RowPosition++;

			if (RowPosition < 0) RowPosition = 0;
			if (RowPosition > Map.Setup.Rows - Map.Setup.DisplayRows) RowPosition = Map.Setup.Rows - Map.Setup.DisplayRows;

			_mapScrollDiff.y = mY;
		}
	}

}

void LevelEditor::ScrollMap(SDL_Point p)
{
	switch (p.x)
	{
	case 3:
		ColumnPosition = Map.Setup.Cols - Map.Setup.DisplayCols;
		break;

	case 2:
		ColumnPosition += Map.Setup.DisplayCols;
		break;

	case 1:
		ColumnPosition++;
		break;
	case -3:
		ColumnPosition = 0;
		break;

	case -2:
		ColumnPosition -= Map.Setup.DisplayCols;
		break;

	case -1:
		ColumnPosition--;
		break;

	}

	switch (p.y)
	{
	case 3:
		RowPosition = Map.Setup.Rows - Map.Setup.DisplayRows;
		break;

	case 2:
		RowPosition += Map.Setup.DisplayRows;
		break;

	case 1:
		RowPosition++;
		break;
	case -3:
		RowPosition = 0;
		break;

	case -2:
		RowPosition -= Map.Setup.DisplayRows;
		break;

	case -1:
		RowPosition--;
		break;

	}

	if (ColumnPosition < 0) ColumnPosition = 0;
	if (ColumnPosition > Map.Setup.Cols - Map.Setup.DisplayCols) ColumnPosition = Map.Setup.Cols - Map.Setup.DisplayCols;

	if (RowPosition < 0) RowPosition = 0;
	if (RowPosition > Map.Setup.Rows - Map.Setup.DisplayRows) RowPosition = Map.Setup.Rows - Map.Setup.DisplayRows;
}

