#pragma once
#include <SDL.h>
#include <stdlib.h>
#include <string>

// indicate the players status
typedef enum
{
	PLAYER_UNDEF_STATUS = 0,
	PLAYER_IDLE_STATUS = 1,
	PLAYER_WALKING_STATUS = 1 << 1,
	PLAYER_RUNNING_STATUS = 1 << 2,
	PLAYER_JUMPING_STATUS = 1 << 3,
	PLAYER_LANDING_STATUS = 1 << 4,
	PLAYER_MOVINGLEFT_STATUS = 1 << 5,
	PLAYER_MOVINGRIGHT_STATUS = 1 << 6,
	PLAYER_MOVINGUP_STATUS = 1 << 7,
	PLAYER_MOVINGDOWN_STATUS = 1 << 8,
	PLAYER_ROTATINGLEFT_STATUS = 1 << 9,
	PLAYER_ROTATINGRIGHT_STATUS = 1 << 10
} PLAYER_MoveStatus;
// make flags useable via | and & operators
inline PLAYER_MoveStatus operator|(PLAYER_MoveStatus a, PLAYER_MoveStatus b)
{
	return static_cast<PLAYER_MoveStatus>(static_cast<long>(a) | static_cast<long>(b));
}
inline PLAYER_MoveStatus operator&(PLAYER_MoveStatus a, PLAYER_MoveStatus b)
{
	return static_cast<PLAYER_MoveStatus>(static_cast<long>(a)& static_cast<long>(b));
}

typedef struct
{
	SDL_Point Position;
	SDL_Rect Frame;
	std::string TextureSourcePath;
	Uint16 HorTiling;
	Uint16 VerTiling;
	PLAYER_MoveStatus LastMovingState;

	std::string Name;
	int Score;
	int Speed;
	double RotationAngle;
	double RotationSpeed;
	bool IsIdle;
	bool IsWalking;
	bool IsRunning;
	bool IsJumping;
	bool IsLanding;
	bool IsMovingLeft;
	bool IsMovingRight;
	bool IsMovingUp;
	bool IsMovingDown;
	bool IsRotatingLeft;
	bool IsRotatingRight;

} PlayerProps;

class GamePlayer
{

public:
	GamePlayer();
	GamePlayer(SDL_Renderer* rend);
	GamePlayer(SDL_Renderer* rend, PlayerProps* props);

	GamePlayer(SDL_Renderer* rend, PlayerProps* props, const char* texturePath, Uint16 tilingX, Uint16 tilingY);

	virtual ~GamePlayer();

	PlayerProps Properties;
	int OnInit();
	int OnLoop(PlayerProps* props);
	int OnRender();
	int OnExit();

private:


protected:
	static void* g_instance;
	static	int g_refcount;

	SDL_Renderer* renderer;
	SDL_Surface* _playerSurface;
	SDL_Texture* _playerTexture;
	char* _textureResourcePath;
	SDL_Rect _playerFrameRect;

};