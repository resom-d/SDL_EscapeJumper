#include "GameComponentBase.h"

GameComponentBase::GameComponentBase(void)
{}

GameComponentBase::~GameComponentBase(void)
{}

void GameComponentBase::OnInit(SDL_Renderer * rend, CharacterTextureMap* charMap)
{
	_rend = rend;
	_charMap = charMap;
}

void GameComponentBase::OnRender(void)
{}
