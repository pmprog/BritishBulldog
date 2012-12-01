
#pragma once

#include "main.h"

#define STATE_AWAIT_ENTRY			-1
#define STATE_SELECT_GENDER		0
#define STATE_SELECT_SKIN			1
#define STATE_SELECT_HAIR			2
#define STATE_SELECT_TEAM			3
#define STATE_READY						4
#define STATE_STANDING				5
#define STATE_WALKING					6
#define STATE_RUNNING					7
#define STATE_KNACKERED				8

class Player
{
	public:
		Player();

		ALLEGRO_JOYSTICK* Joystick;
		int Score;
		Vector* Position;

		int State;
		int Frame;
		bool FacingLeft;
		bool IsBulldog;

		int Gender;
		float HairHue;
		float HairLum;
		float SkinHue;
		float SkinLum;
		float TeamHue;
		float TeamLum;

		int GetFrameNumber();
		void MenuProcessInput( float Direction );
		void Render( int DrawX, int DrawY, int DrawW, int DrawH );
};