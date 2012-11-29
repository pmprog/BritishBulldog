
#pragma once

#define _config
#include "../main.h"

class Configuration : Stage
{
	public:
		Configuration();
		~Configuration();

		// Settings
		void InitSettings();
		void LoadSettings();
		void SaveSettings();
		int ScreenWidth;
		int ScreenHeight;
		bool FullScreen;
		bool WasLoaded;


		// Config Screen
    void Begin();
    void Pause();
    void Resume();
    void Finish();
    void Event(ALLEGRO_EVENT *e);
    void Update();
    void Render();

		ALLEGRO_FONT* configFont;

};