#include "MainUI.h"


MainUI::MainUI()
{}

void MainUI::OnInit(SDL_Renderer* renderer, CharacterTextureMap charMap)
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
	// Give us Background and a Border
	SDL_Color col = UI_EditorFill;
	SDL_SetRenderDrawColor(_rend, col.r, col.b, col.g, col.a );
	SDL_RenderFillRect(_rend, &DisplayRect);
	col = UI_EditorBorder;
	SDL_SetRenderDrawColor(_rend, col.r, col.b, col.g, col.a);
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