#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <string>
#include <list>
#include <unordered_map>
#include <boost/filesystem.hpp>
#include "SDL_Extras.h"
#include "TextScroller.h"
using namespace std;
using namespace boost::filesystem;

typedef struct Highscore
{
	string Name;
	Uint32 Score;
	Uint32 Jumps;
};

struct ScoreComparator
{
	bool operator ()(const Highscore& a, const Highscore& b) const
	{
		return a.Score > b.Score;
	}
}; 

struct JumpsComparator
{
	bool operator ()(const Highscore& a, const Highscore& b) const
	{
		return a.Jumps > b.Jumps;
	}
};

typedef struct ScoreScreenItem
{
	Highscore Item;
	SDL_Point Screenpos;
	Uint16 Width, Height;
	SDL_Texture* Texture;
};

class Highscores
{
public:
	Highscores();
	~Highscores();

	SDL_Rect DisplayRect;
	list<Highscore> HighScores;

	void OnInit(SDL_Renderer* rend);
	void OnLoop(void);
	void OnRender(void);
	void OnCleanup(void);
	void LoadHighscores(void);
	void SaveHighscores(void);
	void Restart(void);

protected:
	SDL_Renderer* _rend;
	SDL_Texture* _background;
	SDL_Rect _scrollerRect;
	TTF_Font* _font;
	list<ScoreScreenItem> _screenItems;
	Uint16 _itemsIndex;
	Uint16 _lastItemHeight;
	Uint16 _loopCnt;
	TextScroller _msgScroller;
};

