/*-----------------------------------------------------------------------------

File: Project1/player.h

struct def for: Player
-----------------------------------------------------------------------------*/

#pragma once
#include "Project1.h"

enum { LARGE_COLS = 24, LARGE_ROWS = 10, SMALL_COLS = 12, SMALL_ROWS = 8, INITIAL_PIECES = 17 };
class Screen;

struct Player
{
	Player();
	GameSize gridSize;
	short gridColumns;
	short gridRows;
	Ship** m_gameGrid[2];
	ShipInfo m_ships[6];
	short m_piecesLeft;
};

Player* PlayerCreate(GameSize size);
void PlayerDelete(Player* player);
void PlayerSetup(Player* player, Screen& screen);
void PlayerTurn(Player* active_player, Player* inactive_player, Screen& screen);
void PlayerPlaceShip(Player* player, Ship ship, Screen& screen);
Ship** PlayerCreateGrid(short rows, short columns);
void PlayerDeleteGrid(Ship** game_grid, short columns);