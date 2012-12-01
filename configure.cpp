
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
	int curDev = al_get_new_display_adapter();
	ALLEGRO_MONITOR_INFO curMon;

	if( al_get_monitor_info(0, &curMon) )
	{
		ScreenWidth = curMon.x2 - curMon.x1;
		ScreenHeight = curMon.y2 - curMon.y1;
#ifdef _DEBUG
		FullScreen = false;
#else
		FullScreen = true;
#endif
	} else {
		ScreenWidth = 1024;
		ScreenHeight = 600;
		FullScreen = false;
	}
	WasLoaded = true;
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
}

void Configuration::Pause()
{
}

void Configuration::Resume()
{
}

void Configuration::Finish()
{
}

void Configuration::Event(ALLEGRO_EVENT *e)
{
}

void Configuration::Update()
{
}

void Configuration::Render()
{
}
