#include "UI_TextInput.h"

UI_TextInput::UI_TextInput()
{
	UI_Control::UI_Control();
}

void UI_TextInput::OnInit(SDL_Renderer* rend, CharacterTextureMap charMap)
{
	UI_Control::OnInit(rend);
	_rend = rend;
	_charMap = charMap;
}

void UI_TextInput::OnEvent(SDL_Event* event)
{
	UI_Control::OnEvent(event);

	switch (event->type)
	{
	case SDL_MOUSEBUTTONDOWN:
		if (event->button.button == SDL_BUTTON_LEFT) OnLeftMouseDown(event->button.x, event->button.y);
		break;

	case SDL_KEYDOWN:
		if (!_hasFocus) break;
		if (event->key.keysym.sym == SDLK_BACKSPACE)
		{
			if (TextBuffer->length() > 0) TextBuffer->pop_back();
			break;
		}
		break;

	case SDL_TEXTINPUT:
		if (!_hasFocus) break;
		if (Vocabular.find(event->text.text) != string::npos && TextBuffer->length() < MaxSize)
		{
			TextBuffer->append(event->text.text);
			break;
		}
	}

}

void UI_TextInput::OnRender(void)
{
	UI_Control::OnRender();

	SDL_Rect dRect =
	{
		DisplayRect.x + Padding,
		DisplayRect.y + Padding,
		DisplayRect.w - (Padding << 1),
		DisplayRect.h - (Padding << 1)
	};

	SDL_RenderStringAt(_rend, *TextBuffer, { dRect.x, dRect.y}, _charMap, 28, &dRect);


}

void UI_TextInput::OnCleanup(void)
{}

void UI_TextInput::OnLeftMouseDown(int mx, int my)
{
	_hasFocus = false;
	if (mx > DisplayRect.x + DisplayRect.w || mx < DisplayRect.x || my < DisplayRect.y || my > DisplayRect.y + DisplayRect.h) return;
	_hasFocus = true;

}
