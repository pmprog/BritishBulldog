
#include "game.h"
#include "configure.h"


void Game::Begin()
{
	bkgSky = al_load_bitmap( "resource/game_sky.png" );
	bkgGrnd = al_load_bitmap( "resource/game_ground.png" );
	noticeBox = al_load_bitmap( "resource/notice.png" );
	buttonUp = al_load_bitmap( "resource/button_up.png" );
	buttonDown = al_load_bitmap( "resource/button_down.png" );
	readyFont = 0;
	buttonPop = 0;

	gameAreaUpperY = CurrentConfiguration->ScreenHeight / 4;
	gameAreaLowerY = (CurrentConfiguration->ScreenHeight / 8) * 7;
	gameAreaLeft = CurrentConfiguration->ScreenWidth / 10;
	gameAreaRight = CurrentConfiguration->ScreenWidth - gameAreaLeft;

	// TODO: Populate AI players
	while( PlayerList->count < fillAIToCount )
	{
		Player* p = new Player();
		p->IsAI = true;
		PlayerList->AddToEnd( p );
	}

	State = GAMESTATE_REQUEST_BULLDOG;

	gameMusic = al_load_audio_stream( "resource/wandschrank-spannung2.ogg", 4, 2048 );
	al_set_audio_stream_playmode( gameMusic, ALLEGRO_PLAYMODE_LOOP );
	al_attach_audio_stream_to_mixer( gameMusic, mixer );
	al_set_audio_stream_playing( gameMusic, true );

	Bulldogs = new List();
	Winner = 0;

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

	al_set_audio_stream_playing( gameMusic, false );
	al_destroy_audio_stream( gameMusic );

	for( int i = PlayerList->count - 1; i >= 0; i-- )
	{
		Player* p = (Player*)PlayerList->ItemAt(i);
		if( p->IsAI )
		{
			PlayerList->RemoveAt( i );
			delete p;
		} else {
			p->State = STATE_AWAIT_ENTRY;
			p->IsBulldog = false;
			p->WasTagged = false;
		}
	}

	while( Bulldogs->count > 0 )
		Bulldogs->RemoveLast();
	delete Bulldogs;
}

void Game::Event(ALLEGRO_EVENT *e)
{
	if( e->type == ALLEGRO_EVENT_KEY_DOWN && e->keyboard.keycode == ALLEGRO_KEY_ESCAPE )
		GameStack->Pop();

	switch( State )
	{
		case GAMESTATE_REQUEST_BULLDOG:
			destinationIsRightSide = true;
			if( e->type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN )
			{
				for( int pI = 0; pI < PlayerList->count; pI++ )
				{
					Player* p = (Player*)PlayerList->ItemAt( pI );
					if( p->Joystick == e->joystick.id )
					{
						p->IsBulldog = true;
						Bulldogs->AddToEnd( (void*)p );
					}
					p->Position->y = (rand() % (gameAreaLowerY - gameAreaUpperY)) + gameAreaUpperY;
					p->Position->x = gameAreaLeft;
					p->DesintationIsRightSide = destinationIsRightSide;
				}
				State = GAMESTATE_READY;
				GameCountdown = SCREEN_FPS * 3;
				SortPlayerList();
			}
			break;
		case GAMESTATE_PREPROUND:
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
			if( WinnerCountDown > SCREEN_FPS * 4 )
				GameStack->Pop();
			break;
	}
}

