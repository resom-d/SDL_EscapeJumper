#include "LevelEditor.h"

LevelEditor::LevelEditor()
{}

void LevelEditor::OnInit(SDL_Window* win, SDL_Renderer* rend, MatrixRectItem** map, MatrixSetup* setup)
{
	MapSetup = setup;
	MapMatrix = map;
	_appWindow = win;
	_renderer = rend;
	_colorIndexBorder = 0;
	_colorIndexFill = 0;

	// make some default colors
	ColorPalettte.push_back({ 255,255,255,255 });
	ColorPalettte.push_back({ 0,0,0,255 });
	ColorPalettte.push_back({ 0,0,255,255 });
	ColorPalettte.push_back({ 0,255,0,255 });
	ColorPalettte.push_back({ 255,0,0,255 });
	ColorPalettte.push_back({ 255, 238, 0,255 });
	ColorPalettte.push_back({ 7, 230, 170,255 });
	ColorPalettte.push_back({ 7, 208, 230,255 });
	ColorPalettte.push_back({ 255, 3, 230, 255 });
	ColorPalettte.push_back({ 255, 183, 0,255 });
	// Assign active colors
	list<SDL_Color>::iterator iter = ColorPalettte.begin();
	ActiveFillColor = *iter++;
	_colorIndexFill = 0;
	ActiveBorderColor = *iter;
	_colorIndexBorder = 1;
		
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
{}

void LevelEditor::OnRender()
{
	// Show the Map
	SDL_Rect rect;
	for (int x = 0; x < MapSetup->DisplayColumns; x++)
	{
		for (int y = 0; y < MapSetup->DisplayRows; y++)
		{
			if (MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.a < 1) continue;
			rect.x = MapSetup->ScreenOffsX + (x * (MapSetup->BlockSize + MapSetup->BlockSpacing));
			rect.y = MapSetup->DisplayRect.y + (y * (MapSetup->BlockSize + MapSetup->BlockSpacing));
			rect.w = MapSetup->BlockSize;
			rect.h = MapSetup->BlockSize;

			SDL_RenderSetClipRect(_renderer, &rect);

			SDL_SetRenderDrawColor(
				_renderer,
				MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.r,
				MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.g,
				MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.b,
				MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.a
			);
			SDL_RenderFillRect(_renderer, &rect);

			SDL_SetRenderDrawColor(
				_renderer,
				MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor.r,
				MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor.g,
				MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor.b,
				MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor.a
			);

			SDL_RenderDrawRect(_renderer, &rect);

		}
	}

	SDL_RenderSetClipRect(_renderer, &MapSetup->DisplayRect);
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);

	// Draw a grid
	int x1, y1, x2, y2;
	//Draw horizontal Gridlines
	for (int x = 0; x < MapSetup->DisplayRows + 1; x++)
	{
		x1 = MapSetup->DisplayRect.x;
		x2 = x1 + MapSetup->DisplayRect.w;
		y1 = MapSetup->DisplayRect.y + (x * (MapSetup->BlockSize + MapSetup->BlockSpacing));
		y2 = y1;
		SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
	}
	//Draw vertical Gridlines
	for (int y = 0; y < MapSetup->DisplayColumns + 1; y++)
	{
		x1 = MapSetup->DisplayRect.x + (y * (MapSetup->BlockSize + MapSetup->BlockSpacing));
		x2 = x1;
		y1 = MapSetup->DisplayRect.y;
		y2 = y1 + MapSetup->DisplayRect.h;
		SDL_RenderDrawLine(_renderer, x1, y1, x2, y2);
	}

}

void LevelEditor::OnCleanUp()
{
	free(MapMatrix);
}

void LevelEditor::OnLoadMap()
{}

void LevelEditor::OnSaveMap()
{}

void LevelEditor::OnClearMap()
{}

