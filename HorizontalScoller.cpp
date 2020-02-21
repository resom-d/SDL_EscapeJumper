#include "HorizontalScoller.h"


HorizontalScoller::HorizontalScoller()
{}

// Game Functions
void HorizontalScoller::OnInit(SDL_Renderer* renderer, MatrixSetup* setup, MatrixRectItem** map)
{
	_renderer = renderer;
	MapSetup = setup;
	MapMatrix = map;
	//// Allocate memory for a 2D matrix of tiles
	//MapMatrix = (MatrixRectItem**)malloc(MapSetup->Cols * sizeof(MatrixRectItem*));
	//for (int cols = 0; cols < MapSetup->Cols; cols++)
	//{
	//	MapMatrix[cols] = (MatrixRectItem*)malloc(MapSetup->Rows * sizeof(MatrixRectItem*));
	//}
	// fill the matrix with alpha 0 to make items invisible
}

void HorizontalScoller::InitScollerContent()
{

	MapMatrix[10][0] = { 255,0,0,255 };
	MapMatrix[10][1] = { 0,0,255,255 };
	MapMatrix[10][18] = { 0,0,255,255 };
	MapMatrix[10][19] = { 0,0,255,255 };
	MapMatrix[11][0] = { 255,0,0,255 };
	MapMatrix[11][1] = { 0,0,255,255 };
	MapMatrix[11][18] = { 0,0,255,255 };
	MapMatrix[11][19] = { 0,0,255,255 };
	MapMatrix[12][0] = { 255,0,0,255 };
	MapMatrix[12][1] = { 0,0,255,255 };
	MapMatrix[12][18] = { 0,0,255,255 };
	MapMatrix[12][19] = { 0,0,255,255 };
	MapMatrix[13][0] = { 255,0,0,255 };
	MapMatrix[13][1] = { 0,0,255,255 };
	MapMatrix[13][18] = { 0,0,255,255 };
	MapMatrix[13][19] = { 0,0,255,255 };
	MapMatrix[14][0] = { 255,0,0,255 };
	MapMatrix[14][1] = { 0,0,255,255 };
	MapMatrix[14][18] = { 0,0,255,255 };
	MapMatrix[14][19] = { 0,0,255,255 };
	MapMatrix[15][0] = { 255,0,0,255 };
	MapMatrix[15][1] = { 0,0,255,255 };
	MapMatrix[15][18] = { 0,0,255,255 };
	MapMatrix[15][19] = { 0,0,255,255 };
	MapMatrix[16][0] = { 255,0,0,255 };
	MapMatrix[16][1] = { 0,0,255,255 };
	MapMatrix[16][18] = { 0,0,255,255 };
	MapMatrix[16][19] = { 0,0,255,255 };
	MapMatrix[17][0] = { 255,0,0,255 };
	MapMatrix[17][1] = { 0,0,255,255 };
	MapMatrix[17][18] = { 0,0,255,255 };
	MapMatrix[17][19] = { 0,0,255,255 };
	MapMatrix[18][0] = { 255,0,0,255 };
	MapMatrix[18][1] = { 0,0,255,255 };
	MapMatrix[18][18] = { 0,0,255,255 };
	MapMatrix[18][19] = { 0,0,255,255 };
	MapMatrix[19][0] = { 255,0,0,255 };
	MapMatrix[19][1] = { 0,0,255,255 };
	MapMatrix[19][18] = { 0,0,255,255 };
	MapMatrix[19][19] = { 0,0,255,255 };
	MapMatrix[20][0] = { 255,0,0,255 };
	MapMatrix[20][1] = { 0,0,255,255 };
	MapMatrix[20][18] = { 0,0,255,255 };
	MapMatrix[20][19] = { 0,0,255,255 };

	MapMatrix[21][10] = { 0,0,255,255 };
	MapMatrix[22][12] = { 0,0,255,255 };
	MapMatrix[23][14] = { 0,0,255,255 };
	MapMatrix[24][15] = { 255,0,0,255 };
	MapMatrix[25][17] = { 255,0,0,1 };
	MapMatrix[26][19] = { 255,0,0,255 };

	MapMatrix[30][9] = { 255,0,0,1 };
	MapMatrix[30][10] = { 0,0,255,255 };

	MapMatrix[40][9] = { 0,255,0,1 };
	MapMatrix[40][10] = { 0,255,0,255 };

	MapMatrix[50][0] = { 255,0,0,1 };
	MapMatrix[50][1] = { 255,0,0,255 };
	MapMatrix[50][2] = { 255,0,0,255 };
	MapMatrix[50][17] = { 0,0,255,255 };
	MapMatrix[50][18] = { 0,0,255,255 };
	MapMatrix[50][19] = { 0,0,255,255 };

	MapMatrix[60][0] = { 255,0,0,1 };
	MapMatrix[60][1] = { 255,0,0,255 };
	MapMatrix[60][2] = { 255,0,0,255 };
	MapMatrix[60][17] = { 0,0,255,255 };
	MapMatrix[60][18] = { 0,0,255,255 };
	MapMatrix[60][19] = { 0,0,255,255 };

	MapMatrix[70][0] = { 255,0,0,1 };
	MapMatrix[70][1] = { 255,0,0,255 };
	MapMatrix[70][2] = { 255,0,0,255 };
	MapMatrix[70][17] = { 0,0,255,255 };
	MapMatrix[70][18] = { 0,0,255,255 };
	MapMatrix[70][19] = { 0,0,255,255 };

	MapMatrix[80][5] = { 255,0,0,1 };
	MapMatrix[80][6] = { 255,0,0,255 };
	MapMatrix[80][7] = { 255,0,0,255 };
	MapMatrix[80][12] = { 0,0,255,255 };
	MapMatrix[80][13] = { 0,0,255,255 };
	MapMatrix[80][14] = { 0,0,255,255 };

	MapMatrix[90][6] = { 255,0,0,1 };
	MapMatrix[90][7] = { 255,0,0,255 };
	MapMatrix[90][8] = { 255,0,0,255 };
	MapMatrix[90][11] = { 0,0,255,255 };
	MapMatrix[90][12] = { 0,0,255,255 };
	MapMatrix[90][13] = { 0,0,255,255 };

	MapMatrix[100][7] = { 255,0,0,1 };
	MapMatrix[100][8] = { 255,0,0,255 };
	MapMatrix[100][9] = { 255,0,0,255 };
	MapMatrix[100][10] = { 0,0,255,255 };
	MapMatrix[100][11] = { 0,0,255,255 };
	MapMatrix[100][12] = { 0,0,255,255 };

	MapMatrix[110][9] = { 255,0,0,255 };
	MapMatrix[110][10] = { 0,0,255,255 };
	MapMatrix[111][9] = { 255,0,0,255 };
	MapMatrix[111][10] = { 0,0,255,255 };
	MapMatrix[112][9] = { 0,0,255,255 };


	MapMatrix[113][9] = { 0,0,255,255 };
	MapMatrix[114][10] = { 255,0,0,255 };

	MapMatrix[115][9] = { 0,0,255,255 };
	MapMatrix[116][10] = { 255,0,0,255 };

	MapMatrix[117][9] = { 0,0,255,255 };
	MapMatrix[118][10] = { 255,0,0,255 };
}

