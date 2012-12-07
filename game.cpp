
#include "game.h"
#include "configure.h"
#include "player.h"

void Game::Begin()
{
	bkgSky = al_load_bitmap( "resource/game_sky.png" );
	bkgGrnd = al_load_bitmap( "resource/game_ground.png" );
	noticeBox = al_load_bitmap( "resource/notice.png" );
	buttonUp = al_load_bitmap( "resource/button_up.png" );
	buttonDown = al_load_bitmap( "resource/button_down.png" );
	readyFont = 0;

	gameAreaUpperY = CurrentConfiguration->ScreenHeight / 4;
	gameAreaLowerY = (CurrentConfiguration->ScreenHeight / 8) * 7;
	gameAreaLeft = CurrentConfiguration->ScreenWidth / 10;
	gameAreaRight = CurrentConfiguration->ScreenWidth - gameAreaLeft;

	AIList = new List();

	// TODO: Populate AI players

	State = GAMESTATE_REQUEST_BULLDOG;
}

void Game::Pause()
{
}

void Game::Resume()
{
}

void Game::Finish()
{
	al_destroy_bitmap( bkgGrnd );
	al_destroy_bitmap( bkgSky );
	al_destroy_bitmap( buttonDown );
	al_destroy_bitmap( buttonUp );
	al_destroy_bitmap( noticeBox );

	for( int i = 0; i < PlayerList->count; i++ )
	{
		Player* p = (Player*)PlayerList->ItemAt(i);
		p->State = STATE_AWAIT_ENTRY;
	}
}

void Game::Event(ALLEGRO_EVENT *e)
{
	if( e->type == ALLEGRO_EVENT_KEY_DOWN && e->keyboard.keycode == ALLEGRO_KEY_ESCAPE )
		GameStack->Pop();

	switch( State )
	{
		case GAMESTATE_REQUEST_BULLDOG:
			if( e->type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN )
			{
				for( int pI = 0; pI < PlayerList->count; pI++ )
				{
					Player* p = (Player*)PlayerList->ItemAt( pI );
					if( p->Joystick == e->joystick.id )
					{
						p->IsBulldog = true;
						p->Position->y = (rand() % (gameAreaLowerY - gameAreaUpperY)) + gameAreaUpperY;
						p->Position->x = CurrentConfiguration->ScreenWidth / 2;
					} else {
						p->Position->y = (rand() % (gameAreaLowerY - gameAreaUpperY)) + gameAreaUpperY;
						p->Position->x = gameAreaLeft;
						p->DesintationIsRightSide = true;
					}
				}
				State = GAMESTATE_READY;
				GameCountdown = SCREEN_FPS * 5;
			}
			break;
		case GAMESTATE_READY:
			break;
		case GAMESTATE_INGAME:
			for( int pI = 0; pI < PlayerList->count; pI++ )
			{
				Player* p = (Player*)PlayerList->ItemAt( pI );
				if( p->Joystick == e->joystick.id )
				{
					p->ProcessInput( e );
				}
			}
			break;
		case GAMESTATE_RESULTS:
			break;
	}
}

void Game::Update()
{
	switch( State )
	{
		case GAMESTATE_REQUEST_BULLDOG:
			break;
		case GAMESTATE_READY:
			GameCountdown--;
			if( GameCountdown == 0 )
			{
				State = GAMESTATE_INGAME;
				for( int pI = 0; pI < PlayerList->count; pI++ )
				{
					Player* p = (Player*)PlayerList->ItemAt( pI );
					p->State = STATE_STANDING;
				}
			}
			break;
		case GAMESTATE_INGAME:
			for( int pI = 0; pI < PlayerList->count; pI++ )
			{
				Player* p = (Player*)PlayerList->ItemAt( pI );
				p->Update();

				// Keep player in bounds
				if( p->Position->x < 0 )
					p->Position->x = 0;
				if( p->Position->x > CurrentConfiguration->ScreenWidth )
					p->Position->x = CurrentConfiguration->ScreenWidth - 1;
				if( p->IsBulldog )
				{
					if( p->Position->x < gameAreaLeft )
						p->Position->x = gameAreaLeft;
					if( p->Position->x > gameAreaRight )
						p->Position->x = gameAreaRight;
				}
				if( p->Position->y < gameAreaUpperY )
					p->Position->y = gameAreaUpperY;
				if( p->Position->y > gameAreaLowerY )
					p->Position->y = gameAreaLowerY;

			}
			break;
		case GAMESTATE_RESULTS:
			break;
	}
}

