#include "GameEngine.h"

GameEvents::GameEvents()
{}
GameEvents::~GameEvents() {};

void GameEvents::OnEvent(SDL_Event* Event)
{
	switch (Event->type)
	{
	case SDL_KEYDOWN: {
		OnKeyDown(Event->key.keysym.sym, Event->key.keysym.mod);
		break;
	}

	case SDL_KEYUP: {
		OnKeyUp(Event->key.keysym.sym, Event->key.keysym.mod);
		break;
	}

	case SDL_MOUSEMOTION: {
		OnMouseMove(Event->motion.x, Event->motion.y, Event->motion.xrel, Event->motion.yrel, (Event->motion.state & SDL_BUTTON(SDL_BUTTON_LEFT)) != 0, (Event->motion.state & SDL_BUTTON(SDL_BUTTON_RIGHT)) != 0, (Event->motion.state & SDL_BUTTON(SDL_BUTTON_MIDDLE)) != 0);
		break;
	}

	case SDL_MOUSEBUTTONDOWN: {
		switch (Event->button.button)
		{
		case SDL_BUTTON_LEFT: {
			OnLButtonDown(Event->button.x, Event->button.y);
			break;
		}
		case SDL_BUTTON_RIGHT: {
			OnRButtonDown(Event->button.x, Event->button.y);
			break;
		}
		case SDL_BUTTON_MIDDLE: {
			OnMButtonDown(Event->button.x, Event->button.y);
			break;
		}
		}
		break;
	}

	case SDL_MOUSEBUTTONUP: {
		switch (Event->button.button)
		{
		case SDL_BUTTON_LEFT: {
			OnLButtonUp(Event->button.x, Event->button.y);
			break;
		}
		case SDL_BUTTON_RIGHT: {
			OnRButtonUp(Event->button.x, Event->button.y);
			break;
		}
		case SDL_BUTTON_MIDDLE: {
			OnMButtonUp(Event->button.x, Event->button.y);
			break;
		}
		}
		break;
	}

	case SDL_JOYAXISMOTION: {
		OnJoyAxis(Event->jaxis.which, Event->jaxis.axis, Event->jaxis.value);
		break;
	}

	case SDL_JOYBALLMOTION: {
		OnJoyBall(Event->jball.which, Event->jball.ball, Event->jball.xrel, Event->jball.yrel);
		break;
	}

	case SDL_JOYHATMOTION: {
		OnJoyHat(Event->jhat.which, Event->jhat.hat, Event->jhat.value);
		break;
	}

	case SDL_JOYBUTTONDOWN: {
		OnJoyButtonDown(Event->jbutton.which, Event->jbutton.button);
		break;
	}

	case SDL_JOYBUTTONUP: {
		OnJoyButtonUp(Event->jbutton.which, Event->jbutton.button);
		break;
	}

	case SDL_QUIT: {
		OnExit();
		break;
	}

	case SDL_SYSWMEVENT: {
		//Ignore
		break;
	}


	default: {
		OnUser(Event->user.type, Event->user.code, Event->user.data1, Event->user.data2);
		break;
	}
	}
}

void GameEvents::OnInputFocus()
{
	//Pure virtual, do nothing
}

void GameEvents::OnInputBlur()
{
	//Pure virtual, do nothing
}

void GameEvents::OnKeyDown(SDL_Keycode sym, SDL_Keycode mod)
{

}

void GameEvents::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	//Pure virtual, do nothing
}

void GameEvents::OnMouseFocus()
{
	//Pure virtual, do nothing
}

void GameEvents::OnMouseBlur()
{
	//Pure virtual, do nothing
}

void GameEvents::OnMouseMove(int mX, int mY, int relX, int relY, bool Left, bool Right, bool Middle)
{
	//Pure virtual, do nothing
}

void GameEvents::OnMouseWheel(bool Up, bool Down)
{
	//Pure virtual, do nothing
}

void GameEvents::OnLButtonDown(int mX, int mY)
{
	//Pure virtual, do nothing
}

void GameEvents::OnLButtonUp(int mX, int mY)
{
	//Pure virtual, do nothing
}

void GameEvents::OnRButtonDown(int mX, int mY)
{
	//Pure virtual, do nothing
}

void GameEvents::OnRButtonUp(int mX, int mY)
{
	//Pure virtual, do nothing
}

void GameEvents::OnMButtonDown(int mX, int mY)
{
	//Pure virtual, do nothing
}

void GameEvents::OnMButtonUp(int mX, int mY)
{
	//Pure virtual, do nothing
}

void GameEvents::OnJoyAxis(Uint8 which, Uint8 axis, Sint16 value)
{
	//Pure virtual, do nothing
}

void GameEvents::OnJoyButtonDown(Uint8 which, Uint8 button)
{
	//Pure virtual, do nothing
}

void GameEvents::OnJoyButtonUp(Uint8 which, Uint8 button)
{
	//Pure virtual, do nothing
}

void GameEvents::OnJoyHat(Uint8 which, Uint8 hat, Uint8 value)
{
	//Pure virtual, do nothing
}

void GameEvents::OnJoyBall(Uint8 which, Uint8 ball, Sint16 xrel, Sint16 yrel)
{
	//Pure virtual, do nothing
}

void GameEvents::OnMinimize()
{
	//Pure virtual, do nothing
}

void GameEvents::OnRestore()
{
	//Pure virtual, do nothing
}

void GameEvents::OnResize(int w, int h)
{
	//Pure virtual, do nothing
}

void GameEvents::OnExpose()
{
	//Pure virtual, do nothing
}

void GameEvents::OnExit()
{

}

void GameEvents::OnUser(Uint8 type, int code, void* data1, void* data2)
{
	//Pure virtual, do nothing
}

