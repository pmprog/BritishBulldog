
#pragma once

#include "main.h"

class Menu : Stage
{
	private:
		ALLEGRO_BITMAP* titleBkg;
		ALLEGRO_FONT* titleFont;
		ALLEGRO_FONT* menuFont;
		int titleTint;
		int titleTintDelay;
		int titleTextY;
		int playerTint;

		void RenderPlayerBox( int PlayerIdx, int BoxX, int BoxY, int BoxW, int BoxH );

  public:
    // Stage control
    void Begin();
    void Pause();
    void Resume();
    void Finish();
    void Event(ALLEGRO_EVENT *e);
    void Update();
    void Render();

};
