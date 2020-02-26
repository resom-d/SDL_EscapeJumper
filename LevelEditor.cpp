#include "LevelEditor.h"

LevelEditor::LevelEditor()
{}

void LevelEditor::OnInit(SDL_Window* win, SDL_Renderer* rend, MatrixRectItem** map, MatrixSetup* setup)
{
	MapSetup = setup;
	MapMatrix = map;
	_appWindow = win;
	_rend = rend;
	_colorIndexBorder = 1;
	_colorIndexFill = 2;
	_blockdrawStart = { -1, -1 };
	_blockdrawEnd = { -1, -1 };

	// make some default colors
	ColorPalette.push_back({ 255,255,255,0 }); // Default with zero alpha at position 0
	ColorPalette.push_back({ 255,255,255,255 });
	ColorPalette.push_back({ 0,0,0,255 });
	ColorPalette.push_back({ 0,0,255,255 });
	ColorPalette.push_back({ 0,255,0,255 });
	ColorPalette.push_back({ 255,0,0,255 });
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
	// Assign active colors
	list<SDL_Color>::iterator iter = ColorPalette.begin();
	iter++;
	ActiveFillColor = *iter++;
	_colorIndexFill = 1;
	ActiveBorderColor = *iter;
	_colorIndexBorder = 2;

	UI.DisplayRect = { 0,0, DisplayRect.w, UI_Height };
	UI.OnInit(_rend, ColorPalette);

	/*MapSetup->Rows = 100;
	MapSetup->Cols = 1000;
	MapSetup->DisplayRows = 20;
	MapSetup->DisplayColumns = 40;

	MapSetup->BlockSpacing = 1;
	MapSetup->BlockSize = ((_outputHeight - 160) - (MapSetup->DisplayColumns * MapSetup->BlockSpacing)) / MapSetup->DisplayRows;

	MapSetup->DisplayRect.h = (MapSetup->BlockSize + MapSetup->BlockSpacing) * MapSetup->DisplayRows + 1;
	MapSetup->DisplayRect.w = (MapSetup->BlockSize + MapSetup->BlockSpacing) * MapSetup->DisplayColumns + 1;
	MapSetup->ScreenOffsX = (_outputWidth - MapSetup->DisplayRect.w) / 2;
	MapSetup->DisplayRect.x = MapSetup->ScreenOffsX;
	MapSetup->DisplayRect.y = 160;*/

	//// Allocate memory for a 2D matrix of tiles
	//MapMatrix = (MatrixRectItem**)malloc(MapSetup->Cols * sizeof(MatrixRectItem*));
	//for (int cols = 0; cols < MapSetup->Cols; cols++)
	//{
	//	MapMatrix[cols] = (MatrixRectItem*)malloc(MapSetup->Rows * sizeof(MatrixRectItem*));
	//}
	//// fill the matrix with alpha 0 to make items invisible
	//for (int y = 0; y < MapSetup->Rows; y++)
	//{
	//	for (int x = 0; x < MapSetup->Cols; x++)
	//	{
	//		MapMatrix[x][y].BorderColor.a = 0;
	//		MapMatrix[x][y].FillColor.a = 0;
	//	}
	//}
}

void LevelEditor::OnLoop()
{
	UI.OnLoop();
}

