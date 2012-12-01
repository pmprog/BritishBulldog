
#pragma once

#include "main.h"

struct PlayerInformation
{
	bool active;
};

class Game : Stage
{
	private:
		ALLEGRO_BITMAP* bkgSky;
		ALLEGRO_BITMAP* bkgGrnd;

  public:
		PlayerInformation	Players[MAX_PLAYERS];

    // Stage control
    void Begin();
    void Pause();
    void Resume();
    void Finish();
    void Event(ALLEGRO_EVENT *e);
    void Update();
    void Render();
};
