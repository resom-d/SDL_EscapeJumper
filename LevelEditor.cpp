#include "LevelEditor.h"

LevelEditor::LevelEditor()
{}

void LevelEditor::OnInit(SDL_Window* win, SDL_Renderer* rend, GameMap* map, CharacterMap charMap)
{
	_map = map;
	_appWindow = win;
	_rend = rend;
	_charMap = charMap;
	_colorIndexBorder = 1;
	_colorIndexFill = 2;
	_blockdrawStart = { -1, -1 };
	_blockdrawEnd = { -1, -1 };
		
	// Assign active colors
	_colorIndexFill = 1;
	_colorIndexBorder = 1;

	UI.DisplayRect = { 0,0, DisplayRect.w, UI_Height };
	UI.OnInit(_rend, _charMap,  _map->ColorPallete);
		
}

void LevelEditor::OnLoop()
{
	UI.OnLoop();
}

void LevelEditor::OnRender()
{
	SDL_RenderSetClipRect(_rend, NULL);
	SDL_Rect rect;

	UI.OnRender(ColumnPosition, RowPosition);

	// Fill the background
	SDL_RenderSetClipRect(_rend, &_map->Setup.DisplayRect);
	SDL_SetRenderDrawColor(_rend,
		_map->Setup.Background.r,
		_map->Setup.Background.g,
		_map->Setup.Background.b,
		_map->Setup.Background.a
	);
	SDL_RenderFillRect(_rend, &_map->Setup.DisplayRect);

	// Render the map
	_map->OnRender({ ColumnPosition,RowPosition }, { 0,0 });

	// Draw a grid
	SDL_RenderSetClipRect(_rend, &_map->Setup.DisplayRect);
	SDL_SetRenderDrawColor(_rend, 255, 255, 255, 255);
	int x1, y1, x2, y2;
	// horizontal Gridlines
	for (int x = 0; x < _map->Setup.DisplayRows + 1; x++)
	{
		x1 = _map->Setup.DisplayRect.x;
		x2 = x1 + _map->Setup.DisplayRect.w;
		y1 = _map->Setup.DisplayRect.y + (x * (_map->Setup.BlockSize + _map->Setup.BlockSpacing));
		y2 = y1;
		SDL_RenderDrawLine(_rend, x1, y1, x2, y2);
	}
	// vertical Gridlines
	for (int y = 0; y < _map->Setup.DisplayCols + 1; y++)
	{
		x1 = _map->Setup.DisplayRect.x + (y * (_map->Setup.BlockSize + _map->Setup.BlockSpacing));
		x2 = x1;
		y1 = _map->Setup.DisplayRect.y;
		y2 = y1 + _map->Setup.DisplayRect.h;
		SDL_RenderDrawLine(_rend, x1, y1, x2, y2);
	}

	// Show boxselect border
	if (Mode == EDITOR_ACTION::BORDERDRAWMODE && (_drawActive || _eraseActive))
	{
		int minX, minY, maxX, maxY;

		minX = _blockdrawStartScreen.x > _blockdrawEndScreen.x ? _blockdrawEndScreen.x : _blockdrawStartScreen.x;
		minY = _blockdrawStartScreen.y > _blockdrawEndScreen.y ? _blockdrawEndScreen.y : _blockdrawStartScreen.y;
		maxX = _blockdrawStartScreen.x < _blockdrawEndScreen.x ? _blockdrawEndScreen.x : _blockdrawStartScreen.x;
		maxY = _blockdrawStartScreen.y < _blockdrawEndScreen.y ? _blockdrawEndScreen.y : _blockdrawStartScreen.y;

		SDL_Rect rect = { minX, minY, maxX - minX, maxY - minY };
		SDL_SetRenderDrawColor(_rend, 0, 255, 0, 255);
		SDL_RenderSetClipRect(_rend, &rect);
		SDL_RenderDrawRect(_rend, &rect);
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
	ofstream theFile;
	theFile.open("Level-001");
	theFile << "<- Zehnfinger Game-Engine Level-File for MatrixRectItems" << endl << endl;;

	theFile << "<- Map-Setup" << endl;
	theFile << "MXC:," << to_string(_map->Setup.Cols) << endl;
	theFile << "MXR:," << to_string(_map->Setup.Rows) << endl;
	theFile << "MXDC:," << to_string(_map->Setup.DisplayCols) << endl;
	theFile << "MXDR:," << to_string(_map->Setup.DisplayRows) << endl;
	theFile << "MXBZ:," << to_string(_map->Setup.BlockSize) << endl;
	theFile << "MXBS:," << to_string(_map->Setup.BlockSpacing) << endl;
	theFile << "MXFR:," << to_string(_map->Setup.Background.r) << endl;
	theFile << "MXFG:," << to_string(_map->Setup.Background.g) << endl;
	theFile << "MXFB:," << to_string(_map->Setup.Background.b) << endl;
	theFile << "MXFA:," << to_string(_map->Setup.Background.a) << endl << endl;

	theFile << "<- Color-Pallete:" << endl;
	

	theFile << endl << "<- MATRIX-ITEMS" << endl;
	for (int x = 0; x < _map->Setup.Cols; x++)
	{
		for (int y = 0; y < _map->Setup.Rows; y++)
		{
			
		}
	}

	theFile.close();

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
		case (Sint32)EDITOR_ACTION::DRAWMODE:
			Mode = EDITOR_ACTION::DRAWMODE;
			break;
		case (Sint32)EDITOR_ACTION::BORDERDRAWMODE:
			Mode = EDITOR_ACTION::BORDERDRAWMODE;
			break;

		case (Sint32)EDITOR_ACTION::SCROLL_BLOCK_START:
			ColumnPosition = 0;
			break;

		case (Sint32)EDITOR_ACTION::SCROLL_BLOCK_END:
			ColumnPosition = _map->Setup.Cols - _map->Setup.DisplayCols - 1;
			break;

		case (Sint32)EDITOR_ACTION::SCROLL_BLOCK_LEFT:
			ColumnPosition--;
			if (ColumnPosition < 0) ColumnPosition = 0;
			break;

		case (Sint32)EDITOR_ACTION::SCROLL_BLOCK_RIGHT:
			ColumnPosition++;
			if (ColumnPosition > _map->Setup.Cols - _map->Setup.DisplayCols - 1) ColumnPosition = _map->Setup.Cols - _map->Setup.DisplayCols - 1;
			break;

		case (Sint32)EDITOR_ACTION::SET_FILL_COLOR:
			ud = *(Userdata*)event->user.data2;
			if (ud.ColorIndex < 0 || ud.ColorIndex > _map->ColorPallete.size() - 1) return;
			_colorIndexFill = ud.ColorIndex;
			break;

		case (Sint32)EDITOR_ACTION::SET_BORDER_COLOR:
			ud = *(Userdata*)event->user.data2;
			if (ud.ColorIndex < 0 || ud.ColorIndex > _map->ColorPallete.size() - 1) return;
			_colorIndexBorder = ud.ColorIndex;
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
		if (ColumnPosition > _map->Setup.Cols - _map->Setup.DisplayCols - 1) ColumnPosition = _map->Setup.Cols - _map->Setup.DisplayCols - 1;
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
		Mode = EDITOR_ACTION::DRAWMODE;
		break;

	case SDLK_e:
		Mode = EDITOR_ACTION::ERASEMODE;
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
	if (mX <  _map->Setup.DisplayRect.x || mX >  _map->Setup.DisplayRect.x + _map->Setup.DisplayRect.w || mY <  _map->Setup.DisplayRect.y || mY >  _map->Setup.DisplayRect.y + _map->Setup.DisplayRect.h) return;
	int x = (mX - _map->Setup.DisplayRect.x) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (x > _map->Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY - _map->Setup.DisplayRect.y) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (y >  _map->Setup.DisplayRows - 1 || y < 0) return;
	ActiveIndex.x = x;
	ActiveIndex.y = y;

	_drawActive = true;

	if (Mode == EDITOR_ACTION::DRAWMODE)
	{
		_blockdrawStart = { -1, -1 };
		_blockdrawEnd = { -1, -1 };

		_drawActive = true;
		_eraseActive = false;
		
		MatrixTile tile;
		tile.Visible = true;
		tile.FillColor = _colorIndexFill;
		tile.BorderColor = _colorIndexBorder;

		_map->SetTileInMap({ ColumnPosition+ x, RowPosition + y }, tile);
	}

	if (Mode == EDITOR_ACTION::BORDERDRAWMODE)
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

	if (mX <  _map->Setup.DisplayRect.x || mX >  _map->Setup.DisplayRect.x + _map->Setup.DisplayRect.w || mY <  _map->Setup.DisplayRect.y || mY >  _map->Setup.DisplayRect.y + _map->Setup.DisplayRect.h) return;
	int x = (mX - _map->Setup.DisplayRect.x) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	if (x >  _map->Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY -  _map->Setup.DisplayRect.y) / ( _map->Setup.BlockSize +  _map->Setup.BlockSpacing);
	if (y >  _map->Setup.DisplayRows - 1 || y < 0) return;

	if (Mode == EDITOR_ACTION::BORDERDRAWMODE)
	{
		if (_blockdrawStart.x < 0 || _blockdrawStart.y < 0) return;
		_blockdrawEnd.x = x;
		_blockdrawEnd.y = y;
		
		MatrixTile tile;
		tile.Type = TileType::Background;
		tile.TileIndex = 0;
		tile.FillColor = _colorIndexFill;
		tile.BorderColor = _colorIndexBorder;
		tile.Visible = true;

		_map->FillArea({ _blockdrawStart.x,  _blockdrawStart.y }, { _blockdrawEnd.x,  _blockdrawEnd.y },  { ColumnPosition, RowPosition }, tile);
	}

}

void LevelEditor::OnRightButtonDown(int mX, int mY)
{
	_eraseActive = true;
	_drawActive = false;

	if (mX <  _map->Setup.DisplayRect.x || mX >  _map->Setup.DisplayRect.x +  _map->Setup.DisplayRect.w || mY <  _map->Setup.DisplayRect.y || mY >  _map->Setup.DisplayRect.y +  _map->Setup.DisplayRect.h) return;
	int x = (mX -  _map->Setup.DisplayRect.x) / ( _map->Setup.BlockSize +  _map->Setup.BlockSpacing);
	if (x >  _map->Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY -  _map->Setup.DisplayRect.y) / ( _map->Setup.BlockSize +  _map->Setup.BlockSpacing);
	if (y >  _map->Setup.DisplayRows - 1 || y < 0) return;
	
	if (Mode == EDITOR_ACTION::DRAWMODE)
	{
		MatrixTile tile;
		tile.Type = TileType::Background;
		tile.TileIndex = 0;
		tile.Visible = false;
		tile.FillColor = 0;
		tile.BorderColor = 0;

		_map->SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
	}

	if (Mode == EDITOR_ACTION::BORDERDRAWMODE)
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

	if (mX <  _map->Setup.DisplayRect.x || mX >  _map->Setup.DisplayRect.x +  _map->Setup.DisplayRect.w || mY <  _map->Setup.DisplayRect.y || mY >  _map->Setup.DisplayRect.y +  _map->Setup.DisplayRect.h) return;
	int x = (mX -  _map->Setup.DisplayRect.x) / ( _map->Setup.BlockSize +  _map->Setup.BlockSpacing);
	if (x >  _map->Setup.DisplayCols - 1 || mX < DisplayRect.x) return;
	int y = (mY -  _map->Setup.DisplayRect.y) / ( _map->Setup.BlockSize +  _map->Setup.BlockSpacing);
	if (y >  _map->Setup.DisplayRows - 1 || y < 0) return;
	
	if (Mode == EDITOR_ACTION::BORDERDRAWMODE)
	{
		if (_blockdrawStart.x < 0 || _blockdrawStart.y < 0) return;
		_blockdrawEnd.x = x;
		_blockdrawEnd.y = y;
		
		MatrixTile tile;
		tile.Type = TileType::Background;
		tile.TileIndex = 0;
		tile.FillColor = 0;
		tile.BorderColor = 0;
		tile.Visible = false;

		_map->FillArea({ _blockdrawStart.x,  _blockdrawStart.y }, { _blockdrawEnd.x,  _blockdrawEnd.y }, { ColumnPosition, RowPosition }, tile);
	}
}

void LevelEditor::OnMiddleButtonDown(int mX, int mY)
{}

void LevelEditor::OnMiddleButtonUp(int mX, int mY)
{}

void LevelEditor::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle)
{
	if (mX <  _map->Setup.DisplayRect.x || mX >  _map->Setup.DisplayRect.x +  _map->Setup.DisplayRect.w || mY <  _map->Setup.DisplayRect.y || mY >  _map->Setup.DisplayRect.y +  _map->Setup.DisplayRect.h) return;

	int x = (mX -  _map->Setup.DisplayRect.x) / ( _map->Setup.BlockSize +  _map->Setup.BlockSpacing);
	if (x >  _map->Setup.DisplayCols - 1 || x < 0) return;
	int y = (mY -  _map->Setup.DisplayRect.y) / ( _map->Setup.BlockSize +  _map->Setup.BlockSpacing);
	if (y >  _map->Setup.DisplayRows - 1 || y < 0) return;
	ActiveIndex.x = x;
	ActiveIndex.y = y;

	if (Mode == EDITOR_ACTION::DRAWMODE)
	{
		if (_drawActive)
		{
			MatrixTile tile;
			tile.Visible = true;
			tile.FillColor = _colorIndexFill;
			tile.BorderColor = _colorIndexBorder;

			_map->SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
		}
		if (_eraseActive)
		{
			MatrixTile tile;
			tile.Type = TileType::Background;
			tile.TileIndex = 0;
			tile.Visible = false;
			tile.FillColor = 0;
			tile.BorderColor =0;

			_map->SetTileInMap({ ColumnPosition + x, RowPosition + y }, tile);
		}
	}
	
	if (Mode == EDITOR_ACTION::BORDERDRAWMODE)
	{		
			_blockdrawEnd = { x, y };
			_blockdrawEndScreen = { mX, mY };
	}

}

