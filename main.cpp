#define _core
#include "main.h"
#include "boot.h"
#include "Config/configure.h"

int main( int argc, char* argv[] )
{
	ALLEGRO_EVENT e;
	if( !al_init() )
	{
		return -1;
	}
	
	al_init_font_addon();
	if( !al_install_keyboard() || !al_install_joystick() || !al_init_primitives_addon() || !al_init_ttf_addon() )
	{
		return -1;
	}

	GameStack = new StageStack();
	CurrentConfiguration = new Configuration();

	if( CurrentConfiguration->FullScreen )
		al_set_new_display_flags( ALLEGRO_FULLSCREEN );

	bool foundMode = false;
	int fallbackW = 800;
	int fallbackH = 600;
	for( int modeIdx = 0; modeIdx < al_get_num_display_modes(); modeIdx++ )
	{
		if( al_get_display_mode( modeIdx, &ScreenMode ) != NULL )
		{
			if( ScreenMode.width == CurrentConfiguration->ScreenWidth && ScreenMode.height == CurrentConfiguration->ScreenHeight )
			{
				foundMode = true;
			} else {
				fallbackW = ScreenMode.width;
				fallbackH = ScreenMode.height;
			}
		}

		if( foundMode )
			break;
	}


	if( foundMode )
	{
		Screen = al_create_display( CurrentConfiguration->ScreenWidth, CurrentConfiguration->ScreenHeight );
	} else {
		Screen = al_create_display( fallbackW, fallbackH );
	}
	
	EventQueue = al_create_event_queue();
	al_register_event_source( EventQueue, al_get_display_event_source( Screen ) );
	al_register_event_source( EventQueue, al_get_keyboard_event_source() );

	GameStack->Push( (Stage*)new BootUp() );

	while( !Quit )
	{
		if( GameStack->IsEmpty() )
		{
			Quit = true;
		} else {

			while( al_get_next_event( EventQueue, &e ) )
			{
				switch( e.type )
				{
					case ALLEGRO_EVENT_DISPLAY_CLOSE:
						Quit = true;
						break;
					case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION:
						al_reconfigure_joysticks();
						break;
					default:
						GameStack->Current()->Event( &e );
						break;
				}
			}
			al_clear_to_color( al_map_rgb( 0, 0, 0 ) );
			GameStack->Current()->Render();
			al_flip_display();
		}
	}

	al_destroy_event_queue( EventQueue );
	al_destroy_display( Screen );

	return 0;
}