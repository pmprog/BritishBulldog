
#include "configure.h"

Configuration::Configuration()
{
	LoadSettings();
}

Configuration::~Configuration()
{
}

void Configuration::InitSettings()
{
	ScreenWidth = 800;
	ScreenHeight = 600;
	FullScreen = false;
	WasLoaded = false;
}

void Configuration::LoadSettings()
{
	// TODO: Check if file exists, then load, otherwise Init
	InitSettings();
}

void Configuration::SaveSettings()
{
}


void Configuration::Begin()
{
	configFont = al_load_font( "resource/forte.ttf", 48, 0 );
}

void Configuration::Pause()
{
}

void Configuration::Resume()
{
}

void Configuration::Finish()
{
	al_destroy_font( configFont );
}

void Configuration::Event(ALLEGRO_EVENT *e)
{
}

void Configuration::Update()
{
}

void Configuration::Render()
{
	char numJoy[200];
	sprintf( numJoy, "%d Joysticks", al_get_num_joysticks() );
	al_draw_text( configFont, al_map_rgb( 128, 220, 255 ), 0, 0, ALLEGRO_ALIGN_LEFT, numJoy );
}
