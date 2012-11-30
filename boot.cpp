
#include "boot.h"
#include "configure.h"
#include "menu.h"

void BootUp::Begin()
{
	/*
	if( !CurrentConfiguration->WasLoaded )
	{
		GameStack->Push( (Stage*)CurrentConfiguration );
	} else {
		Resume();
	}
	*/
	GameStack->Pop();
	GameStack->Push( (Stage*)new Menu() );
}

void BootUp::Pause()
{
}

void BootUp::Resume()
{

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

