
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