void LevelEditor::OnEvent(SDL_Event* event)
{
	switch (event->type)
	{
	case SDL_KEYDOWN:
		OnKeyDown(event->key.keysym.sym, event->key.keysym.mod);
		break;

	case SDL_KEYUP:
		OnKeyUp(event->key.keysym.sym, event->key.keysym.mod);
		break;

	case SDL_MOUSEMOTION: {
		OnMouseMove(event->motion.x, event->motion.y, event->motion.xrel, event->motion.yrel, (event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0, (event->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0, (event->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
		break;
	}

	case SDL_MOUSEBUTTONDOWN: {
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT: {
			OnLeftButtonDown(event->button.x, event->button.y);
			break;
		}
		case SDL_BUTTON_RIGHT: {
			OnRightButtonDown(event->button.x, event->button.y);
			break;
		}
		case SDL_BUTTON_MIDDLE: {
			OnMiddleButtonDown(event->button.x, event->button.y);
			break;
		}
		}
		break;
	}

	case SDL_MOUSEBUTTONUP: {
		switch (event->button.button)
		{
		case SDL_BUTTON_LEFT: {
			OnLeftButtonUp(event->button.x, event->button.y);
			break;
		}
		case SDL_BUTTON_RIGHT: {
			OnRightButtonUp(event->button.x, event->button.y);
			break;
		}
		case SDL_BUTTON_MIDDLE: {
			OnMiddleButtonUp(event->button.x, event->button.y);
			break;
		}
		}
		break;
	}
	}
}

void LevelEditor::OnKeyDown(SDL_Keycode sym, SDL_Keycode mod)
{
	list<SDL_Color>::iterator iter = ColorPalettte.begin();
	switch (sym)
	{
	case SDLK_LEFT:
		ColumnPosition--;
		if (ColumnPosition < 0) ColumnPosition = 0;
		break;

	case SDLK_RIGHT:
		ColumnPosition++;
		if (ColumnPosition >= MapSetup->Cols - MapSetup->DisplayColumns - 1) ColumnPosition = MapSetup->Cols - MapSetup->DisplayColumns - 1;
		break;

	case SDLK_UP:
		RowPosition--;
		if (RowPosition < 0) RowPosition = 0;
		break;

	case SDLK_DOWN:
		RowPosition++;
		if (RowPosition >= MapSetup->Rows - MapSetup->DisplayRows - 1) RowPosition = MapSetup->Rows - MapSetup->DisplayRows - 1;
		break;

	case SDLK_d:
		Mode = EditorMode_Draw;
		break;

	case SDLK_e:
		Mode = EditorMode_Erase;
		break;

	case SDLK_f:
		Mode = EditorMode_Fill;
		break;

	case SDLK_s:
		Mode = EditorMode_Select;
		break;

	case SDLK_m:
		Mode = EditorMode_Move;
		break;

	case SDLK_c:
		;
		break;

	case SDLK_1:
		if (--_colorIndexFill < 0) _colorIndexFill = 0;
		advance(iter, _colorIndexFill);
		ActiveFillColor = *iter;
		break;

	case SDLK_2:
		if (++_colorIndexFill > ColorPalettte.size() - 1) _colorIndexFill = ColorPalettte.size() - 1;
		advance(iter, _colorIndexFill);
		ActiveFillColor = *(iter);
		break;

	case SDLK_3:
		if (--_colorIndexBorder < 0) _colorIndexBorder = 0;
		advance(iter, _colorIndexBorder);
		ActiveBorderColor = *iter;
		break;

	case SDLK_4:
		if (++_colorIndexBorder > ColorPalettte.size() - 1) _colorIndexBorder = ColorPalettte.size() - 1;
		advance(iter, _colorIndexBorder);
		ActiveBorderColor = *iter;
		break;

	case SDLK_0:
		_colorIndexFill = 9;
		advance(iter, _colorIndexFill);
		ActiveFillColor = *iter;
		break;
	}
}

void LevelEditor::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{}

void LevelEditor::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle)
{
	int x = (mX - MapSetup->DisplayRect.x) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (x > MapSetup->DisplayColumns - 1 || x < 0) return;
	int y = (mY - MapSetup->DisplayRect.y) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (y > MapSetup->DisplayRows - 1 || y < 0) return;

	if (Mode == EditorMode_Select)
	{
		list<MatrixRectItem>::iterator iter;
		/*iter = find(DrawCollection.begin(), DrawCollection.end(), MapMatrix[x][y]);
		if (iter != DrawCollection.end)
		{
			;
		}*/

		for (iter = DrawCollection.begin(); iter != DrawCollection.end(); ++iter)
		{

		}
		DrawCollection.push_back(MapMatrix[ColumnPosition + x][y + RowPosition]);

	}

	if (Mode == EditorMode_Draw && _drawActive)
	{
		MapMatrix[ColumnPosition + x][y + RowPosition].FillColor = ActiveFillColor;
		MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor = ActiveBorderColor;
	}
	if (Mode == EditorMode_Erase && _drawActive)
	{
		MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor.a = 0;
		MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.a = 0;
	}
}

void LevelEditor::OnLeftButtonDown(int mX, int mY)
{

	int x = (mX - MapSetup->DisplayRect.x) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (x > MapSetup->DisplayColumns - 1 || x < 0) return;
	int y = (mY - MapSetup->DisplayRect.y) / (MapSetup->BlockSize + MapSetup->BlockSpacing);
	if (y > MapSetup->DisplayRows - 1 || y < 0) return;

	ActiveIndex.x = x;
	ActiveIndex.y = y;

	_drawActive = true;

	if (Mode == EditorMode_Select)
	{
		DrawCollection.clear();
		DrawCollection.push_back(MapMatrix[x][y]);
	}


	if (Mode == EditorMode_Draw)
	{
		int alpha = ActiveFillColor.a;
		int red = ActiveFillColor.r;
		int green = ActiveFillColor.g;
		int blue = ActiveFillColor.b;

		MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.a = alpha;
		MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.r = red;
		MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.g = green;
		MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.b = blue;

		alpha = ActiveBorderColor.a;
		red = ActiveBorderColor.r;
		green = ActiveBorderColor.g;
		blue = ActiveBorderColor.b;

		MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor.a = alpha;
		MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor.r = red;
		MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor.g = green;
		MapMatrix[ColumnPosition + x][y + RowPosition].BorderColor.b = blue;
	}

	if (Mode == EditorMode_Erase)
	{
		MapMatrix[ColumnPosition + x][y + RowPosition].FillColor.a = 0;
	}
	
}

void LevelEditor::OnRightButtonDown(int mX, int mY)
{}

void LevelEditor::OnMiddleButtonDown(int mX, int mY)
{}

void LevelEditor::OnLeftButtonUp(int mX, int mY)
{
	_drawActive = false;
}

void LevelEditor::OnRightButtonUp(int mX, int mY)
{}

void LevelEditor::OnMiddleButtonUp(int mX, int mY)
{}
