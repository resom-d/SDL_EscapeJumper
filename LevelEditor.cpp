#include "LevelEditor.h"

LevelEditor::LevelEditor()
{}

void LevelEditor::OnInit(SDL_Window* win, SDL_Renderer* rend, GameMap* map, CharacterTextureMap charMap)
{
	_map = map;
	_appWindow = win;
	_rend = rend;
	_charMap = charMap;
	ResourceIndex = 1;
	_colorIndexBorder = 1;
	_colorIndexFill = 2;
	_blockdrawStart = { -1, -1 };
	_blockdrawEnd = { -1, -1 };

	// Assign active colors
	_colorIndexFill = 1;
	_colorIndexBorder = 1;

	UI.DisplayRect = { 0,0, DisplayRect.w, UI_Height };
	UI.OnInit(_rend, _map, _charMap, _map->ColorPallete);

}

void LevelEditor::OnLoop()
{
	UI.OnLoop();
}

void LevelEditor::OnRender()
{
	SDL_RenderSetClipRect(_rend, nullptr);
	SDL_SetRenderDrawColor(_rend, _map->Setup.Background.r, _map->Setup.Background.g, _map->Setup.Background.b, _map->Setup.Background.a);
	SDL_RenderFillRect(_rend, &_map->Setup.DisplayRect);

	// Draw a grid
	SDL_SetRenderDrawColor(_rend, 255, 255, 255, 255);
	for (auto x = 0; x <= _map->Setup.DisplayRows; x++)
	{
		SDL_RenderDrawLine(_rend,
			_map->Setup.DisplayRect.x,
			_map->Setup.DisplayRect.y + (x * (_map->Setup.BlockSize + _map->Setup.BlockSpacing)),
			_map->Setup.DisplayRect.x + DisplayRect.w,
			_map->Setup.DisplayRect.y + (x * (_map->Setup.BlockSize + _map->Setup.BlockSpacing))
		);
	}
	for (auto y = 0; y <= _map->Setup.DisplayCols; y++)
	{
		SDL_RenderDrawLine(_rend,
			_map->Setup.DisplayRect.x + (y * (_map->Setup.BlockSize + _map->Setup.BlockSpacing)),
			_map->Setup.DisplayRect.y,
			_map->Setup.DisplayRect.x + (y * (_map->Setup.BlockSize + _map->Setup.BlockSpacing)),
			_map->Setup.DisplayRect.y + DisplayRect.h
		);
	}

	UI.OnRender(ColumnPosition, RowPosition);
	_map->OnRender({ ColumnPosition, RowPosition }, { ScrollPosition, 0 });

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

void LevelEditor::OnCleanUp()
{
	UI.OnCleanup();

}

void LevelEditor::OnLoadMap()
{}

void LevelEditor::OnSaveMap()
{
	_map->SaveMap(UI.FilenameSave);

}

void LevelEditor::OnClearMap()
{}

void LevelEditor::OnEvent(SDL_Event* event)
{
	if (event->type == EDITOR_EVENT_TYPE)
	{
		Userdata ud;
		list<SDL_Color>::iterator iter;

		switch (event->user.code)
		{
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

		case (Sint32)UI_ACTION::SCROLL_BLOCK_START:
			ColumnPosition = 0;
			break;

		case (Sint32)UI_ACTION::SCROLL_BLOCK_END:
			ColumnPosition = _map->Setup.Cols - _map->Setup.DisplayCols - 1;
			break;

		case (Sint32)UI_ACTION::SCROLL_BLOCK_LEFT:
			ColumnPosition--;
			if (ColumnPosition < 0) ColumnPosition = 0;
			break;

		case (Sint32)UI_ACTION::SCROLL_BLOCK_RIGHT:
			ColumnPosition++;
			if (ColumnPosition > _map->Setup.Cols - _map->Setup.DisplayCols - 1) ColumnPosition = _map->Setup.Cols - _map->Setup.DisplayCols - 1;
			break;

		case (Sint32)UI_ACTION::SET_FILL_COLOR:
			ud = *(Userdata*)event->user.data2;
			if (ud.ColorIndex < 0 || ud.ColorIndex >_map->ColorPallete.size() - 1) return;
			_colorIndexFill = ud.ColorIndex;
			TileIndex = 0;
			break;

		case (Sint32)UI_ACTION::SET_BORDER_COLOR:
			ud = *(Userdata*)event->user.data2;
			if (ud.ColorIndex < 0 || ud.ColorIndex >_map->ColorPallete.size() - 1) return;
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
	case SDLK_q:
		OnSaveMap();
		break;

	case SDLK_LEFT:
		ColumnPosition--;
		if (ColumnPosition < 0) ColumnPosition = 0;
		break;

	case SDLK_RIGHT:
		ColumnPosition++;
		if (ColumnPosition > _map->Setup.Cols - _map->Setup.DisplayCols) ColumnPosition = _map->Setup.Cols - _map->Setup.DisplayCols;
		break;

	case SDLK_UP:
		RowPosition--;
		if (RowPosition < 0) RowPosition = 0;
		break;

	case SDLK_DOWN:
		RowPosition++;
		if (RowPosition > _map->Setup.Rows - _map->Setup.DisplayRows) RowPosition = _map->Setup.Rows - _map->Setup.DisplayRows;
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
		if (++_colorIndexFill > _map->ColorPallete.size() - 1) _colorIndexFill = _map->ColorPallete.size() - 1;
		break;

	case SDLK_3:
		if (--_colorIndexBorder < 1) _colorIndexBorder = 1;
		break;

	case SDLK_4:
		if (++_colorIndexBorder > _map->ColorPallete.size() - 1) _colorIndexBorder = _map->ColorPallete.size() - 1;
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
	if (mX < _map->Setup.DisplayRect.x || mX > _map->Setup.DisplayRect.x + _map->Setup.DisplayRect.w || mY < _map->Setup.DisplayRect.y || mY > _map->Setup.DisplayRect.y + _map->Setup.DisplayRect.h) return;
	int x = (mX - _map->Setup.DisplayRect.x) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (x > _map->Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY - _map->Setup.DisplayRect.y) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (y > _map->Setup.DisplayRows - 1 || y < 0) return;

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

		_map->SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
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

	if (mX < _map->Setup.DisplayRect.x || mX > _map->Setup.DisplayRect.x + _map->Setup.DisplayRect.w || mY < _map->Setup.DisplayRect.y || mY > _map->Setup.DisplayRect.y + _map->Setup.DisplayRect.h) return;
	int x = (mX - _map->Setup.DisplayRect.x) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (x > _map->Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY - _map->Setup.DisplayRect.y) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (y > _map->Setup.DisplayRows - 1 || y < 0) return;

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

		_map->FillArea({ _blockdrawStart.x,  _blockdrawStart.y }, { _blockdrawEnd.x,  _blockdrawEnd.y }, { ColumnPosition, RowPosition }, tile);
	}

}

void LevelEditor::OnRightButtonDown(int mX, int mY)
{
	_eraseActive = true;
	_drawActive = false;

	if (mX < _map->Setup.DisplayRect.x || mX > _map->Setup.DisplayRect.x + _map->Setup.DisplayRect.w || mY < _map->Setup.DisplayRect.y || mY > _map->Setup.DisplayRect.y + _map->Setup.DisplayRect.h) return;
	int x = (mX - _map->Setup.DisplayRect.x) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (x > _map->Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY - _map->Setup.DisplayRect.y) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (y > _map->Setup.DisplayRows - 1 || y < 0) return;

	if (Mode == UI_ACTION::DRAWMODE)
	{
		TilemapTile tile;
		tile.Type = TileType::Background;
		tile.Visible = false;
		tile.TileIndex = 0;
		tile.ResourceIndex = 0;
		tile.FillColor = 0;
		tile.BorderColor = 0;

		_map->SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
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

	if (mX < _map->Setup.DisplayRect.x || mX > _map->Setup.DisplayRect.x + _map->Setup.DisplayRect.w || mY < _map->Setup.DisplayRect.y || mY > _map->Setup.DisplayRect.y + _map->Setup.DisplayRect.h) return;
	int x = (mX - _map->Setup.DisplayRect.x) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (x > _map->Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY - _map->Setup.DisplayRect.y) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (y > _map->Setup.DisplayRows - 1 || y < 0) return;

	for (int row = 0; row < _map->Setup.Cols; row++)
	{
		for (int column = 0; column < _map->Setup.Rows; column++)
		{
			TilemapTile t;
			t = _map->GetTileAt(row, column);
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

		_map->FillArea({ _blockdrawStart.x,  _blockdrawStart.y }, { _blockdrawEnd.x,  _blockdrawEnd.y }, { ColumnPosition, RowPosition }, tile);
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
	if (mX < _map->Setup.DisplayRect.x || mX > _map->Setup.DisplayRect.x + _map->Setup.DisplayRect.w || mY < _map->Setup.DisplayRect.y || mY > _map->Setup.DisplayRect.y + _map->Setup.DisplayRect.h) return;

	int x = (mX - _map->Setup.DisplayRect.x) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (x > _map->Setup.DisplayCols - 1 || x < 0) return;
	int y = (mY - _map->Setup.DisplayRect.y) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (y > _map->Setup.DisplayRows - 1 || y < 0) return;

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

			_map->SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
		}
		if (_eraseActive)
		{
			TilemapTile tile;
			tile.Visible = false;
			tile.ResourceIndex = 0;
			tile.TileIndex = 0;
			tile.FillColor = 0;
			tile.BorderColor = 0;

			_map->SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
		}
	}

	if (Mode == UI_ACTION::BORDERDRAWMODE)
	{
		_blockdrawEnd = { x, y };
		_blockdrawEndScreen = { mX, mY };
	}

	if (_mapScrollMidlleMouse)
	{
		int diff = abs(_mapScrollDiff.x - mX);

		if (diff >= _map->Setup.BlockSize + _map->Setup.BlockSpacing)
		{
			if (_mapScrollDiff.x - mX < 0) ColumnPosition--;
			else ColumnPosition++;

			if (ColumnPosition < 0) ColumnPosition = 0;
			if (ColumnPosition > _map->Setup.Cols - _map->Setup.DisplayCols ) ColumnPosition = _map->Setup.Cols - _map->Setup.DisplayCols;

			_mapScrollDiff.x = mX;
		}
	}

}

