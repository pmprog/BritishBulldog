
#include "boot.h"
#include "configure.h"
#include "menu.h"

void BootUp::Begin()
{
	SkinBase = al_load_bitmap( "resource/skins.png" );
	SkinHair = al_load_bitmap( "resource/skins_hair.png" );
	SkinTeam = al_load_bitmap( "resource/skins_team.png" );
	SkinSkin = al_load_bitmap( "resource/skins_skin.png" );

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

