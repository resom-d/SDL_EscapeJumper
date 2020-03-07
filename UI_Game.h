#pragma once
#include "GameComponentBase.h"
#include "SDL_Extras.h"
#include "UI_Types.h"
#include "UI_Container.h"

class UI_Game : public GameComponentBase
{
public:
	UI_Game();
	~UI_Game();
	void OnInit(SDL_Renderer* rend, CharacterTextureMap* charMap);	
	void OnLoop(void);
	void OnEvent(SDL_Event* event);
	void OnCleanup(void);
	void OnRender(string playerName, string playerScore, bool playerStatus);

private:
	UI_Container _bbox;
	SDL_Texture* _texHappy;
	SDL_Texture* _texSad;
};

