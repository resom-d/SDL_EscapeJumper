#include "MainUI.h"


MainUI::MainUI()
{}

void MainUI::OnInit(SDL_Renderer* renderer, CharacterMap charMap)
{
	_rend = renderer;
	_charMap = charMap;

	int screenWidth, screenHeight;
	SDL_GetRendererOutputSize(_rend, &screenWidth, &screenHeight);
	
	UI_TextInput ti;
	ti.DisplayRect = { 10, 10, 400, 50 };
	ti.BorderColor = { 0,0,0, 255 };
	ti.Padding = 5;
	
	TextInputs.push_back(ti);

	for (list<UI_TextInput>::iterator iter = TextInputs.begin(); iter != TextInputs.end(); iter++)
	{
		iter->OnInit(_rend, _charMap, "--Nobody--");
	}
}

void MainUI::OnEvent(SDL_Event* event)
{
	for (list<UI_TextInput>::iterator iter = TextInputs.begin(); iter != TextInputs.end(); iter++)
	{
		iter->OnEvent(event);
	}
}

void MainUI::OnRender(std::string playerName, int playerScore, bool gameOver)
{
	int h, w;
	SDL_GetRendererOutputSize(_rend, &w, &h);

	if (_rend == nullptr) return;
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	// Give us Background and a Boarder
	SDL_SetRenderDrawColor(_rend, 168, 127, 50, 255);
	SDL_RenderFillRect(_rend, &DisplayRect);
	SDL_SetRenderDrawColor(_rend, 255, 255, 255, 255);
	SDL_RenderDrawRect(_rend, &DisplayRect);

	SDL_Extras::SDL_RenderStringAt(_rend, "ESCAPE JUMPER", { 150, 20 }, _charMap, 80, &DisplayRect);
}

void MainUI::OnPostRender()
{
	
}

void MainUI::OnCleanup()
{
	for (list<UI_TextInput>::iterator iter = TextInputs.begin(); iter != TextInputs.end(); iter++)
	{
		iter->OnCleanup();
	}
}