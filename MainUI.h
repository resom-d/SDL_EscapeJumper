#pragma once
#include "UI_Types.h"
#include "GameTypes.h"
#include "UI_Control.h"
#include "UI_Container.h"
#include "UI_GridContainer.h"
#include "UI_TextInput.h"
#include "SDL_Extras.h"
using namespace std;

class MainUI
{
public:
	MainUI();

	void OnInit(SDL_Renderer* renderer, CharacterTextureMap charMap);
	void OnLoop();
	void OnEvent(SDL_Event* event);
	void OnRender(std::string  playerName, int playerScore, bool gameOver);
	void OnPostRender();
	void OnCleanup();

	SDL_Renderer* _rend;
	
	CharacterTextureMap _charMap;
	SDL_Rect DisplayRect;
	list<UI_Control> Buttons;
	
	
};

