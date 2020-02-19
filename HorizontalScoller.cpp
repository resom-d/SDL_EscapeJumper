#include "HorizontalScoller.h"


HorizontalScoller::HorizontalScoller()
{}

// Game Functions
void HorizontalScoller::OnInit(SDL_Renderer* renderer)
{
	_renderer = renderer;

	int screenWidth, screenHeight;
	SDL_GetRendererOutputSize(_renderer, &screenWidth, &screenHeight);

	MatrixSetup.ScreenOffsX = (screenWidth - ((MatrixSetup.BlockSize + MatrixSetup.BlockSpacing) * MatrixSetup.DisplayColumns)) / 2;

	MatrixSetup.DisplayRect = { MatrixSetup.ScreenOffsX, 150, (MatrixSetup.BlockSize + MatrixSetup.BlockSpacing) * MatrixSetup.DisplayColumns, (MatrixSetup.BlockSize + MatrixSetup.BlockSpacing) * MatrixSetup.DisplayRows };


	InitScollerContent();
}

void HorizontalScoller::InitScollerContent()
{

	ColumnContainer[10][0] = { 255,0,0,255 };
	ColumnContainer[10][1] = { 0,0,255,255 };
	ColumnContainer[10][18] = { 0,0,255,255 };
	ColumnContainer[10][19] = { 0,0,255,255 };
	ColumnContainer[11][0] = { 255,0,0,255 };
	ColumnContainer[11][1] = { 0,0,255,255 };
	ColumnContainer[11][18] = { 0,0,255,255 };
	ColumnContainer[11][19] = { 0,0,255,255 };
	ColumnContainer[12][0] = { 255,0,0,255 };
	ColumnContainer[12][1] = { 0,0,255,255 };
	ColumnContainer[12][18] = { 0,0,255,255 };
	ColumnContainer[12][19] = { 0,0,255,255 };
	ColumnContainer[13][0] = { 255,0,0,255 };
	ColumnContainer[13][1] = { 0,0,255,255 };
	ColumnContainer[13][18] = { 0,0,255,255 };
	ColumnContainer[13][19] = { 0,0,255,255 };
	ColumnContainer[14][0] = { 255,0,0,255 };
	ColumnContainer[14][1] = { 0,0,255,255 };
	ColumnContainer[14][18] = { 0,0,255,255 };
	ColumnContainer[14][19] = { 0,0,255,255 };
	ColumnContainer[15][0] = { 255,0,0,255 };
	ColumnContainer[15][1] = { 0,0,255,255 };
	ColumnContainer[15][18] = { 0,0,255,255 };
	ColumnContainer[15][19] = { 0,0,255,255 };
	ColumnContainer[16][0] = { 255,0,0,255 };
	ColumnContainer[16][1] = { 0,0,255,255 };
	ColumnContainer[16][18] = { 0,0,255,255 };
	ColumnContainer[16][19] = { 0,0,255,255 };
	ColumnContainer[17][0] = { 255,0,0,255 };
	ColumnContainer[17][1] = { 0,0,255,255 };
	ColumnContainer[17][18] = { 0,0,255,255 };
	ColumnContainer[17][19] = { 0,0,255,255 };
	ColumnContainer[18][0] = { 255,0,0,255 };
	ColumnContainer[18][1] = { 0,0,255,255 };
	ColumnContainer[18][18] = { 0,0,255,255 };
	ColumnContainer[18][19] = { 0,0,255,255 };
	ColumnContainer[19][0] = { 255,0,0,255 };
	ColumnContainer[19][1] = { 0,0,255,255 };
	ColumnContainer[19][18] = { 0,0,255,255 };
	ColumnContainer[19][19] = { 0,0,255,255 };
	ColumnContainer[20][0] = { 255,0,0,255 };
	ColumnContainer[20][1] = { 0,0,255,255 };
	ColumnContainer[20][18] = { 0,0,255,255 };
	ColumnContainer[20][19] = { 0,0,255,255 };

	ColumnContainer[21][10] = { 0,0,255,255 };
	ColumnContainer[22][12] = { 0,0,255,255 };
	ColumnContainer[23][14] = { 0,0,255,255 };
	ColumnContainer[24][15] = { 255,0,0,255 };
	ColumnContainer[25][17] = { 255,0,0,1 };
	ColumnContainer[26][19] = { 255,0,0,255 };

	ColumnContainer[30][9] = { 255,0,0,1 };
	ColumnContainer[30][10] = { 0,0,255,255 };

	ColumnContainer[40][9] = { 0,255,0,1 };
	ColumnContainer[40][10] = { 0,255,0,255 };

	ColumnContainer[50][0] = { 255,0,0,1 };
	ColumnContainer[50][1] = { 255,0,0,255 };
	ColumnContainer[50][2] = { 255,0,0,255 };
	ColumnContainer[50][17] = { 0,0,255,255 };
	ColumnContainer[50][18] = { 0,0,255,255 };
	ColumnContainer[50][19] = { 0,0,255,255 };

	ColumnContainer[60][0] = { 255,0,0,1 };
	ColumnContainer[60][1] = { 255,0,0,255 };
	ColumnContainer[60][2] = { 255,0,0,255 };
	ColumnContainer[60][17] = { 0,0,255,255 };
	ColumnContainer[60][18] = { 0,0,255,255 };
	ColumnContainer[60][19] = { 0,0,255,255 };

	ColumnContainer[70][0] = { 255,0,0,1 };
	ColumnContainer[70][1] = { 255,0,0,255 };
	ColumnContainer[70][2] = { 255,0,0,255 };
	ColumnContainer[70][17] = { 0,0,255,255 };
	ColumnContainer[70][18] = { 0,0,255,255 };
	ColumnContainer[70][19] = { 0,0,255,255 };

	ColumnContainer[80][5] = { 255,0,0,1 };
	ColumnContainer[80][6] = { 255,0,0,255 };
	ColumnContainer[80][7] = { 255,0,0,255 };
	ColumnContainer[80][12] = { 0,0,255,255 };
	ColumnContainer[80][13] = { 0,0,255,255 };
	ColumnContainer[80][14] = { 0,0,255,255 };

	ColumnContainer[90][6] = { 255,0,0,1 };
	ColumnContainer[90][7] = { 255,0,0,255 };
	ColumnContainer[90][8] = { 255,0,0,255 };
	ColumnContainer[90][11] = { 0,0,255,255 };
	ColumnContainer[90][12] = { 0,0,255,255 };
	ColumnContainer[90][13] = { 0,0,255,255 };

	ColumnContainer[100][7] = { 255,0,0,1 };
	ColumnContainer[100][8] = { 255,0,0,255 };
	ColumnContainer[100][9] = { 255,0,0,255 };
	ColumnContainer[100][10] = { 0,0,255,255 };
	ColumnContainer[100][11] = { 0,0,255,255 };
	ColumnContainer[100][12] = { 0,0,255,255 };

	ColumnContainer[110][9] = { 255,0,0,255 };
	ColumnContainer[110][10] = { 0,0,255,255 };
	ColumnContainer[111][9] = { 255,0,0,255 };
	ColumnContainer[111][10] = { 0,0,255,255 };
	ColumnContainer[112][9] = { 0,0,255,255 };


	ColumnContainer[113][9] = { 0,0,255,255 };
	ColumnContainer[114][10] = { 255,0,0,255 };

	ColumnContainer[115][9] = { 0,0,255,255 };
	ColumnContainer[116][10] = { 255,0,0,255 };

	ColumnContainer[117][9] = { 0,0,255,255 };
	ColumnContainer[118][10] = { 255,0,0,255 };
}

