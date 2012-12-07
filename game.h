
#pragma once

#include "main.h"
#include "player.h"

#define GAMESTATE_REQUEST_BULLDOG			0
#define GAMESTATE_READY								1
#define GAMESTATE_PREPROUND						2
#define GAMESTATE_INGAME							3
#define GAMESTATE_RESULTS							4

class Game : Stage
{
	private:
		ALLEGRO_BITMAP* bkgSky;
		ALLEGRO_BITMAP* bkgGrnd;
		ALLEGRO_BITMAP* noticeBox;
		ALLEGRO_BITMAP* buttonUp;
		ALLEGRO_BITMAP* buttonDown;
		ALLEGRO_AUDIO_STREAM* gameMusic;

		int State;
		int GameCountdown;
		int buttonPop;

		List* Bulldogs;

		ALLEGRO_FONT* readyFont;
		int readyFontSize;

		int gameAreaUpperY;
		int gameAreaLowerY;
		int gameAreaLeft;
		int gameAreaRight;

		void SortPlayerList();
		void CheckTags( Player* bulldog );

		bool destinationIsRightSide;

		Player* Winner;
		int WinnerCountDown;
		void ProcessAI( Player* player );

  public:
		int fillAIToCount;

    // Stage control
    void Begin();
    void Pause();
    void Resume();
    void Finish();
    void Event(ALLEGRO_EVENT *e);
    void Update();
    void Render();
};
