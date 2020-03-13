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
	
	list<path> Avatars;
	list<UI_Control> Buttons;
	SDL_Texture* Texture;
	SDL_Rect DisplayRect;

	void OnInit(SDL_Renderer* renderer);
	void OnLoop();
	void OnEvent(SDL_Event* event);
	void OnRender(std::string  playerName, int playerScore, bool gameOver);
	void OnPostRender();
	void OnCleanup();

private:
	SDL_Renderer* _rend;
	
	
};

