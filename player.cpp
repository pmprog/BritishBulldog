
#include "player.h"

Player::Player()
{
	Position = new Vector(0, 0);

	State = STATE_SELECT_GENDER;
	Frame = 0;
	FacingLeft = false;
	IsBulldog = false;

	Gender = rand() % 2;
	HairHue = rand() % 360;
	HairLum = (rand() % 2 ? 0.9 : 0.2);
	SkinHue = 10.0;
	SkinLum = (rand() % 2 ? 0.9 : 0.2);
	TeamHue = rand() % 360;
	TeamLum = 0.5;
}

int Player::GetFrameNumber()
{
	if( State == STATE_WALKING || State == STATE_RUNNING )
	{
		return 1 + (Frame % 4);
	} else {
		return 0;
	}
}

void Player::Render( int DrawX, int DrawY, int DrawW, int DrawH )
{
	ALLEGRO_BITMAP* sub;

	sub = al_create_sub_bitmap( SkinBase, GetFrameNumber() * 48, Gender * 48, 48, 48 );
	al_draw_scaled_bitmap( sub, 0, 0, 48, 48, DrawX - (DrawW/2), DrawY - DrawH, DrawW, DrawH, (FacingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0) );
	al_destroy_bitmap( sub );

	sub = al_create_sub_bitmap( SkinHair, GetFrameNumber() * 48, Gender * 48, 48, 48 );
	al_draw_tinted_scaled_bitmap( sub, al_color_hsl( HairHue, 0.7, HairLum ), 0, 0, 48, 48, DrawX - (DrawW/2), DrawY - DrawH, DrawW, DrawH, (FacingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0) );
	al_destroy_bitmap( sub );

	sub = al_create_sub_bitmap( SkinSkin, GetFrameNumber() * 48, Gender * 48, 48, 48 );
	al_draw_tinted_scaled_bitmap( sub, al_color_hsl( SkinHue, 0.7, SkinLum ), 0, 0, 48, 48, DrawX - (DrawW/2), DrawY - DrawH, DrawW, DrawH, (FacingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0) );
	al_destroy_bitmap( sub );

	sub = al_create_sub_bitmap( SkinTeam, GetFrameNumber() * 48, Gender * 48, 48, 48 );
	al_draw_tinted_scaled_bitmap( sub, al_color_hsl( TeamHue, 0.8, TeamLum ), 0, 0, 48, 48, DrawX - (DrawW/2), DrawY - DrawH, DrawW, DrawH, (FacingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0) );
	al_destroy_bitmap( sub );
}

void Player::MenuProcessInput( float Direction )
{
	switch( State )
	{
		case STATE_SELECT_GENDER:
			Gender = 1 - Gender;
			break;
		case STATE_SELECT_HAIR:
			HairHue += Direction;
			if( HairHue >= 360.0 )
				HairHue -= 360.0;
			if( HairHue < 0.0 )
				HairHue += 360.0;
			break;
		case STATE_SELECT_SKIN:
			SkinLum = 1.1 - SkinLum;
			break;
		case STATE_SELECT_TEAM:
			TeamHue += Direction;
			if( TeamHue >= 360.0 )
				TeamHue -= 360.0;
			if( TeamHue < 0.0 )
				TeamHue += 360.0;
			break;
	}

}
