#pragma once
#include "UI_Types.h"
#include "GameTypes.h"
#include "UI_Control.h"
#include "UI_Container.h"
#include "UI_GridContainer.h"
#include "UI_TextInput.h"
#include "SDL_Extras.h"
#include "JumperPlayer.h"
using namespace std;

class MainUI
{
public:
	MainUI();
	
	list<path> Avatars;
	list<UI_Control> Buttons;
	SDL_Texture* Texture;
	SDL_Rect DisplayRect;
	UI_TextInput txtPlayerName;

	void OnInit(SDL_Renderer* renderer, JumperPlayer* player);
	void OnLoop();
	void OnEvent(SDL_Event* event);
	void OnRender(bool gameOver);
	void OnPostRender();
	void OnCleanup();

private:
	SDL_Renderer* _rend;
	CharacterTextureMap _charMap;
	TTF_Font* _font;
	JumperPlayer* _player;
};

