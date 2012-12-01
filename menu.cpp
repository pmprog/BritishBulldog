
#include "menu.h"
#include "configure.h"
#include "game.h"
#include "player.h"

void Menu::Begin()
{
	titleTint = 0;
	titleTintDelay = 0;
	titleTextY = -200;
	playerTint = 0;

	buttonState = 0;
	buttonStateDelay = 0;

	gameCountDown = 10;
	gameCountDownDelay = 0;
	gameCountDownActive = false;

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
	Player* p;
	bool found = false;

	switch( e->type )
	{
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			for( int i = 0; i < PlayerList->count; i++ )
			{
				p = (Player*)PlayerList->ItemAt(i);
				if( p->Joystick == e->joystick.id )
				{
					if( p->State < STATE_READY )
						p->State++;
					found = true;
					break;
				}
			}

			if( !found )
			{
				p = new Player();
				p->Joystick = e->joystick.id;
				PlayerList->AddToEnd( p );
			}
			break;
		case ALLEGRO_EVENT_JOYSTICK_AXIS:
			for( int i = 0; i < PlayerList->count; i++ )
			{
				p = (Player*)PlayerList->ItemAt(i);
				if( p->Joystick == e->joystick.id )
				{
					p->MenuProcessInput( e->joystick.pos );

					break;
				}
			}
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


	if( PlayerList->count > 0 )
	{
		int ready = 0;
		for( int idx = 0; idx < PlayerList->count; idx++ )
		{
			if( ((Player*)PlayerList->ItemAt(idx))->State == STATE_READY )
				ready++;
		}

		if( PlayerList->count == ready )
		{
			gameCountDownActive = true;
			gameCountDownDelay = (++gameCountDownDelay) % SCREEN_FPS;
			if( gameCountDownDelay == 0 )
				gameCountDown--;
			if( gameCountDown == 0 )
			{
				GameStack->Push( (Stage*)new Game() );
			}
		} else {
			// Wait for players to select character data
			gameCountDownActive = false;
			gameCountDown = 10;
		}
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


	if( gameCountDownActive )
	{
		sprintf( numJoy, "Game Starts In %d Seconds", gameCountDown );
		al_draw_text( titleFont, al_map_rgb( 0, 0, 0 ), (CurrentConfiguration->ScreenWidth / 2) + 2, (CurrentConfiguration->ScreenHeight / 2) + 2, ALLEGRO_ALIGN_CENTRE, numJoy );
		al_draw_text( titleFont, al_map_rgb( 255, 255, 128 ), CurrentConfiguration->ScreenWidth / 2, CurrentConfiguration->ScreenHeight / 2, ALLEGRO_ALIGN_CENTRE, numJoy );
	}
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
		Player* p = (Player*)PlayerList->ItemAt( PlayerIdx );
		if( p->State == STATE_AWAIT_ENTRY )
		{
			al_draw_bitmap( (buttonState == 0 ? buttonUp : buttonDown), BoxX + (BoxW / 2) - (al_get_bitmap_width(buttonDown) / 2), BoxY + (BoxH / 2) - (al_get_bitmap_height(buttonDown) / 2), 0 );
		} else {
			int ScaleSize = ((BoxW <= BoxH ? BoxW : BoxH) / 5) * 4;
			p->Render( BoxX + (BoxW / 2), BoxY + (BoxH / 2) + (ScaleSize / 2), ScaleSize, ScaleSize );

			switch( p->State )
			{
				case STATE_SELECT_GENDER:
					al_draw_text( menuFont, al_map_rgb( 255, 255, 255 ), BoxX + (BoxW / 2), BoxY + BoxH - al_get_font_line_height( menuFont ), ALLEGRO_ALIGN_CENTRE, "< Gender >" );
					break;
				case STATE_SELECT_SKIN:
					al_draw_text( menuFont, al_map_rgb( 255, 255, 255 ), BoxX + (BoxW / 2), BoxY + BoxH - al_get_font_line_height( menuFont ), ALLEGRO_ALIGN_CENTRE, "< Skin >" );
					break;
				case STATE_SELECT_HAIR:
					al_draw_text( menuFont, al_map_rgb( 255, 255, 255 ), BoxX + (BoxW / 2), BoxY + BoxH - al_get_font_line_height( menuFont ), ALLEGRO_ALIGN_CENTRE, "< Hair >" );
					break;
				case STATE_SELECT_TEAM:
					al_draw_text( menuFont, al_map_rgb( 255, 255, 255 ), BoxX + (BoxW / 2), BoxY + BoxH - al_get_font_line_height( menuFont ), ALLEGRO_ALIGN_CENTRE, "< Colour >" );
					break;
				case STATE_READY:
					al_draw_text( menuFont, al_map_rgb( 255, 255, 255 ), BoxX + (BoxW / 2), BoxY + BoxH - al_get_font_line_height( menuFont ), ALLEGRO_ALIGN_CENTRE, "Ready" );
					break;
			}

		}
	}

}