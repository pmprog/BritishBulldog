
#pragma once

#include "main.h"

#define GAMESTATE_REQUEST_BULLDOG			0
#define GAMESTATE_READY								1
#define GAMESTATE_INGAME							2
#define GAMESTATE_RESULTS							3

class Game : Stage
{
	private:
		ALLEGRO_BITMAP* bkgSky;
		ALLEGRO_BITMAP* bkgGrnd;
		ALLEGRO_BITMAP* noticeBox;
		ALLEGRO_BITMAP* buttonUp;
		ALLEGRO_BITMAP* buttonDown;

		int State;
		int GameCountdown;

		ALLEGRO_FONT* readyFont;
		int readyFontSize;

		int gameAreaUpperY;
		int gameAreaLowerY;
		int gameAreaLeft;
		int gameAreaRight;

  public:
		List* AIList;

    // Stage control
    void Begin();
    void Pause();
    void Resume();
    void Finish();
    void Event(ALLEGRO_EVENT *e);
    void Update();
    void Render();
};
