/*-----------------------------------------------------------------------------

File: Project1/main.cpp

Functions:
	main()

-----------------------------------------------------------------------------*/

#include <Windows.h>

#include "Project1.h"
#include "screen.h"
#include "player.h"

//---------------------------------------------------------------------------------
// Function: main()
//
// Title: Greeting and Main Code
//
// Description: Show a greeting message and calls several functions
//				to run the program. 
//
// Programmer: Simon Bennett and Reinaldo Daniswara
// 
// Date: 2/2/2015
//
// Version: 1.0
// 
//      Environment:	PC 
//      Software: MS Windows 7
//      Compiles under Microsoft Visual Studio 2013
//
// Output: Screen display of welcome message
//
// Calls: screen.prompt(), screen.PromptSize(), PlayerCreate()
//		PlayerSetup(), PlayerTurn(), PlayerDelete(), screen.PromptPlayAgain()
//		screen.GetKeyboardInput()
//		
// 
// Returns: EXIT_SUCCESS for successful run 
//
// History Log:
//	1/31/2016  SB and RD  completed version 1.0
// ------------------------------------------------------------------------------

int main(void)
{
	Screen screen;
	Player *player1;
	Player *player2;
	Player *active_player;
	Player *inactive_player;

	screen.prompt("Welcome to Sink The Fleet by Simon and Rei");
	while (!screen.GetKeyboardInput()) {}
	for (;;)
	{
		GameSize size = screen.PromptSize();
		player1 = PlayerCreate(size);
		player2 = PlayerCreate(size);

		PlayerSetup(player1, screen);
		PlayerSetup(player2, screen);

		active_player = player1;
		inactive_player = player2;
		for (;;)
		{
			if (active_player == player1)
			{
				screen.prompt("Player 1 Open Fire!");
				while (!screen.GetKeyboardInput()) {}
			}
			else
			{
				screen.prompt("Player 2 Open Fire!");
				while (!screen.GetKeyboardInput()) {}
			}
			PlayerTurn(active_player, inactive_player, screen);
			if (active_player == player1)
			{
				active_player = player2;
				inactive_player = player1;
			}
			else
			{
				active_player = player1;
				inactive_player = player2;
			}

			if (player1->m_piecesLeft == 0)
			{
				screen.prompt("Player 2 Wins!");
				while (!screen.GetKeyboardInput()) {}
				break;
			}
			if (player2->m_piecesLeft == 0)
			{
				screen.prompt("Player 1 Wins!");
				while (!screen.GetKeyboardInput()) {}
				break;
			}
		}
		PlayerDelete(player1);
		PlayerDelete(player2);
		if (!screen.PromptPlayAgain()) break;
	}
	screen.prompt("Goodbye!");
	while (!screen.GetKeyboardInput()) {}
	return EXIT_SUCCESS;
}