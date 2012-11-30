
#pragma once

#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_color.h>
#include <allegro5/allegro_image.h>
#include "stagestack.h"
#include "maths/vector2.h"
#include "maths/list.h"
#include "maths/line.h"
#ifndef Configuration
class Configuration;
#endif


#define SCREEN_FPS		50


#ifdef _core

ALLEGRO_DISPLAY_MODE	ScreenMode;
ALLEGRO_DISPLAY				*Screen;
ALLEGRO_EVENT_QUEUE		*EventQueue;
bool									Quit = false;
StageStack*						GameStack;
Configuration*				CurrentConfiguration;
List*									PlayerList;

#else

extern bool						Quit;
extern StageStack*		GameStack;
extern Configuration*	CurrentConfiguration;
extern List*					PlayerList;

#endif

