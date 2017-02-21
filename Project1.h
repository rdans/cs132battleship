/*-----------------------------------------------------------------------------

File: Project1/Project1.h

enum and struct defs for: Direction, Ship, GameSize, Cell, ShipInfo

-----------------------------------------------------------------------------*/

#pragma once

enum {L_KEY = 0x4C, N_KEY = 0x4E, R_KEY = 0x52, S_KEY, Y_KEY = 0x59};
enum {SMOKE = 176, V_LINE = 186, UR_ELBOW = 187, LR_ELBOW = 188, 
		LL_ELBOW = 200, UL_ELBOW = 201, H_LINE = 205, WAVE = 247};

enum Direction 
{
	HORIZONTAL, VERTICAL
};

enum Ship 
{
	NOSHIP, MINESWEEPER, SUB, FRIGATE, BATTLESHIP, CARRIER, HIT, MISSED, INVALID
};

enum GameSize
{
	LARGE, SMALL
};

struct Cell
{
	unsigned short m_row;
	unsigned short m_col;
};

struct ShipInfo
{
	Ship m_name;
	Direction m_orientation;
	Cell m_bowLocation;
	short m_piecesLeft;
};

