
#include "menu.h"
#include "configure.h"

void Menu::Begin()
{
	titleTint = 0;
	titleTintDelay = 0;
	titleTextY = -100;
	playerTint = 0;

	titleFont = al_load_font( "resource/forte.ttf", 128, 0 );
	menuFont = al_load_font( "resource/forte.ttf", 32, 0 );
	titleBkg = al_load_bitmap( "resource/title.bmp" );
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
	al_destroy_bitmap( titleBkg );
}

void Menu::Event(ALLEGRO_EVENT *e)
{
}

void Menu::Update()
{
	titleTintDelay = (++titleTintDelay) % 128;
	if( titleTintDelay == 0 )
	{
		if( titleTint < 255 )
			titleTint++;
		else if( playerTint < 128 )
			playerTint++;

		if( titleTextY < 20 )
			titleTextY++;
	}
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
}