void HorizontalScoller::OnLoop()
{
	if (ScrollPosition > MapSetup->BlockSize + MapSetup->BlockSpacing)
	{
		ScrollPosition = ScrollPosition % (MapSetup->BlockSpacing + MapSetup->BlockSpacing);
		if (++ColumnPosition >= MapSetup->Cols - MapSetup->DisplayColumns-1) ColumnPosition = 0;
	}

	ScrollPosition += ScrollSpeed;
}

void HorizontalScoller::OnRender()
{
	if (_renderer == nullptr) return;

	SDL_Rect rect;
	rect.w = MapSetup->BlockSize;
	rect.h = MapSetup->BlockSize;

	for (int x = 0; x < MapSetup->DisplayColumns + 1; x++)
	{
		rect.x = MapSetup->DisplayRect.x + (x * (MapSetup->BlockSize + MapSetup->BlockSpacing) - ScrollPosition);
		for (int y = 0; y < MapSetup->DisplayRows; y++)
		{
			int alpha = MapMatrix[ColumnPosition + x][y].FillColor.a;
			if (alpha < 1) continue;
			int red = MapMatrix[ColumnPosition + x][y].FillColor.r;
			int green = MapMatrix[ColumnPosition + x][y].FillColor.g;
			int blue = MapMatrix[ColumnPosition + x][y].FillColor.b;

			rect.y = MapSetup->DisplayRect.y + (y * (MapSetup->BlockSize + MapSetup->BlockSpacing));

			SDL_RenderSetClipRect(_renderer, &MapSetup->DisplayRect);
			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha);
			SDL_RenderFillRect(_renderer, &rect);

			alpha = MapMatrix[ColumnPosition + x][y].BorderColor.a;
			red = MapMatrix[ColumnPosition + x][y].BorderColor.r;
			green = MapMatrix[ColumnPosition + x][y].BorderColor.g;
			blue = MapMatrix[ColumnPosition + x][y].BorderColor.b;
			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha);
			SDL_RenderDrawRect(_renderer, &rect);

		}
	}

	SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
	SDL_RenderDrawRect(_renderer, &MapSetup->DisplayRect);
}

void HorizontalScoller::OnCleanUp()
{

}