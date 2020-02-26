#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
#include "UI_Abstract.h"
#include "UI_Button.h"
#include "UI_Container.h"
#include "UI_TextInput.h"
#include "SDL_Extras.h"
using namespace std;

class MainUI
{
public:
	MainUI();

	void OnInit(SDL_Renderer* renderer, unordered_map<char, SDL_Texture*> charMap);
	void OnLoop();
	void OnEvent(SDL_Event* event);
	void OnRender(std::string  playerName, int playerScore, bool gameOver);
	void OnPostRender();
	void OnCleanup();

	SDL_Renderer* _rend;
	
	unordered_map<char, SDL_Texture*> _charMap;
	SDL_Rect DisplayRect;
	list<UI_Button> Buttons;
	list<UI_TextInput> TextInputs;
	
};

