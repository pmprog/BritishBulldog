
#pragma once

#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include "stagestack.h"
#ifndef Configuration
class Configuration;
#endif

#ifdef _core

ALLEGRO_DISPLAY_MODE	ScreenMode;
ALLEGRO_DISPLAY				*Screen;
ALLEGRO_EVENT_QUEUE		*EventQueue;
bool									Quit = false;
StageStack*						GameStack;
Configuration*				CurrentConfiguration;

#else

extern bool						Quit;
extern StageStack*		GameStack;
extern Configuration*	CurrentConfiguration;

#endif