void LevelEditor::OnRender()
{
	SDL_RenderSetClipRect(_rend, NULL);
	SDL_Rect rect;

	UI.OnRender();

	// Fill the background
	SDL_RenderSetClipRect(_rend, &MapSetup->DisplayRect);
	SDL_SetRenderDrawColor(_rend,
		MapSetup->Background.r,
		MapSetup->Background.g,
		MapSetup->Background.b,
		MapSetup->Background.a
	);
	SDL_RenderFillRect(_rend, &MapSetup->DisplayRect);

	list<SDL_Color>::iterator iter = ColorPalette.begin();
	// Show the Map
	for (int x = 0; x < MapSetup->DisplayColumns; x++)
	{
		for (int y = 0; y < MapSetup->DisplayRows; y++)
		{
			iter = ColorPalette.begin();
			std::advance(iter, MapMatrix[ColumnPosition + x][y + RowPosition].FillColor);
			if (iter->a < 1) continue;

			rect.x = MapSetup->ScreenOffsX + (x * (MapSetup->BlockSize + MapSetup->BlockSpacing));
			rect.y = MapSetup->DisplayRect.y + (y * (MapSetup->BlockSize + MapSetup->BlockSpacing));
			rect.w = MapSetup->BlockSize;
			rect.h = MapSetup->BlockSize;

			SDL_RenderSetClipRect(_rend, &rect);
			SDL_SetRenderDrawColor(
				_rend,
				iter->r,
				iter->g,
				iter->b,
				iter->a
			);
			SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);
			SDL_RenderFillRect(_rend, &rect);
			SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_NONE);

			iter = ColorPalette.begin();
			std::advance(iter, MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor);
			SDL_SetRenderDrawColor(
				_rend,
				iter->r,
				iter->g,
				iter->b,
				iter->a
			);

			SDL_RenderDrawRect(_rend, &rect);

		}
	}

	SDL_RenderSetClipRect(_rend, &MapSetup->DisplayRect);
	SDL_SetRenderDrawColor(_rend, 255, 255, 255, 255);
	// Draw a grid
	int x1, y1, x2, y2;
	// horizontal Gridlines
	for (int x = 0; x < MapSetup->DisplayRows + 1; x++)
	{
		x1 = MapSetup->DisplayRect.x;
		x2 = x1 + MapSetup->DisplayRect.w;
		y1 = MapSetup->DisplayRect.y + (x * (MapSetup->BlockSize + MapSetup->BlockSpacing));
		y2 = y1;
		SDL_RenderDrawLine(_rend, x1, y1, x2, y2);
	}
	// vertical Gridlines
	for (int y = 0; y < MapSetup->DisplayColumns + 1; y++)
	{
		x1 = MapSetup->DisplayRect.x + (y * (MapSetup->BlockSize + MapSetup->BlockSpacing));
		x2 = x1;
		y1 = MapSetup->DisplayRect.y;
		y2 = y1 + MapSetup->DisplayRect.h;
		SDL_RenderDrawLine(_rend, x1, y1, x2, y2);
	}

	// Show boxselect border
	if (Mode == EDITOR_BORDERDRAWMODE && (_drawActive || _eraseActive))
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
	free(MapMatrix);
}

void LevelEditor::OnLoadMap()
{}

