
#include "boot.h"
#include "Config/configure.h"
#include "menu.h"

void BootUp::Begin()
{
	if( !CurrentConfiguration->WasLoaded )
	{
		GameStack->Push( (Stage*)CurrentConfiguration );
	} else {
		Resume();
	}
}

void BootUp::Pause()
{
}

void BootUp::Resume()
{
	GameStack->Pop();
	GameStack->Push( (Stage*)new Menu() );
}

void BootUp::Finish()
{
}

void BootUp::Event(ALLEGRO_EVENT *e)
{
}

void BootUp::Update()
{
}

void BootUp::Render()
{
}

