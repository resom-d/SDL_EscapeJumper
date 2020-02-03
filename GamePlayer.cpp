#include "GamePlayer.h"
#include <SDL.h>
#include <SDL_image.h>

GamePlayer::GamePlayer()
{
	
}
GamePlayer::GamePlayer(SDL_Renderer* rend){}

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

int GamePlayer::OnInit(SDL_Renderer* rend)
{
	renderer = rend;
	if (renderer == NULL) return -1;

	_playerSurface = IMG_Load(Properties.TextureSourcePath.c_str());
	_playerTexture = SDL_CreateTextureFromSurface(renderer, _playerSurface);
	Properties.Frame.h = _playerSurface->h;
	Properties.Frame.w = _playerSurface->w;
	Properties.HorizontalTileSize = Properties.Frame.w / Properties.HorizontalTiling;
	Properties.VerticalTileSize = Properties.Frame.h / Properties.VerticalTiling;
	return 0;
}

int GamePlayer::OnLoop()
{
	if (Properties.IsMovingUp) Properties.Position.y -= Properties.Speed;
	if (Properties.IsMovingDown) Properties.Position.y += Properties.Speed;
	if (Properties.IsMovingLeft) Properties.Position.x -= Properties.Speed;
	if (Properties.IsMovingRight) Properties.Position.x += Properties.Speed;

	if (Properties.IsRotatingLeft)
	{
		Properties.RotationAngle -= Properties.RotationSpeed;
		if (Properties.RotationAngle > 360.0) Properties.RotationAngle -= 360;
	}
	if (Properties.IsRotatingRight)
	{

		Properties.RotationAngle += Properties.RotationSpeed;
		if (Properties.RotationAngle < 0.0) Properties.RotationAngle += 360;
	}

	return 0;
}

int GamePlayer::OnRender()
{
	SDL_Rect sRect =
	{
		0,
		0,
		Properties.HorizontalTileSize,
		Properties.VerticalTileSize
	};

	SDL_Rect tRect =
	{
		Properties.Position.x - (Properties.HorizontalTileSize >> 1),
		Properties.Position.y - (Properties.VerticalTileSize >> 1),
		Properties.HorizontalTileSize, 
		Properties.VerticalTileSize
	};

	SDL_RendererFlip flip = (Properties.LastMovingState & PLAYER_MOVINGLEFT_STATUS) ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE;
	if ((Properties.LastMovingState & PLAYER_MOVINGDOWN_STATUS) == PLAYER_MOVINGDOWN_STATUS) flip = flip | SDL_FLIP_VERTICAL;

	SDL_RenderCopyEx(renderer, _playerTexture, &sRect, &tRect, Properties.RotationAngle, NULL, flip);

	return 0;
}

int GamePlayer::OnCleanup()
{
	SDL_free(_playerSurface);
	SDL_free(_playerTexture);
	return 0;
}

int GamePlayer::OnExit()
{
	return 0;
}

