
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

	MinPlayerCount = 6;

	buttonState = 0;
	buttonStateDelay = 0;

	gameCountDown = START_GAME_COUNTDOWN;
	gameCountDownDelay = 0;
	gameCountDownActive = false;

	titleFont = al_load_font( "resource/forte.ttf", CurrentConfiguration->ScreenHeight / 8, 0 );
	menuFont = al_load_font( "resource/forte.ttf", CurrentConfiguration->ScreenHeight / 48, 0 );
	titleBkg = al_load_bitmap( "resource/title.png" );
	buttonUp = al_load_bitmap( "resource/button_up.png" );
	buttonDown = al_load_bitmap( "resource/button_down.png" );

	if( PlayerList->count > 0 )
	{
		for( int idx = 0; idx < PlayerList->count; idx++ )
		{
			Player* p = (Player*)PlayerList->ItemAt(idx);
			p->State = STATE_AWAIT_ENTRY;
		}
	}

	if( voice != 0 )
	{
		menuMusic = al_load_audio_stream( "resource/wandschrank-gurkenbalken.ogg", 4, 2048 );
		al_set_audio_stream_playmode( menuMusic, ALLEGRO_PLAYMODE_LOOP );
		al_attach_audio_stream_to_mixer( menuMusic, mixer );
		al_set_audio_stream_playing( menuMusic, true );
	}
}

void Menu::Pause()
{
	if( voice != 0 )
	{
		al_set_audio_stream_playing( menuMusic, false );
	}
}

void Menu::Resume()
{
	if( voice != 0 )
	{
		al_set_audio_stream_playing( menuMusic, true );
	}
}

void Menu::Finish()
{
	
	al_destroy_font( menuFont );
	al_destroy_font( titleFont );
	al_destroy_bitmap( buttonDown );
	al_destroy_bitmap( buttonUp );
	al_destroy_bitmap( titleBkg );

	if( voice != 0 )
	{
		al_set_audio_stream_playing( menuMusic, false );
		al_destroy_audio_stream( menuMusic );
	}
}

