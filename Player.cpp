/*-----------------------------------------------------------------------------

File: Project1/player.cpp

Functions:
	Player()
	PlayerCreate()
	PlayerDelete()
	PlayerSetup()
	PlayerTurn()
	PlayerPlaceShip()
	PlayerCreateGrid()
	PlayerDeleteGrid()

-----------------------------------------------------------------------------*/

#include "Project1.h"
#include "player.h"
#include "screen.h"

#include <fstream>

//-----------------------------------------------------------------------------
//	Function:	   Player()
//	Description:   Initializing constructor for Player struct
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        None
//	Calls:         None
//	Called By:     N/A
//	Parameters:    None
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------
Player::Player()
{
	gridColumns = 0;
	gridRows = 0;
	m_gameGrid[0] = NULL;
	m_gameGrid[1] = NULL; 
	m_piecesLeft = INITIAL_PIECES;
}

//-----------------------------------------------------------------------------
//	Function:      PlayerCreate(GameSize size)
//	Description:   Factory function for Player struct
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        returns a pointer to the initialized Player struct
//	Calls:         PlayerCreateGrid()
//	Called By:     main()
//	Parameters:    GameSize size - size of game grid
//	Returns:       Pointer to Player 
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

Player* PlayerCreate(GameSize size)
{
	Player* player = new Player;

	switch (size)
	{
	case (SMALL) :
		player->gridColumns = SMALL_COLS;
		player->gridRows = SMALL_ROWS;
		player->gridSize = SMALL;
		break;
	case (LARGE) :
		player->gridColumns = LARGE_COLS;
		player->gridRows = LARGE_ROWS;
		player->gridSize = LARGE;
		break;
	}
	player->m_gameGrid[0] = PlayerCreateGrid(player->gridRows, player->gridColumns);
	player->m_gameGrid[1] = PlayerCreateGrid(player->gridRows, player->gridColumns);
	return player;
}

//-----------------------------------------------------------------------------
//	Function:      PlayerCreateGrid(short rows, short columns)
//	Description:   Dynamically allocates memory for a game grid array of size
//						rows * columns and initializes each cell to NOSHIP
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        None
//	Calls:         None
//	Called By:     PlayerCreate(), PlayerPlaceShip()
//	Parameters:    short rows, short columns - dimensions of game grid
//	Returns:       A pointer to the dynamically allocated array game_grid
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

Ship** PlayerCreateGrid(short rows, short columns)
{
	Ship** game_grid = new Ship*[columns];
	for (int x = 0; x < columns; x++)
	{
		game_grid[x] = new Ship[rows];
		for (int y = 0; y < rows; y++)
		{
			game_grid[x][y] = NOSHIP;
		}
	}
	return game_grid;
}

//-----------------------------------------------------------------------------
//	Function:      PlayerDeleteGrid(Ship** game_grid, short columns)
//	description:   deletes the dynamically allocated game grid
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        None
//	Calls:         None
//	Called By:     PlayerDelete(), PlayerPlaceShip()
//	Parameters:    Ship** game_grid, short columns - width of game grid
//	Returns:       None
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void PlayerDeleteGrid(Ship** game_grid, short columns)
{
	if (game_grid != NULL) 
	{
		for (int x = 0; x < columns; x++)
		{
			delete game_grid[x];
		}
		delete game_grid;
	}
}

//-----------------------------------------------------------------------------
//	Function:      void PlayerDelete(Player* player)
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	description:   Free dynamically allocated memory in the Player class.
//	Input:         None
//	Output:        None
//	Calls:         PlayerDeleteGrid()
//	Called By:     main() 
//	Parameters:    Player* player
//	Returns:       None
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void PlayerDelete(Player* player)
{
	PlayerDeleteGrid(player->m_gameGrid[0], player->gridColumns);
	PlayerDeleteGrid(player->m_gameGrid[1], player->gridColumns);
}

//-----------------------------------------------------------------------------
//	Function:      void PlayerSetup(Player* player, Screen& screen)
//	description:   Prompts the player to place their ships
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        None
//	Calls:         PlayerPlaceShip()
//	Called By:     main()
//	Parameters:    Player* player, Screen& screen
//	Returns:       None
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void PlayerSetup(Player* player, Screen& screen)
{
		screen.prompt("Use arrow keys to position; press R to rotate");
		while (!screen.GetKeyboardInput()) {}
		PlayerPlaceShip(player, MINESWEEPER, screen);
		PlayerPlaceShip(player, SUB, screen);
		PlayerPlaceShip(player, FRIGATE, screen);
		PlayerPlaceShip(player, BATTLESHIP, screen);
		PlayerPlaceShip(player, CARRIER, screen);

}

