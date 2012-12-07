
#include "player.h"
#include "configure.h"

Player::Player()
{
	Position = new Vector(0, 0);

	Energy = ENERGY_MAX;

	State = STATE_SELECT_GENDER;
	Frame = 0;
	FacingLeft = false;
	WasTagged = false;
	IsBulldog = false;
	IsAI = false;

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
	switch( State )
	{
		case STATE_WALKING:
			return 1 + (Frame % 4);
			break;
		case STATE_KNACKERED:
			return 5 + (Frame % 2);
		default:
			return 0;
	}
}

void Player::Render( int DrawW, int DrawH )
{
	Render( Position->x, Position->y, DrawW, DrawH );
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
	Update( true );
}

void Player::Update( bool UseEnergy )
{
	UserInputPrevious = UserInput;

	bool turbo = UserInput & INPUT_BUTTON;
	float distanceMod = (turbo ? ((float)CurrentConfiguration->ScreenWidth / ((float)SCREEN_FPS * SPEED_RUN_TIME)) : ((float)CurrentConfiguration->ScreenWidth / ((float)SCREEN_FPS * SPEED_WALK_TIME)));
	int dir = UserInput & ~INPUT_BUTTON;
	float xMod = 0.0;
	float yMod = 0.0;
	switch( dir )
	{
		case INPUT_LEFT:
			xMod = -2.0;
			break;
		case INPUT_RIGHT:
			xMod = 2.0;
			break;
		case INPUT_UP:
			yMod = -2.0;
			break;
		case INPUT_DOWN:
			yMod = 2.0;
			break;
		case INPUT_LEFT | INPUT_UP:
			xMod = -1.414;
			yMod = -1.414;
			break;
		case INPUT_RIGHT | INPUT_UP:
			xMod = 1.414;
			yMod = -1.414;
			break;
		case INPUT_LEFT | INPUT_DOWN:
			xMod = -1.414;
			yMod = 1.414;
			break;
		case INPUT_RIGHT | INPUT_DOWN:
			xMod = 1.414;
			yMod = 1.414;
			break;
	}
	xMod *= distanceMod;
	if( xMod < 0.0 )
		FacingLeft = true;
	if( xMod > 0.0 )
		FacingLeft = false;
	yMod *= distanceMod;
	switch( State )
	{
		case STATE_KNACKERED:
			//if( UseEnergy )
			Energy += ENERGY_RECOVER;
			if( Energy >= ENERGY_MAX / 3.0 )
			{
				if( xMod != 0.0 || yMod != 0.0 )
					State = STATE_WALKING;
				else
					State = STATE_STANDING;
			}
			break;

		case STATE_STANDING:
			if( xMod != 0.0 || yMod != 0.0 )
				State = STATE_WALKING;
			else if( UseEnergy )
				Energy += ENERGY_RECOVER;
			break;

		case STATE_WALKING:
			Frame = ++Frame % 4;
			if( xMod != 0.0 || yMod != 0.0 )
			{
				if( UseEnergy )
					Energy -= ( turbo ? ENERGY_RUNUSAGE : ENERGY_WALKUSAGE );
				Position->x += xMod;
				Position->y += yMod;
			} else {
				State = STATE_STANDING;
				Frame = 0;
			}
			break;
	}
	if( Energy <= 0.0 )
	{
		Energy = 0.0;
		State = STATE_KNACKERED;
		Frame = 0;
	} else if ( Energy > ENERGY_MAX ) {
		Energy = ENERGY_MAX;
	}
}

bool Player::DidInputChange( int InputFlag, bool Pressed )
{
	int c = (UserInput & InputFlag);
	int p = (UserInputPrevious & InputFlag);

	return ( Pressed && c != 0 && p == 0 ) || ( !Pressed && c == 0 && p != 0 );
}
