
#include "player.h"

Player::Player()
{
	Skin = al_load_bitmap( "resource/skins.png" );
	Position = new Vector(0, 0);
	Skin = 0;
}

void Player::SetHairColour(int Hue, int Lum)
{
}

void Player::SetSkinColour(int Hue, int Lum)
{
}

void Player::SetShirtColour(int Hue, int Lum)
{
}

