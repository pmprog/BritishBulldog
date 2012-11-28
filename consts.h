
#pragma once

#define MAX_STACK_SIZE			10
#define MAX_PLAYERS					12

struct PlayerInfo
{
	float X;
	float Y;
	float Velocity;
	int Direction;		// 1 = North, 2 = East, 4 = West, 8 = South
	int Energy;
	bool Turbo;
	bool Bulldog;
};