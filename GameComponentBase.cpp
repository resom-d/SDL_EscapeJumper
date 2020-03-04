#include "GameComponentBase.h"

GameComponentBase::GameComponentBase()
{}

GameComponentBase::~GameComponentBase()
{}

void GameComponentBase::OnInit(SDL_Renderer * rend)
{
	_rend = rend;
}
