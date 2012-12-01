
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

#define INPUT_NONE						0
#define INPUT_LEFT						1
#define INPUT_UP							2
#define INPUT_RIGHT						4
#define INPUT_DOWN						8
#define INPUT_BUTTON					16

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

		int UserInputPrevious;
		int UserInput;

		int Gender;
		float HairHue;
		float HairLum;
		float SkinHue;
		float SkinLum;
		float TeamHue;
		float TeamLum;

		int GetFrameNumber();
		void ProcessInput( ALLEGRO_EVENT *e );
		void Render( int DrawX, int DrawY, int DrawW, int DrawH );
		void Update();

		bool DidInputChange( int InputFlag, bool Pressed );
};