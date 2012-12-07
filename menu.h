
#pragma once

#include "main.h"

class Menu : Stage
{
	private:
		ALLEGRO_BITMAP* titleBkg;
		ALLEGRO_BITMAP* buttonUp;
		ALLEGRO_BITMAP* buttonDown;
		ALLEGRO_FONT* titleFont;
		ALLEGRO_FONT* menuFont;
		ALLEGRO_AUDIO_STREAM* menuMusic;
		//ALLEGRO_EVENT_SOURCE* menuMusicSource;

		int titleTint;
		int titleTintDelay;
		int titleTextY;
		int playerTint;
		int buttonState;
		int buttonStateDelay;
		int gameCountDown;
		int gameCountDownDelay;
		bool gameCountDownActive;

		void RenderPlayerBox( int PlayerIdx, int BoxX, int BoxY, int BoxW, int BoxH );

		int MinPlayerCount;

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