void Game::Render()
{
	al_draw_scaled_bitmap( bkgSky, 0, 0, al_get_bitmap_width(bkgSky), al_get_bitmap_height(bkgSky), 0, 0, CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight / 8, 0 );
	al_draw_scaled_bitmap( bkgGrnd, 0, 0, al_get_bitmap_width(bkgGrnd), al_get_bitmap_height(bkgGrnd), 0, CurrentConfiguration->ScreenHeight / 8, CurrentConfiguration->ScreenWidth, (CurrentConfiguration->ScreenHeight / 8) * 7, 0 );

	al_draw_line( 0, gameAreaUpperY, CurrentConfiguration->ScreenWidth, gameAreaUpperY, al_map_rgba(255, 255, 0, 128), 3 );
	al_draw_line( 0, gameAreaLowerY, CurrentConfiguration->ScreenWidth, gameAreaLowerY, al_map_rgba(255, 255, 0, 128), 3 );

	al_draw_line( gameAreaLeft, gameAreaUpperY, gameAreaLeft, gameAreaLowerY, al_map_rgba(255, 255, 255, 128), 5 );
	al_draw_line( gameAreaRight, gameAreaUpperY, gameAreaRight, gameAreaLowerY, al_map_rgba(255, 255, 255, 128), 5 );


	if( State == GAMESTATE_REQUEST_BULLDOG )
	{
		al_draw_filled_rectangle( 0, 0, CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight, al_map_rgba( 0, 0, 0, 128 ) );
		al_draw_bitmap( noticeBox, (CurrentConfiguration->ScreenWidth / 2) - (al_get_bitmap_width(noticeBox) / 2), (CurrentConfiguration->ScreenHeight / 2) - (al_get_bitmap_height(noticeBox) / 2), 0 );
	}

	if( State == GAMESTATE_READY || State == GAMESTATE_INGAME )
	{
			for( int pI = 0; pI < PlayerList->count; pI++ )
			{
				Player* p = (Player*)PlayerList->ItemAt( pI );
				p->Render( (CurrentConfiguration->ScreenHeight / 6), (CurrentConfiguration->ScreenHeight / 6) );
			}
	}

	if( State == GAMESTATE_READY )
	{
		//al_draw_filled_rectangle( 0, 0, CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight, al_map_rgba( 0, 0, 0, 128 ) );
		if( readyFontSize != GameCountdown % SCREEN_FPS )
		{
			if( readyFont != 0 )
				al_destroy_font( readyFont );
			readyFont = al_load_font( "resource/forte.ttf", ((GameCountdown % SCREEN_FPS) * 4) + 24, 0 );				
			readyFontSize = GameCountdown % SCREEN_FPS;
		}
		if( (GameCountdown / SCREEN_FPS) == 0 )
		{
			al_draw_text( readyFont, al_map_rgb(255, 255, 255), (CurrentConfiguration->ScreenWidth / 2), (CurrentConfiguration->ScreenHeight / 2) - (readyFont->height / 2), ALLEGRO_ALIGN_CENTRE, "GO!" );
		} else {
			char cDownTxt[10];
			sprintf( cDownTxt, "%d", GameCountdown / SCREEN_FPS );
			al_draw_text( readyFont, al_map_rgb(255, 255, 255), (CurrentConfiguration->ScreenWidth / 2), (CurrentConfiguration->ScreenHeight / 2) - (readyFont->height / 2), ALLEGRO_ALIGN_CENTRE, cDownTxt );
		}
	}

	if( State == GAMESTATE_RESULTS )
	{
		al_draw_filled_rectangle( 0, 0, CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight, al_map_rgba( 0, 0, 0, 128 ) );
	}

}

