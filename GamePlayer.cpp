#include "GamePlayer.h"
#include <SDL.h>


	GamePlayer::GamePlayer()
	{}
	GamePlayer::GamePlayer(SDL_Renderer* rend)
	{}

	GamePlayer::GamePlayer(SDL_Renderer* rend, PlayerProps* props)
	{
		this->renderer = rend;
	}

	GamePlayer::GamePlayer(SDL_Renderer* rend, PlayerProps* props, const char* texturePath, Uint16 tilingX, Uint16 tilingY)
	{

		this->renderer = rend;
	}

	GamePlayer::~GamePlayer()
	{}

	int GamePlayer::OnInit()
	{
		if (renderer == NULL) return -1;

		return 0;
	}

	int GamePlayer::OnLoop(PlayerProps* props)
	{
		if (props->IsMovingUp) props->Position.y -= props->Speed;
		if (props->IsMovingDown) props->Position.y += props->Speed;
		if (props->IsMovingLeft) props->Position.x -= props->Speed;
		if (props->IsMovingRight) props->Position.x -= props->Speed;

		if (props->IsRotatingLeft)
		{
			props->RotationAngle -= props->RotationSpeed;
			if (props->RotationAngle > 360.0) props->RotationAngle -= 360;
		}
		if (props->IsRotatingRight)
		{

			props->RotationAngle += props->RotationSpeed;
			if (props->RotationAngle < 0.0) props->RotationAngle += 360;
		}

		return 0;
	}

	int GamePlayer::OnRender()
	{
		int h = _playerSurface->h;
		int w = _playerSurface->w;
		SDL_Rect tRect = { Properties.Position.x - (w >> 1), Properties.Position.y - (h >> 1), w, h };
		SDL_RendererFlip flip = (Properties.LastMovingState & PLAYER_MOVINGLEFT_STATUS) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
		if ((Properties.LastMovingState & PLAYER_MOVINGDOWN_STATUS) == PLAYER_MOVINGDOWN_STATUS) flip = flip | SDL_FLIP_VERTICAL;

		SDL_RenderCopyEx(renderer, _playerTexture, &_playerFrameRect, &tRect, Properties.RotationAngle, NULL, flip);

		return 0;
	}

	int GamePlayer::OnExit()
	{
		SDL_free(_playerSurface);
		SDL_free(_playerTexture);
		return 0;
	}

