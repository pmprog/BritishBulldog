
#pragma once

#include "main.h"

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

		int Gender;
		float HairHue;
		float HairLum;
		float SkinHue;
		float SkinLum;
		float TeamHue;
		float TeamLum;

		void Player::Render( int DrawX, int DrawY, int DrawW, int DrawH );
};