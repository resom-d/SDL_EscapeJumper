#include "JumperPlayer.h"

int JumperPlayer::OnInit(SDL_Renderer* rend)
{
	_rend = rend;

	int screenWidth, screenHeight;
	SDL_GetRendererOutputSize(_rend, &screenWidth, &screenHeight);

	return 0;
}

int JumperPlayer::OnLoop()
{
	if (MotionVer == MotionState::Plus)
	{
		DisplayRect.y -= Speed;
		if (DisplayRect.y < MinPosition.y)
		{
			DisplayRect.y = MinPosition.y;
			IsTop = true;
			IsBottom = false;
		}
	}

	if (MotionVer == MotionState:: Minus)
	{
		DisplayRect.y += Speed;
		if (DisplayRect.y > MaxPosition.y)
		{
			DisplayRect.y = MaxPosition.y;
			IsTop = false;
			IsBottom = true;
		}
	}

	if (MotionHor == MotionState::Minus)
	{
		DisplayRect.x -= Speed >> 1;
		if (DisplayRect.x < MinPosition.x)
		{
			DisplayRect.x = MinPosition.x;
		}
	}

	if (MotionHor == MotionState::Plus)
	{
		DisplayRect.x += Speed >> 1;
		if (DisplayRect.x > MaxPosition.x)
		{
			DisplayRect.x = MaxPosition.x;
		}

	}
	
	return 0;
}

int JumperPlayer::OnRender()
{
	SDL_RenderSetClipRect(_rend, &DisplayRect);
	SDL_SetRenderDrawColor(_rend, 24, 141, 199, 255);
	SDL_RenderFillRect(_rend, &DisplayRect);
	SDL_SetRenderDrawColor(_rend, 0, 0, 0, 255);
	SDL_RenderDrawRect(_rend, &DisplayRect);

	return 0;
}

int JumperPlayer::OnCleanup()
{
	return 0;
}

void JumperPlayer::OnEvent(SDL_Event* Event)
{
	switch (Event->type)
	{
	case SDL_KEYDOWN: 
		OnKeyDown(Event->key.keysym.sym, Event->key.keysym.mod);
		break;
	
	case SDL_KEYUP: 
		OnKeyUp(Event->key.keysym.sym, Event->key.keysym.mod);
		break;
	}
	
}

void JumperPlayer::OnKeyDown(SDL_Keycode sym, SDL_Keycode mod)
{
	// player keys
	if (sym == SDLK_UP && (IsBottom||true))
	{
		MotionVer = MotionState::Plus;
		
	}
	if (sym == SDLK_DOWN && (IsTop||true))
	{
		MotionVer = MotionState::Minus;
		
	}

	if (sym == SDLK_LEFT)
	{
		MotionHor = MotionState::Minus;
	}		

	if (sym == SDLK_RIGHT)
	{
		MotionHor = MotionState::Plus;
	}

}

void JumperPlayer::OnKeyUp(SDL_Keycode sym, SDL_Keycode mod)
{
	if (sym == SDLK_LEFT || sym == SDLK_RIGHT)
	{
		MotionHor = MotionState::None;
	}

	if (sym == SDLK_UP && (IsBottom||true))
	{
		MotionVer = MotionState::None;
	}
	if (sym == SDLK_DOWN && (IsTop||true))
	{
		MotionVer = MotionState::None;
	}

}
