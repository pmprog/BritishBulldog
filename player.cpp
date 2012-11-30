
#include "player.h"

Player::Player()
{
	Position = new Vector(0, 0);

	State = 0;
	Frame = 0;
	FacingLeft = false;

	Gender = 0;
	HairHue = 0;
	HairLum = 0.2;
	SkinHue = 10.0;
	SkinLum = 0.9;
	TeamHue = 275.0;
	TeamLum = 0.5;
}

void Player::Render( int DrawX, int DrawY, int DrawW, int DrawH )
{
	ALLEGRO_BITMAP* sub;

	sub = al_create_sub_bitmap( SkinBase, (State + Frame) * 48, Gender * 48, 48, 48 );
	al_draw_scaled_bitmap( sub, 0, 0, 48, 48, DrawX - (DrawW/2), DrawY - DrawH, DrawW, DrawH, (FacingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0) );
	al_destroy_bitmap( sub );

	sub = al_create_sub_bitmap( SkinHair, (State + Frame) * 48, Gender * 48, 48, 48 );
	al_draw_tinted_scaled_bitmap( sub, al_color_hsl( HairHue, 0.7, HairLum ), 0, 0, 48, 48, DrawX - (DrawW/2), DrawY - DrawH, DrawW, DrawH, (FacingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0) );
	al_destroy_bitmap( sub );

	sub = al_create_sub_bitmap( SkinSkin, (State + Frame) * 48, Gender * 48, 48, 48 );
	al_draw_tinted_scaled_bitmap( sub, al_color_hsl( SkinHue, 0.7, SkinLum ), 0, 0, 48, 48, DrawX - (DrawW/2), DrawY - DrawH, DrawW, DrawH, (FacingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0) );
	al_destroy_bitmap( sub );

	sub = al_create_sub_bitmap( SkinTeam, (State + Frame) * 48, Gender * 48, 48, 48 );
	al_draw_tinted_scaled_bitmap( sub, al_color_hsl( TeamHue, 0.8, TeamLum ), 0, 0, 48, 48, DrawX - (DrawW/2), DrawY - DrawH, DrawW, DrawH, (FacingLeft ? ALLEGRO_FLIP_HORIZONTAL : 0) );
	al_destroy_bitmap( sub );
}