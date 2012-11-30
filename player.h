
#pragma once

#include "main.h"

class Player
{
	public:
		Player();

		ALLEGRO_JOYSTICK* Joystick;
		ALLEGRO_BITMAP* Skin;
		int Score;
		Vector* Position;
		int Skin;

		void SetHairColour(int Hue, int Lum);
		void SetSkinColour(int Hue, int Lum);
		void SetShirtColour(int Hue, int Lum);
};