//-----------------------------------------------------------------------------
//	Function:      PlayerTurn(Player* active_player, 
//						Player* inactive_player, Screen& screen)
//	description:   receive keyboard input, set position to hit, check is it hit or not,
//					if a ship is sunk, display a congratulatory message and then break
//					out of the game loop if the player misses or sinks all the enemy ships
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         Keyboard input (arrow up, down, left, right and return)
//	Output:        screen will display the cursor, which movable up, down, left
//				   and right; draws game grid to screen
//	Calls:         screen.BigCursor(), screen.GetKeyboardInput(), screen.SetCursor()
//				   screen.HideCursor(), screen.DrawGameGrid()
//	Called By:     main()
//	Parameters:    Player* active_player, Player* inactive_player, Screen& screen
//	Returns:       None
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void PlayerTurn(Player* active_player, Player* inactive_player, Screen& screen)
{
	bool missed = FALSE;
	COORD grid_position = { 0 };
	COORD cursor_position;
	cursor_position.X = screen.centroid.X;
	cursor_position.Y = screen.centroid.Y;

	do
	{
		screen.BigCursor();
		switch (screen.GetKeyboardInput())
		{
		case (VK_LEFT) :
			grid_position.X--;
			if (grid_position.X < 0) grid_position.X = 0;
			break;
		case (VK_RIGHT) :
			grid_position.X++;
			if (grid_position.X >= active_player->gridColumns)
				grid_position.X = active_player->gridColumns - 1;
			break;
		case (VK_UP) :
			grid_position.Y--;
			if (grid_position.Y < 0) grid_position.Y = 0;
			break;
		case (VK_DOWN) :
			grid_position.Y++;
			if (grid_position.Y >= active_player->gridRows)
				grid_position.Y = active_player->gridRows - 1;
			break;
		case (VK_RETURN) :
			Ship* target_square;
			Ship* grid_square;
			target_square = 
				&inactive_player->m_gameGrid[0][grid_position.X][grid_position.Y];
			grid_square =
				&active_player->m_gameGrid[1][grid_position.X][grid_position.Y];
			if (*grid_square == HIT) continue;
			if (*target_square != NOSHIP)
			{
				*grid_square = HIT;
				inactive_player->m_ships[*target_square].m_piecesLeft--;
				if (!inactive_player->m_ships[*target_square].m_piecesLeft)
				{
					std::string sink_message = "You sunk the ";
					switch (*target_square)
					{
					case (MINESWEEPER) :
						sink_message += "Minesweeper!";
						break;
					case(FRIGATE) :
						sink_message += "Frigate!";
						break;
					case (SUB) :
						sink_message += "Submarine!";
						break;
					case (BATTLESHIP) :
						sink_message += "Battleship!";
						break;
					case (CARRIER) :
						sink_message += "Carrier!";
						break;
					}
					screen.HideCursor();
					screen.prompt(sink_message);
					while (!screen.GetKeyboardInput()) {}
					screen.BigCursor();
				}
				inactive_player->m_piecesLeft--;
				*target_square = NOSHIP;
			}
			else
			{
				*grid_square = MISSED;
				missed = TRUE;
			}
		}

		cursor_position = screen.centroid;
		cursor_position.X -= ((active_player->gridColumns / 2) + 1);
		cursor_position.Y -= ((active_player->gridRows / 2) + 1);
		cursor_position.X += grid_position.X;
		cursor_position.Y += grid_position.Y;
		screen.SetCursor(cursor_position);
		screen.HideCursor();
		screen.drawGameGrid(
			active_player->m_gameGrid[1],
			active_player->gridRows,
			active_player->gridColumns
			);
		screen.SetCursor(cursor_position);
		if (inactive_player->m_piecesLeft == 0) break;
	} while (!missed);
}

