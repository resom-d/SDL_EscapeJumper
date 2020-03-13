#include "JumperPlayer.h"

int JumperPlayer::OnInit(SDL_Renderer* rend, GameMap* map)
{
	_rend = rend;
	_map = map;
	int screenWidth, screenHeight;
	SDL_GetRendererOutputSize(_rend, &screenWidth, &screenHeight);

	SDL_Surface* surf = IMG_Load("Resources/sprites/Block_001.png");
	Texture = SDL_CreateTextureFromSurface(_rend, surf);
	SDL_FreeSurface(surf);

	_sndLand = Mix_LoadWAV("Resources/sndfx/jumpland.wav");

	return 0;
}

int JumperPlayer::OnLoop()
{
		if (MotionVer == MotionState::Plus)
		{
			DisplayRect.y ++;
			if (DisplayRect.y > MaxPosition.y)
			{
				DisplayRect.y = MaxPosition.y;
				Landed = true;				
			}
		}

		if (MotionVer == MotionState::Minus)
		{
			DisplayRect.y --;
			if (DisplayRect.y < MinPosition.y)
			{
				DisplayRect.y = MinPosition.y;
				Landed = true;
			}
		}
		
		OnCollisionCheck();

		return 0;
}

int JumperPlayer::OnRender()
{
	SDL_SetRenderDrawBlendMode(_rend, SDL_BLENDMODE_BLEND);

	SDL_RenderSetClipRect(_rend, &DisplayRect);
	
	int w, h;
	SDL_QueryTexture(Texture, nullptr, nullptr, &w, &h);
	SDL_Rect srcRect = { 0,0, w,h };
	SDL_RenderCopy(_rend, Texture, &srcRect, &DisplayRect);
	return 0;
}

int JumperPlayer::OnCleanup()
{
	return 0;
}

void JumperPlayer::OnEvent(SDL_Event* Event)
{
	switch (Event->type)
	{
	case SDL_KEYDOWN:
		OnKeyDown(Event->key.keysym.sym, Event->key.keysym.mod);
		break;

	case SDL_KEYUP:
		OnKeyUp(Event->key.keysym.sym, Event->key.keysym.mod);
		break;
	}

}

void JumperPlayer::OnKeyDown(SDL_Keycode sym, SDL_Keycode mod)
{
	// player keys
	if (sym == SDLK_UP && Landed)
	{
		MotionVer = MotionState::Minus;
		Landed = false;
		Jumps++;
	}
	if (sym == SDLK_DOWN && Landed)
	{
		MotionVer = MotionState::Plus;
		Landed = false;
		Jumps++;
	}

	if (sym == SDLK_LEFT)
	{
		MotionHor = MotionState::Minus;
	}

	if (sym == SDLK_RIGHT)
	{
		MotionHor = MotionState::Plus;
	}

}

void JumperPlayer::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	if (sym == SDLK_LEFT || sym == SDLK_RIGHT)
	{
		MotionHor = MotionState::None;
	}

}

void JumperPlayer::OnCollisionCheck()
{
	int playerScreenColumn, playerScreenRow;
	int playerEdgeT, playerEdgeL, playerEdgeR, playerEdgeB;
	int obstEdgeT, obstEdgeL, obstEdgeR, obstEdgeB;

	playerScreenColumn = (DisplayRect.x - _map->Setup.ScreenOffsX) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);
	playerScreenRow = (DisplayRect.y - 200) / (_map->Setup.BlockSize + _map->Setup.BlockSpacing);

	playerEdgeT = DisplayRect.y;
	playerEdgeB = DisplayRect.y + _map->Setup.BlockSize;
	playerEdgeL = DisplayRect.x;
	playerEdgeR = DisplayRect.x + _map->Setup.BlockSize;

	// Check if row and column of player make sense - exit if not.
	if (playerScreenColumn < 0 || playerScreenRow < 0 || playerScreenColumn > _map->Setup.DisplayCols || playerScreenRow > _map->Setup.DisplayRows) return;
	// Check 9 possible zones
	for (int x = -1; x < 2; x++)
	{
		for (int y = -1; y <= 1; y++)
		{
			int obstX = playerScreenColumn + x + _map->BlockPosition.x + 1;
			int obstY = playerScreenRow + y + _map->BlockPosition.y;
			if (obstX < 0 || obstY < 0 || obstX > _map->Setup.Cols - 1 || obstY > _map->Setup.Rows - 1) continue;

			TilemapTile* tile = &_map->TileMap[obstX][obstY];
			if (!tile->Visible || !tile->InView) continue;

			obstEdgeT = _map->Setup.DisplayRect.y + ((playerScreenRow + y) * (_map->Setup.BlockSize + _map->Setup.BlockSpacing));
			obstEdgeL = _map->Setup.DisplayRect.x + ((playerScreenColumn + x + 1) * (_map->Setup.BlockSize + _map->Setup.BlockSpacing)) - _map->ScrollPosition.x;

			SDL_Rect rObst =
			{
				obstEdgeL,
				obstEdgeT,
				_map->Setup.BlockSize,
				_map->Setup.BlockSize
			};

			SDL_Rect result;

			if (SDL_IntersectRect(&DisplayRect, &rObst, &result))
			{
				int ri = tile->ResourceIndex - 1;
				TileType tt = next(_map->TextureResources.begin(), ri)->Type;
				if (tt == TileType::Coin)
				{
					Mix_PlayChannel(-1, _sndLand, 0);
					tile->InView = false;
					Score++;
				}
				else
				{
					double d = (double)_map->Setup.BlockSize * 0.5;
					if ((result.w >= result.h || Landed) && (double)result.h < d)
					{
						if (MotionVer == MotionState::Minus)
						{
							DisplayRect.y = rObst.y + _map->Setup.BlockSize;
							Landed = true;
						}
						else if (MotionVer == MotionState::Plus)
						{
							DisplayRect.y = rObst.y - _map->Setup.BlockSize;
							Landed = true;
						}
					}
					else
					{
						GameOver = true;
						return;
					}
				}
			}
			
		}
	}
}
