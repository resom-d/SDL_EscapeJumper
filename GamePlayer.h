#pragma once

struct _playerProps
{
	char* name;
	
	bool isIdle;
	bool isWalking;
	bool isRunning;
	bool isJumping;
	bool isLanding;
	bool movesLeft;
	bool movesRight;
	bool movesUp;
	bool movesDown;

};

typedef struct _playerProps PlayerProps;

class GamePlayer
{

public:
	GamePlayer();
	virtual ~GamePlayer();

	PlayerProps Properties;
	

private:

protected:


};