void Game::Update()
{
	int playersReady = 0;
	int playersFinished = 0;
	int playersNotBulldog = 0;

	switch( State )
	{
		case GAMESTATE_REQUEST_BULLDOG:
			buttonPop = (++buttonPop) % (SCREEN_FPS * 2);
			break;
		case GAMESTATE_READY:
			GameCountdown--;
			if( GameCountdown == 0 )
			{
				State = GAMESTATE_PREPROUND;
				for( int pI = 0; pI < PlayerList->count; pI++ )
				{
					Player* p = (Player*)PlayerList->ItemAt( pI );
					p->State = STATE_STANDING;
				}
			}
			break;
		case GAMESTATE_PREPROUND:
			for( int pI = 0; pI < PlayerList->count; pI++ )
			{
				Player* p = (Player*)PlayerList->ItemAt( pI );

				p->DesintationIsRightSide = destinationIsRightSide;
				p->UserInput = INPUT_NONE;
				if( p->IsBulldog || p->WasTagged )
				{
					if( p->WasTagged && !p->IsBulldog )
					{
						p->IsBulldog = true;
						Bulldogs->AddToEnd( p );
					}
					if( p->Position->x < CurrentConfiguration->ScreenWidth / 2 && destinationIsRightSide )
						p->UserInput = INPUT_RIGHT;
					if( p->Position->x > CurrentConfiguration->ScreenWidth / 2 && !destinationIsRightSide )
						p->UserInput = INPUT_LEFT;
				}
				if( p->UserInput == INPUT_NONE )
					playersReady++;
				p->Update( false );
			}
			if( playersReady == PlayerList->count )
			{
				if( PlayerList->count == Bulldogs->count + 1 )
				{
					State = GAMESTATE_RESULTS;
					WinnerCountDown = 0;
				} else {
					State = GAMESTATE_INGAME;
				}
			}
			break;

		case GAMESTATE_INGAME:

			// Check bulldog tags first
			for( int bI = 0; bI < Bulldogs->count; bI++ )
			{
				CheckTags( (Player*)Bulldogs->ItemAt( bI ) );
			}

			// Process movements
			for( int pI = 0; pI < PlayerList->count; pI++ )
			{
				Player* p = (Player*)PlayerList->ItemAt( pI );
				if( p->IsAI )
					ProcessAI( p );
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
				} else {
					playersNotBulldog++;
					if( p->Position->x > gameAreaRight && destinationIsRightSide )
						playersFinished++;
					if( p->Position->x < gameAreaLeft && !destinationIsRightSide )
						playersFinished++;
				}
				if( p->Position->y < gameAreaUpperY )
					p->Position->y = gameAreaUpperY;
				if( p->Position->y > gameAreaLowerY )
					p->Position->y = gameAreaLowerY;

			}
			// Sort display order
			SortPlayerList();

			if( playersNotBulldog <= 0 )
			{
				State = GAMESTATE_RESULTS;
				WinnerCountDown = 0;
			} else if( playersFinished == playersNotBulldog ) {
				destinationIsRightSide = !destinationIsRightSide;
				State = GAMESTATE_PREPROUND;
			}
			break;

		case GAMESTATE_RESULTS:
			WinnerCountDown++;
			if( WinnerCountDown > SCREEN_FPS * 10 )
				GameStack->Pop();
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

		readyFont = al_load_font( "resource/forte.ttf", (CurrentConfiguration->ScreenHeight / 18), 0 );
		al_draw_text( readyFont, al_map_rgb(0, 0, 0), (CurrentConfiguration->ScreenWidth / 2), (CurrentConfiguration->ScreenHeight / 2) - readyFont->height + 2, ALLEGRO_ALIGN_CENTRE, "Press to be Bulldog" );
		al_draw_text( readyFont, al_map_rgb(255, 255, 255), (CurrentConfiguration->ScreenWidth / 2), (CurrentConfiguration->ScreenHeight / 2) - readyFont->height, ALLEGRO_ALIGN_CENTRE, "Press to be Bulldog" );
		al_destroy_font( readyFont );
		readyFont = 0;

		if( buttonPop / SCREEN_FPS == 0 )
			al_draw_bitmap( buttonDown, (CurrentConfiguration->ScreenWidth / 2) - (al_get_bitmap_width(buttonUp) / 2), (CurrentConfiguration->ScreenHeight / 2), 0 );
		else
			al_draw_bitmap( buttonUp, (CurrentConfiguration->ScreenWidth / 2) - (al_get_bitmap_width(buttonUp) / 2), (CurrentConfiguration->ScreenHeight / 2), 0 );
	}

	if( State == GAMESTATE_READY || State == GAMESTATE_INGAME || State == GAMESTATE_PREPROUND || State == GAMESTATE_RESULTS )
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
		if( Winner == 0 )
		{
			if( PlayerList->count == Bulldogs->count )
			{
				Winner = (Player*)Bulldogs->last->itemData;
			} else {
				for( int pI = 0; pI < PlayerList->count; pI++ )
				{
					Player* p = (Player*)PlayerList->ItemAt( pI );
					if( !p->IsBulldog )
					{
						Winner = p;
						break;
					}
				}
			}
		}

		al_draw_filled_rectangle( 0, 0, CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight, al_map_rgba( 0, 0, 0, 128 ) );

		al_draw_filled_rectangle( CurrentConfiguration->ScreenWidth / 4, CurrentConfiguration->ScreenHeight / 4, CurrentConfiguration->ScreenWidth / 4 * 3, CurrentConfiguration->ScreenHeight / 4 * 3, al_map_rgba( 255, 255, 255, 128 ) );
		Winner->Render( CurrentConfiguration->ScreenWidth / 2, (CurrentConfiguration->ScreenHeight / 3) * 2, (CurrentConfiguration->ScreenHeight / 3), (CurrentConfiguration->ScreenHeight / 3) );

		readyFont = al_load_font( "resource/forte.ttf", (CurrentConfiguration->ScreenHeight / 12), 0 );
		al_draw_text( readyFont, al_map_rgb(0, 0, 0), (CurrentConfiguration->ScreenWidth / 2), (CurrentConfiguration->ScreenHeight / 4) + 2, ALLEGRO_ALIGN_CENTRE, "WINNER!" );
		al_draw_text( readyFont, al_map_rgb(255, 255, 255), (CurrentConfiguration->ScreenWidth / 2), (CurrentConfiguration->ScreenHeight / 4), ALLEGRO_ALIGN_CENTRE, "WINNER!" );
		al_destroy_font( readyFont );
		readyFont = 0;
	}

}

