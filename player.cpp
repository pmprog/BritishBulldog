
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
	SkinLum = (rand() % 2 ? 0.8 : 0.2);
	TeamHue = rand() % 360;
	TeamLum = 0.5;

	UserInput = INPUT_NONE;
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

void Player::ProcessInput( ALLEGRO_EVENT *e )
{

	switch( e->type )
	{
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			UserInput |= INPUT_BUTTON;
			break;
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
			UserInput &= ~INPUT_BUTTON;
			break;
		case ALLEGRO_EVENT_JOYSTICK_AXIS:
			if( (e->joystick.axis % 2) == 0 )
			{
				UserInput &= ~(INPUT_LEFT | INPUT_RIGHT);
				if( e->joystick.pos < 0 )
				{
					UserInput |= INPUT_LEFT;
				} else if( e->joystick.pos > 0 ) {
					UserInput |= INPUT_RIGHT;
				}
			} else {
				UserInput &= ~(INPUT_UP | INPUT_DOWN);
				if( e->joystick.pos < 0 )
				{
					UserInput |= INPUT_UP;
				} else if( e->joystick.pos > 0 ) {
					UserInput |= INPUT_DOWN;
				}
			}
			break;
	}


}

void Player::Update()
{
	UserInputPrevious = UserInput;
}

bool Player::DidInputChange( int InputFlag, bool Pressed )
{
	int c = (UserInput & InputFlag);
	int p = (UserInputPrevious & InputFlag);

	return ( Pressed && c != 0 && p == 0 ) || ( !Pressed && c == 0 && p != 0 );
}