void Menu::Event(ALLEGRO_EVENT *e)
{
	Player* p;
	bool found = false;

	switch( e->type )
	{
		case ALLEGRO_EVENT_KEY_DOWN:
			if( e->keyboard.keycode == ALLEGRO_KEY_LEFT && MinPlayerCount > 0 )
				MinPlayerCount--;
			if( e->keyboard.keycode == ALLEGRO_KEY_RIGHT && MinPlayerCount < 32 )
				MinPlayerCount++;
			break;

		case ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN:
			titleTint = 255;
			playerTint = 128;
			titleTextY = 20;
			for( int i = 0; i < PlayerList->count; i++ )
			{
				p = (Player*)PlayerList->ItemAt(i);
				if( p->Joystick == e->joystick.id )
				{
					if( p->State < STATE_READY )
						p->State++;
					p->ProcessInput( e );
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
		case ALLEGRO_EVENT_JOYSTICK_BUTTON_UP:
		case ALLEGRO_EVENT_JOYSTICK_AXIS:
			for( int i = 0; i < PlayerList->count; i++ )
			{
				p = (Player*)PlayerList->ItemAt(i);
				if( p->Joystick == e->joystick.id )
				{
					p->ProcessInput( e );
					break;
				}
			}
			break;
		case ALLEGRO_EVENT_AUDIO_STREAM_FINISHED:
			/*
			al_set_audio_stream_playing( menuMusic, false );
			al_rewind_audio_stream( menuMusic );
			al_set_audio_stream_playing( menuMusic, true );
			*/
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
			Player* p = (Player*)PlayerList->ItemAt(idx);
			if( p->State == STATE_READY )
			{
				ready++;
			} else {

				switch( p->State )
				{
					case STATE_SELECT_GENDER:
						if( p->DidInputChange(INPUT_LEFT, true) || p->DidInputChange(INPUT_RIGHT, true) )
							p->Gender = 1 - p->Gender;
						break;
					case STATE_SELECT_HAIR:
						if( (p->UserInput & INPUT_LEFT) != 0 )
						{
							p->HairHue += 1.0;
							if( p->HairHue >= 360.0 )
								p->HairHue -= 360.0;
						}
						if( (p->UserInput & INPUT_RIGHT) != 0 )
						{
							p->HairHue -= 1.0;
							if( p->HairHue < 0.0 )
								p->HairHue += 360.0;
						}
						if( (p->UserInput & INPUT_UP) != 0 && p->HairLum < 1.0 )
						{
							p->HairLum += 0.01;
							if( p->HairLum > 1.0 )
								p->HairLum = 1.0;
						}
						if( (p->UserInput & INPUT_DOWN) != 0 && p->HairLum > 0.0 )
						{
							p->HairLum -= 0.01;
							if( p->HairLum < 0.0 )
								p->HairLum = 0.0;
						}
						break;
					case STATE_SELECT_SKIN:
						if( p->DidInputChange(INPUT_LEFT, true) || p->DidInputChange(INPUT_RIGHT, true) )
							p->SkinLum = 1.0 - p->SkinLum;
						break;
					case STATE_SELECT_TEAM:
						if( (p->UserInput & INPUT_LEFT) != 0 )
						{
							p->TeamHue += 2.0;
							if( p->TeamHue >= 360.0 )
								p->TeamHue -= 360.0;
						}
						if( (p->UserInput & INPUT_RIGHT) != 0 )
						{
							p->TeamHue -= 2.0;
							if( p->TeamHue < 0.0 )
								p->TeamHue += 360.0;
						}
						if( (p->UserInput & INPUT_UP) != 0 && p->TeamLum < 1.0 )
						{
							p->TeamLum += 0.01;
							if( p->TeamLum > 1.0 )
								p->TeamLum = 1.0;
						}
						if( (p->UserInput & INPUT_DOWN) != 0 && p->TeamLum > 0.0 )
						{
							p->TeamLum -= 0.01;
							if( p->TeamLum < 0.0 )
								p->TeamLum = 0.0;
						}

						break;
				}
			}
			p->Update();
		}

		if( PlayerList->count == ready )
		{
			gameCountDownActive = true;
			gameCountDownDelay = (++gameCountDownDelay) % SCREEN_FPS;
			if( gameCountDownDelay == 0 )
				gameCountDown--;
			if( gameCountDown == 0 )
			{
				gameCountDownActive = false;
				gameCountDown = START_GAME_COUNTDOWN;
				Game* g = new Game();
				g->fillAIToCount = MinPlayerCount;
				GameStack->Push( (Stage*)g );
			}
		} else {
			// Wait for players to select character data
			gameCountDownActive = false;
			gameCountDown = START_GAME_COUNTDOWN;
		}
	}
}

void Menu::Render()
{
	int plyBoxW = ((CurrentConfiguration->ScreenWidth - 40) / 4) - 10;
	int plyBoxH = CurrentConfiguration->ScreenHeight / 4;

	//al_clear_to_color( al_map_rgb( 0, 0, 0 ) );

	al_draw_tinted_scaled_bitmap( titleBkg, al_map_rgb( titleTint, titleTint, titleTint ), 0, 0, al_get_bitmap_width(titleBkg), al_get_bitmap_height(titleBkg), 0, 0, CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight, 0 );

	al_draw_text( titleFont, al_map_rgb(0, 0, 0), 12, titleTextY + 2, ALLEGRO_ALIGN_LEFT, "British Bulldogs" );
	al_draw_text( titleFont, al_map_rgb(255, 255, 255), 10, titleTextY, ALLEGRO_ALIGN_LEFT, "British Bulldogs" );

	for( int plyIdx = 0; plyIdx < 4; plyIdx++ )
	{
		RenderPlayerBox( (plyIdx * 2), 20 + (plyBoxW * plyIdx) + 5, (CurrentConfiguration->ScreenHeight / 3), plyBoxW - 10, plyBoxH );
		RenderPlayerBox( (plyIdx * 2) + 1, 20 + (plyBoxW * plyIdx) + 5, (CurrentConfiguration->ScreenHeight / 3) + plyBoxH + 10, plyBoxW - 10, plyBoxH );
	}


	char numJoy[200];
	sprintf( numJoy, "AI Fill To %d Players", MinPlayerCount );
	al_draw_text( menuFont, al_map_rgb( 128, 128, 255 ), CurrentConfiguration->ScreenWidth, 48, ALLEGRO_ALIGN_RIGHT, numJoy );


	sprintf( numJoy, "%d Joystick(s) Found", al_get_num_joysticks() );
	al_draw_text( menuFont, al_map_rgb( 0, 0, 0 ), CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight - 48, ALLEGRO_ALIGN_RIGHT, numJoy );


	if( gameCountDownActive )
	{
		sprintf( numJoy, "Starts In %d", gameCountDown );
		al_draw_text( titleFont, al_map_rgb( 0, 0, 0 ), (CurrentConfiguration->ScreenWidth / 2) + 2, (CurrentConfiguration->ScreenHeight / 2) + 2, ALLEGRO_ALIGN_CENTRE, numJoy );
		al_draw_text( titleFont, al_map_rgb( 255, 255, 128 ), CurrentConfiguration->ScreenWidth / 2, CurrentConfiguration->ScreenHeight / 2, ALLEGRO_ALIGN_CENTRE, numJoy );
	}

#ifdef _DEBUG
	for( int idx = 0; idx < al_get_num_joysticks(); idx++ )
	{
		ALLEGRO_JOYSTICK* j = al_get_joystick( idx );
		ALLEGRO_JOYSTICK_STATE je;
		al_get_joystick_state( j, &je );
		sprintf( numJoy, "%d: %f : %d: %f", 0, je.stick[0].axis[0], 1, je.stick[0].axis[1] );
		al_draw_text( menuFont, al_map_rgb( 0, 255, 255 ), 400, idx * 40, ALLEGRO_ALIGN_LEFT, numJoy );
	}
#endif

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