//-----------------------------------------------------------------------------
//	Function:      PlayerPlaceShip(Player* player, Ship ship, Screen& screen)
//	description:   Allows the player to place their ships on the game board;
//					creates a scratch game_grid structure, compares it with the 
//					existing grid to check validity, and then merges them if valid,
//					deleting the scratch space
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         Keyboard input (arrow up, down, left, right and return, r key)
//	Output:        screen will display the cursor, which movable up, down, left
//				   and right to place the ship
//	Calls:         PlayerCreateGrid(), drawGameGrid(), GetKeyboardInput()
//				   PlayerDeleteGrid()
//	Called By:     PlayerSetup()
//	Parameters:    Player* player, Ship ship, Screen& screen
//	Returns:       None
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void PlayerPlaceShip(Player* player, Ship ship, Screen& screen)
{
	Ship** wip_gameGrid = PlayerCreateGrid(player->gridRows, player->gridColumns);
	COORD grid_position = { 0 };
	ShipInfo ship_info;
	ship_info.m_name = ship;
	ship_info.m_orientation = VERTICAL;
	bool main_loop = TRUE;
	switch (ship)
	{
		case (MINESWEEPER) :
			ship_info.m_piecesLeft = 2;
			break;
		case (FRIGATE) :
		case (SUB) :
			ship_info.m_piecesLeft = 3;
			break;
		default:
			ship_info.m_piecesLeft = ship;
	}
	while (main_loop)
	{
		bool is_valid = TRUE;
		ship_info.m_bowLocation.m_col = grid_position.X;
		ship_info.m_bowLocation.m_row = grid_position.Y;

		for (int x = 0; x < player->gridColumns; x++)
		{
			for (int y = 0; y < player->gridRows; y++)
			{
				wip_gameGrid[x][y] = player->m_gameGrid[0][x][y];
			}
		}
		if (ship_info.m_orientation == VERTICAL)
		{
			for (int y = 0; y < ship_info.m_piecesLeft; y++)
			{
				if (grid_position.Y + y >= player->gridRows)
				{
					is_valid = FALSE;
					break;
				}
				if (wip_gameGrid[grid_position.X][grid_position.Y + y] != NOSHIP)
				{
					wip_gameGrid[grid_position.X][grid_position.Y + y] = INVALID;
					is_valid = FALSE;
				}
				else wip_gameGrid[grid_position.X][grid_position.Y + y] = ship;
			}
		}
		else
		{
			for (int x = 0; x < ship_info.m_piecesLeft; x++)
			{
				if (grid_position.X + x >= player->gridColumns)
				{
					is_valid = FALSE;
					break;
				}
				if (wip_gameGrid[grid_position.X + x][grid_position.Y] != NOSHIP)
				{
					wip_gameGrid[grid_position.X + x][grid_position.Y] = INVALID;
					is_valid = FALSE;
				}
				else wip_gameGrid[grid_position.X + x][grid_position.Y] = ship;
			}
		}
		screen.drawGameGrid(wip_gameGrid, player->gridRows, player->gridColumns);
		switch (screen.GetKeyboardInput())
		{
			case (VK_LEFT) :
				grid_position.X--;
				if (grid_position.X < 0) grid_position.X = 0;
				break;
			case (VK_RIGHT) :
				grid_position.X++;
				if (ship_info.m_orientation == HORIZONTAL)
				{
					if (grid_position.X >= player->gridColumns - ship_info.m_piecesLeft)
						grid_position.X = player->gridColumns - ship_info.m_piecesLeft;
				}
				if (grid_position.X >= player->gridColumns)
					grid_position.X = player->gridColumns - 1;
				break;
			case (VK_UP) :
				grid_position.Y--;
				if (grid_position.Y < 0) grid_position.Y = 0;
				break;
			case (VK_DOWN) :
				grid_position.Y++;
				if (ship_info.m_orientation == VERTICAL)
				{
					if (grid_position.Y >= player->gridRows - ship_info.m_piecesLeft)
						grid_position.Y = player->gridRows - ship_info.m_piecesLeft;
				}
				else
				{
					if (grid_position.Y >= player->gridRows)
						grid_position.Y = player->gridRows - 1;
				}
				break;
			case (R_KEY) :
				if (ship_info.m_orientation == HORIZONTAL)
					ship_info.m_orientation = VERTICAL;
				else ship_info.m_orientation = HORIZONTAL;
				break;
			case (VK_RETURN) :
				if (is_valid)
				{
					if (ship_info.m_orientation == VERTICAL)
					{
						for (int y = 0; y < ship_info.m_piecesLeft; y++)
						{
							player->m_gameGrid[0][grid_position.X][grid_position.Y + y] = ship;
						}
					}
					if (ship_info.m_orientation == HORIZONTAL)
					{
						for (int x = 0; x < ship_info.m_piecesLeft; x++)
						{
							player->m_gameGrid[0][grid_position.X + x][grid_position.Y] = ship;
						}
					}
					main_loop = FALSE;
				}
				break;
		}
	}
	player->m_ships[ship] = ship_info;
	PlayerDeleteGrid(wip_gameGrid, player->gridColumns);
}