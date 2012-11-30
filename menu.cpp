
#include "menu.h"
#include "configure.h"

void Menu::Begin()
{
	titleTint = 0;
	titleTintDelay = 0;
	titleTextY = -200;
	playerTint = 0;

	buttonState = 0;
	buttonStateDelay = 0;

	titleFont = al_load_font( "resource/forte.ttf", 128, 0 );
	menuFont = al_load_font( "resource/forte.ttf", 32, 0 );
	titleBkg = al_load_bitmap( "resource/title.png" );
	buttonUp = al_load_bitmap( "resource/button_up.png" );
	buttonDown = al_load_bitmap( "resource/button_down.png" );
}

void Menu::Pause()
{
}

void Menu::Resume()
{
}

void Menu::Finish()
{
	al_destroy_font( menuFont );
	al_destroy_font( titleFont );
	al_destroy_bitmap( buttonDown );
	al_destroy_bitmap( buttonUp );
	al_destroy_bitmap( titleBkg );
}

void Menu::Event(ALLEGRO_EVENT *e)
{
	switch( e->type )
	{
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			break;
		case ALLEGRO_EVENT_JOYSTICK_AXIS:
			break;
	}
}

void Menu::Update()
{
	if( titleTint < 255 )
	{
		titleTint += 2;
		if( titleTint > 255 )
			titleTint = 255;
	} else if( playerTint < 128 ) {
		playerTint += 4;
	}

	if( titleTextY < 20 )
		titleTextY += 3;

	buttonStateDelay = (++buttonStateDelay) % 6;
	if( buttonStateDelay == 0 )
		buttonState = 1 - buttonState;
}

void Menu::Render()
{
	int plyBoxW = ((CurrentConfiguration->ScreenWidth - 40) / 4) - 10;
	int plyBoxH = CurrentConfiguration->ScreenHeight / 4;

	al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	al_draw_tinted_scaled_bitmap( titleBkg, al_map_rgb( titleTint, titleTint, titleTint ), 0, 0, al_get_bitmap_width(titleBkg), al_get_bitmap_height(titleBkg), 0, 0, CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight, 0 );

	al_draw_text( titleFont, al_map_rgb(0, 0, 0), 12, titleTextY + 2, ALLEGRO_ALIGN_LEFT, "British Bulldogs" );
	al_draw_text( titleFont, al_map_rgb(255, 255, 255), 10, titleTextY, ALLEGRO_ALIGN_LEFT, "British Bulldogs" );

	for( int plyIdx = 0; plyIdx < 4; plyIdx++ )
	{
		RenderPlayerBox( (plyIdx * 2), 20 + (plyBoxW * plyIdx) + 5, (CurrentConfiguration->ScreenHeight / 3), plyBoxW - 10, plyBoxH );
		RenderPlayerBox( (plyIdx * 2) + 1, 20 + (plyBoxW * plyIdx) + 5, (CurrentConfiguration->ScreenHeight / 3) + plyBoxH + 10, plyBoxW - 10, plyBoxH );
	}

	

	char numJoy[200];
	sprintf( numJoy, "%d Joystick(s) Found", al_get_num_joysticks() );
	al_draw_text( menuFont, al_map_rgb( 0, 0, 0 ), CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight - 48, ALLEGRO_ALIGN_RIGHT, numJoy );
}

void Menu::RenderPlayerBox( int PlayerIdx, int BoxX, int BoxY, int BoxW, int BoxH )
{
	al_draw_filled_rectangle( BoxX, BoxY, BoxX + BoxW, BoxY + BoxH, al_map_rgba( 0, 0, 0, playerTint ) );
	if( playerTint < 128 )
		return;

	if( PlayerList->count < PlayerIdx + 1 )
	{
		al_draw_bitmap( (buttonState == 0 ? buttonUp : buttonDown), BoxX + (BoxW / 2) - (al_get_bitmap_width(buttonDown) / 2), BoxY + (BoxH / 2) - (al_get_bitmap_height(buttonDown) / 2), 0 );
	} else {
	}

}