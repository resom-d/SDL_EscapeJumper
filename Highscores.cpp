#include "Highscores.h"

Highscores::Highscores()
{
}

Highscores::~Highscores()
{
}

void Highscores::OnInit(SDL_Renderer* rend)
{
	_rend = rend;
	LoadHighscores();
	HighScores.sort(ScoreComparator());
	_font = TTF_OpenFont("Resources/fonts/LuckiestGuy-Regular.ttf", 72);
	SDL_Surface* surf = IMG_Load("Resources/bgnd/HighscoresBackground.png");
	_background = SDL_CreateTextureFromSurface(_rend, surf);
	SDL_FreeSurface(surf);
	_scrollerRect = { 560, 210, 800, 490 };
	string s = "Spiel dir die Finger Wund um in die Hall of Fame zu kommen. Dein Name wird dann ewig leben und die Welt wird dir zu Füßen liegen. Gib alles!";
	_msgScroller.DisplayRect = { 200, 810, 1130, 80 };
	_msgScroller.OnInit(_rend, s, _font, { 85,0,255,255 }, 4);
	

}

void Highscores::OnLoop(void)
{
	if (++_loopCnt > _lastItemHeight)
	{
		_loopCnt = 0;
		if (_itemsIndex < HighScores.size())
		{
			ScoreScreenItem si;
			si.Item = *next(HighScores.begin(), _itemsIndex);
			string st = to_string(_itemsIndex + 1) + ". " +  si.Item.Name + "   " + to_string(si.Item.Score);
			SDL_Surface* surf = TTF_RenderText_Solid(_font, st.c_str(), { 80, 0, 255, 255 });
			SDL_Texture* tex = SDL_CreateTextureFromSurface(_rend, surf);
			SDL_FreeSurface(surf);
			si.Texture = tex;
			int w, h;
			SDL_QueryTexture(tex, nullptr, nullptr, &w, &h);
			si.Width = w;
			si.Height = h;
			si.Screenpos = { _scrollerRect.x, _scrollerRect.y + _scrollerRect.h + si.Height };
			_screenItems.push_back(si);
			_lastItemHeight = h;
			_itemsIndex++;
		}
	}

	for (auto item = _screenItems.begin(); item != _screenItems.end(); item++)
	{
		item->Screenpos.y--;
		if (item->Screenpos.y < _scrollerRect.y - item->Height)
		{
			SDL_DestroyTexture(item->Texture);
			item = _screenItems.erase(item);
		}
		if (_screenItems.size() < 1) break;
	}

	if (_screenItems.size() < 1) _itemsIndex = 0;

	_msgScroller.OnLoop();
}

void Highscores::OnRender(void)
{
	SDL_RenderSetClipRect(_rend, nullptr);
	SDL_RenderCopy(_rend, _background, nullptr, nullptr);

	
	SDL_RenderSetClipRect(_rend, &_scrollerRect);
	for (auto item = _screenItems.begin(); item != _screenItems.end(); item++)
	{
		SDL_Rect sRect = { 0,0, item->Width, item->Height };
		SDL_Rect dRect = { item->Screenpos.x, item->Screenpos.y, item->Width, item->Height };
		SDL_RenderCopy(_rend, item->Texture, &sRect, &dRect);
	}

	_msgScroller.OnRender();
}

void Highscores::OnCleanup(void)
{
	SaveHighscores();
	SDL_DestroyTexture(_background);

	for (auto item = _screenItems.begin(); item != _screenItems.end(); item++)
	{
		SDL_DestroyTexture(item->Texture);
	}		

	_msgScroller.OnCleanUp();
}

void Highscores::LoadHighscores(void)
{
	std::fstream fs;
	string line = "";
	fs.open("Resources/highscores.txt");
	while (getline(fs, line))
	{
		list<string> items = SplitString(line, ':');
		if (items.size() < 4) continue;
		Highscore hs;
		hs.Name = *next(items.begin(), 1);
		hs.Score = stoi(*next(items.begin(), 2));
		hs.Jumps = stoi(*next(items.begin(), 3));
		HighScores.push_back(hs);
	}
	fs.close();
}

void Highscores::SaveHighscores(void)
{
	std::fstream fs;
	fs.open("Resources/highscores.txt");

	for (auto hs = HighScores.begin(); hs != HighScores.end(); hs++)
	{
		fs << "HS:" << hs->Name << ":" << hs->Score << ":" << hs->Jumps << endl;
	}

	fs.close();
}

void Highscores::Restart(void)
{
	HighScores.sort(ScoreComparator());
	_screenItems.clear();
	_itemsIndex = 0;
	_loopCnt = 0;
	_lastItemHeight = 0;
}

