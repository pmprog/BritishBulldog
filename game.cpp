
#include "game.h"
#include "configure.h"
#include "player.h"

void Game::Begin()
{
	bkgSky = al_load_bitmap( "resource/game_sky.png" );
	bkgGrnd = al_load_bitmap( "resource/game_ground.png" );
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

	for( int i = 0; i < PlayerList->count; i++ )
	{
		Player* p = (Player*)PlayerList->ItemAt(i);
		p->State = STATE_AWAIT_ENTRY;
	}
}

void Game::Event(ALLEGRO_EVENT *e)
{
	if( e->type == ALLEGRO_EVENT_KEY_DOWN )
		GameStack->Pop();
}

void Game::Update()
{
}

void Game::Render()
{
	al_draw_scaled_bitmap( bkgSky, 0, 0, al_get_bitmap_width(bkgSky), al_get_bitmap_height(bkgSky), 0, 0, CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight / 8, 0 );
	al_draw_scaled_bitmap( bkgGrnd, 0, 0, al_get_bitmap_width(bkgGrnd), al_get_bitmap_height(bkgGrnd), 0, CurrentConfiguration->ScreenHeight / 8, CurrentConfiguration->ScreenWidth, (CurrentConfiguration->ScreenHeight / 8) * 7, 0 );
}