void LevelEditor::OnSaveMap()
{
	ofstream theFile;
	theFile.open("Level-001");
	theFile << "<- Zehnfinger Game-Engine Level-File for MatrixRectItems" << endl << endl;;

	theFile << "<- Map-Setup" << endl;
	theFile << "MXC:," << to_string(MapSetup->Cols) << endl;
	theFile << "MXR:," << to_string(MapSetup->Rows) << endl;
	theFile << "MXDC:," << to_string(MapSetup->DisplayColumns) << endl;
	theFile << "MXDR:," << to_string(MapSetup->DisplayRows) << endl;
	theFile << "MXBZ:," << to_string(MapSetup->BlockSize) << endl;
	theFile << "MXBS:," << to_string(MapSetup->BlockSpacing) << endl;
	theFile << "MXFR:," << to_string(MapSetup->Background.r) << endl;
	theFile << "MXFG:," << to_string(MapSetup->Background.g) << endl;
	theFile << "MXFB:," << to_string(MapSetup->Background.b) << endl;
	theFile << "MXFA:," << to_string(MapSetup->Background.a) << endl << endl;

	theFile << "<- Color-Pallete:" << endl;
	for (_colorPalletIterator = ColorPalette.begin(); _colorPalletIterator != ColorPalette.end(); _colorPalletIterator++)
	{
		theFile << "C:" << ","
			<< to_string(_colorPalletIterator->r) << ","
			<< to_string(_colorPalletIterator->g) << ","
			<< to_string(_colorPalletIterator->b) << ","
			<< to_string(_colorPalletIterator->a) << endl;
	}

	theFile << endl << "<- MATRIX-ITEMS" << endl;
	for (int x = 0; x < MapSetup->Cols; x++)
	{
		for (int y = 0; y < MapSetup->Cols; y++)
		{
			theFile << "M:" << ","
				<< to_string(MapMatrix[x][y].FillColor) << ","
				<< to_string(MapMatrix[x][y].BorderColor) << ","
				<< to_string(x) << "," << to_string(y)
				<< endl;
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
		case EDITOR_DRAWMODE:
			Mode = EDITOR_DRAWMODE;
			break;
		case EDITOR_BORDERDRAWMODE:
			Mode = EDITOR_BORDERDRAWMODE;
			break;

		case EDITOR_SCROLL_BLOCK_START:
			ColumnPosition = 0;
			break;

		case EDITOR_SCROLL_BLOCK_END:
			ColumnPosition = MapSetup->Cols - MapSetup->DisplayColumns - 1;
			break;

		case EDITOR_SCROLL_BLOCK_LEFT:
			ColumnPosition--;
			if (ColumnPosition < 0) ColumnPosition = 0;
			break;

		case EDITOR_SCROLL_BLOCK_RIGHT:
			ColumnPosition++;
			if (ColumnPosition > MapSetup->Cols - MapSetup->DisplayColumns - 1) ColumnPosition = MapSetup->Cols - MapSetup->DisplayColumns - 1;
			break;

		case EDITOR_SET_FILL_COLOR:
			ud = *(Userdata*)event->user.data2;
			if (ud.ColorIndex < 0 || ud.ColorIndex > ColorPalette.size() - 1) return;

			iter = ColorPalette.begin();
			advance(iter, ud.ColorIndex);
			ActiveFillColor = *iter;
			_colorIndexFill = ud.ColorIndex;
			break;

		case EDITOR_SET_BORDER_COLOR:
			ud = *(Userdata*)event->user.data2;
			if (ud.ColorIndex < 0 || ud.ColorIndex > ColorPalette.size() - 1) return;

			iter = ColorPalette.begin();
			advance(iter, ud.ColorIndex);
			ActiveBorderColor = *iter;
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
	list<SDL_Color>::iterator iter = ColorPalette.begin();
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
		if (ColumnPosition > MapSetup->Cols - MapSetup->DisplayColumns - 1) ColumnPosition = MapSetup->Cols - MapSetup->DisplayColumns - 1;
		break;

	case SDLK_UP:
		RowPosition--;
		if (RowPosition < 0) RowPosition = 0;
		break;

	case SDLK_DOWN:
		RowPosition++;
		if (RowPosition > MapSetup->Rows - MapSetup->DisplayRows) RowPosition = MapSetup->Rows - MapSetup->DisplayRows;
		break;

	case SDLK_d:
		Mode = EDITOR_DRAWMODE;
		break;

	case SDLK_e:
		Mode = EDITOR_ERASEMODE;
		break;

	case SDLK_1:
		if (--_colorIndexFill < 1) _colorIndexFill = 1;
		std::advance(iter, _colorIndexFill);
		ActiveFillColor = *iter;
		break;

	case SDLK_2:
		if (++_colorIndexFill > ColorPalette.size() - 1) _colorIndexFill = ColorPalette.size() - 1;
		std::advance(iter, _colorIndexFill);
		ActiveFillColor = *(iter);
		break;

	case SDLK_3:
		if (--_colorIndexBorder < 1) _colorIndexBorder = 1;
		std::advance(iter, _colorIndexBorder);
		ActiveBorderColor = *iter;
		break;

	case SDLK_4:
		if (++_colorIndexBorder > ColorPalette.size() - 1) _colorIndexBorder = ColorPalette.size() - 1;
		std::advance(iter, _colorIndexBorder);
		ActiveBorderColor = *iter;
		break;

	case SDLK_0:
		_colorIndexFill = 9;
		std::advance(iter, _colorIndexFill);
		ActiveFillColor = *iter;
		break;
	}
}

void LevelEditor::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{}


void LevelEditor::OnLeftButtonDown(int mX, int mY)
{
	if (mX < MapSetup->DisplayRect.x || mX > MapSetup->DisplayRect.x + MapSetup->DisplayRect.w || mY < MapSetup->DisplayRect.y || mY > MapSetup->DisplayRect.y + MapSetup->DisplayRect.h) return;
	int x = (mX - MapSetup->DisplayRect.x) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (x > MapSetup->DisplayColumns - 1 || mX < DisplayRect.x) return;
	int y = (mY - MapSetup->DisplayRect.y) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (y > MapSetup->DisplayRows - 1 || y < 0) return;
	ActiveIndex.x = x;
	ActiveIndex.y = y;

	_drawActive = true;

	if (Mode == EDITOR_DRAWMODE)
	{
		_blockdrawStart = { -1, -1 };
		_blockdrawEnd = { -1, -1 };

		_drawActive = true;
		_eraseActive = false;
		MapMatrix[ColumnPosition + x][y + RowPosition].FillColor = _colorIndexFill;
		MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor = _colorIndexBorder;
	}

	if (Mode == EDITOR_BORDERDRAWMODE)
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

	if (mX < MapSetup->DisplayRect.x || mX > MapSetup->DisplayRect.x + MapSetup->DisplayRect.w || mY < MapSetup->DisplayRect.y || mY > MapSetup->DisplayRect.y + MapSetup->DisplayRect.h) return;
	int x = (mX - MapSetup->DisplayRect.x) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (x > MapSetup->DisplayColumns - 1 || mX < DisplayRect.x) return;
	int y = (mY - MapSetup->DisplayRect.y) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (y > MapSetup->DisplayRows - 1 || y < 0) return;

	if (Mode == EDITOR_BORDERDRAWMODE)
	{
		if (_blockdrawStart.x < 0 || _blockdrawStart.y < 0) return;
		_blockdrawEnd.x = x;
		_blockdrawEnd.y = y;
		FillArea(_blockdrawStart, _blockdrawEnd, {ColumnPosition, RowPosition}, _colorIndexFill, _colorIndexBorder);
	}

}

void LevelEditor::OnRightButtonDown(int mX, int mY)
{
	_eraseActive = true;
	_drawActive = false;

	if (mX < MapSetup->DisplayRect.x || mX > MapSetup->DisplayRect.x + MapSetup->DisplayRect.w || mY < MapSetup->DisplayRect.y || mY > MapSetup->DisplayRect.y + MapSetup->DisplayRect.h) return;
	int x = (mX - MapSetup->DisplayRect.x) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (x > MapSetup->DisplayColumns - 1 || mX < DisplayRect.x) return;
	int y = (mY - MapSetup->DisplayRect.y) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (y > MapSetup->DisplayRows - 1 || y < 0) return;
	
	if (Mode == EDITOR_DRAWMODE)
	{
		MapMatrix[ColumnPosition + x][y + RowPosition].FillColor = 0;
	}

	if (Mode == EDITOR_BORDERDRAWMODE)
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

	if (mX < MapSetup->DisplayRect.x || mX > MapSetup->DisplayRect.x + MapSetup->DisplayRect.w || mY < MapSetup->DisplayRect.y || mY > MapSetup->DisplayRect.y + MapSetup->DisplayRect.h) return;
	int x = (mX - MapSetup->DisplayRect.x) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (x > MapSetup->DisplayColumns - 1 || mX < DisplayRect.x) return;
	int y = (mY - MapSetup->DisplayRect.y) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (y > MapSetup->DisplayRows - 1 || y < 0) return;
	
	if (Mode == EDITOR_BORDERDRAWMODE)
	{
		if (_blockdrawStart.x < 0 || _blockdrawStart.y < 0) return;
		_blockdrawEnd.x = x;
		_blockdrawEnd.y = y;
		FillArea(_blockdrawStart, _blockdrawEnd, { ColumnPosition, RowPosition }, 0, 0);
	}
}

void LevelEditor::OnMiddleButtonDown(int mX, int mY)
{}

void LevelEditor::OnMiddleButtonUp(int mX, int mY)
{}

void LevelEditor::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle)
{
	if (mX < MapSetup->DisplayRect.x || mX > MapSetup->DisplayRect.x + MapSetup->DisplayRect.w || mY < MapSetup->DisplayRect.y || mY > MapSetup->DisplayRect.y + MapSetup->DisplayRect.h) return;

	int x = (mX - MapSetup->DisplayRect.x) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (x > MapSetup->DisplayColumns - 1 || x < 0) return;
	int y = (mY - MapSetup->DisplayRect.y) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (y > MapSetup->DisplayRows - 1 || y < 0) return;
	ActiveIndex.x = x;
	ActiveIndex.y = y;

	if (Mode == EDITOR_DRAWMODE)
	{
		if (_drawActive)
		{
			MapMatrix[ColumnPosition + x][y + RowPosition].FillColor = _colorIndexFill;;
			MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor = _colorIndexBorder;
		}
		if (_eraseActive)
		{
			MapMatrix[ColumnPosition + x][y + RowPosition].FillColor = 0;
			MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor = 0;
		}
	}
	
	if (Mode == EDITOR_BORDERDRAWMODE)
	{		
			_blockdrawEnd = { x, y };
			_blockdrawEndScreen = { mX, mY };
	}

}

void LevelEditor::FillArea(SDL_Point p1, SDL_Point p2, SDL_Point offs, Uint16 cf, Uint16 cb)
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
			MapMatrix[x+offs.x][y+offs.y].FillColor = cf;
			MapMatrix[x+offs.x][y+offs.y].BorderColor = cb;
		}
	}

	_blockdrawStart = { -1, -1 };
	_blockdrawEnd = { -1, -1 };
}