void HorizontalScoller::OnLoop()
{
	if (ScrollPosition > MatrixSetup.BlockSize + MatrixSetup.BlockSpacing)
	{
		ScrollPosition = ScrollPosition % (MatrixSetup.BlockSpacing + MatrixSetup.BlockSpacing);
		if (++ColumnPosition > 120) ColumnPosition = 0;
	}

	ScrollPosition += ScrollSpeed;
}

void HorizontalScoller::OnRender()
{
	if (_renderer == nullptr) return;

	SDL_Rect rect;
	rect.w = MatrixSetup.BlockSize;
	rect.h = MatrixSetup.BlockSize;

	//for (int x = 0; x < MatrixSetup.DisplayColumns + 1; x++)
	//{
	//	rect.x = MatrixSetup.ScreenOffsX + (x * (MatrixSetup.BlockSize + MatrixSetup.BlockSpacing));
	//	for (int y = 0; y < MatrixSetup.DisplayRows; y++)
	//	{
	//		rect.y = MatrixSetup.DisplayRect.y + (y * (MatrixSetup.BlockSize + MatrixSetup.BlockSpacing));

	//		SDL_RenderSetClipRect(_renderer, &MatrixSetup.DisplayRect);
	//		SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	//		SDL_RenderDrawRect(_renderer, &rect);
	//	}

	for (int x = 0; x < MatrixSetup.DisplayColumns + 1; x++)
	{
		rect.x = MatrixSetup.DisplayRect.x + (x * (MatrixSetup.BlockSize + MatrixSetup.BlockSpacing) - ScrollPosition);
		for (int y = 0; y < MatrixSetup.DisplayRows; y++)
		{
			int alpha = ColumnContainer[ColumnPosition + x][y].Color.a;
			if (alpha < 1) continue;
			int red = ColumnContainer[ColumnPosition + x][y].Color.r;
			int green = ColumnContainer[ColumnPosition + x][y].Color.g;
			int blue = ColumnContainer[ColumnPosition + x][y].Color.b;

			rect.y = MatrixSetup.DisplayRect.y + (y * (MatrixSetup.BlockSize + MatrixSetup.BlockSpacing));

			SDL_RenderSetClipRect(_renderer, &MatrixSetup.DisplayRect);
			SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha);
			SDL_RenderFillRect(_renderer, &rect);
			SDL_SetRenderDrawColor(_renderer, 0, 0, 0, alpha);
			SDL_RenderDrawRect(_renderer, &rect);

		}
	}
}

void HorizontalScoller::OnCleanUp()
{

}