void Game::SortPlayerList()
{
	bool changed = true;

	while( changed )
	{
		changed = false;

		for( int pI = 0; pI < PlayerList->count - 1; pI++ )
		{
			Player* p = (Player*)PlayerList->ItemAt( pI );

			for( int pIa = pI + 1; pIa < PlayerList->count; pIa++ )
			{
				Player* pa = (Player*)PlayerList->ItemAt( pIa );
				if( p->Position->y > pa->Position->y )
				{
					PlayerList->Move( pI, pIa );
					changed = true;
					break;
				}
			}
			if( changed )
				break;
		}
	}
}

void Game::CheckTags( Player* bulldog )
{
	int tags = 0;
	for( int i = 0; i < PlayerList->count; i++ )
	{
		Player* p = (Player*)PlayerList->ItemAt( i );
		if( !p->IsBulldog )
		{
			if( bulldog->Position->DistanceBetween( p->Position ) <= TAG_DISTANCE )
			{
				p->WasTagged = true;
				tags++;
			}
		}
		if( tags == 2 )
			break;
	}
}


void Game::ProcessAI( Player* player )
{
	if( player->IsBulldog )
	{
		switch( rand() % 6 )
		{
			case 0:
				player->UserInput = INPUT_NONE;
				break;
			case 1:
				player->UserInput = INPUT_UP;
				break;
			case 2:
				player->UserInput = INPUT_DOWN;
				break;
		}		
	} else {
		if( destinationIsRightSide )
			player->UserInput = INPUT_RIGHT;
		else
			player->UserInput = INPUT_LEFT;
	}

	if( rand() % 3 == 0 )
		player->UserInput |= INPUT_BUTTON;
}