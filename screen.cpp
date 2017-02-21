/*-----------------------------------------------------------------------------

File: Project1/screen.cpp

Functions:
	Screen()
	clearscreen()
	clearscreen()
	PromptPlayAgain()
	PromptSize()
	PromptSize()
	prompt()
	drawGameGrid()
	GetKeyboardInput()
	SetCursor()
	BigCursor()
	HideCursor()
-----------------------------------------------------------------------------*/

#include "Project1.h"
#include "screen.h"
#include "player.h"

#include <iostream>
#include <Windows.h>

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        Screen()
//	description:   Constructor for Screen() - Opens handles to the console
//					screen buffer and standard input buffer, and hides the cursor
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        None
//	Calls:         None
//	Called By:     N/A
//	Parameters:    None
//	Returns:       N/A
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

Screen::Screen()
{
	screen_buffer = CreateFile(
		"CONOUT$",
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_WRITE,
		NULL,
		OPEN_EXISTING,
		NULL,
		NULL);

	GetConsoleCursorInfo(screen_buffer, &old_cursor);
	old_cursor.bVisible = FALSE;
	SetConsoleCursorInfo(screen_buffer, &old_cursor);

	input_buffer = GetStdHandle(STD_INPUT_HANDLE);

	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(screen_buffer, &buffer_info);

	centroid.X = (buffer_info.srWindow.Right - buffer_info.srWindow.Left) / 2;
	centroid.Y = (buffer_info.srWindow.Bottom - buffer_info.srWindow.Top) / 2;
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        clearscreen()
//	description:   clear the screen
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        None
//	Calls:         None
//	Called By:     Screen::prompt(), Screen::drawGameGrid()
//	Parameters:    None
//	Returns:       Clear Screen
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void Screen::clearscreen()
{
	system("cls");
	//TODO: directly clear the screen buffer instead of calling system()
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        PromptPlayAgain()
//	description:   Display a play again message and wait for player to press
//					the Y or N key
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         Y or N 
//	Output:        a play again message
//	Calls:         GetKeyboardInput()
//	Called By:     main()
//	Parameters:    None
//	Returns:       True if input Y and False if input N
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

bool Screen::PromptPlayAgain()
{
	prompt("Play again? Y/N");
	for (;;)
		switch (GetKeyboardInput())
		{
		case (Y_KEY) :
			return TRUE;
		case (N_KEY) :
			return FALSE;
		}
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        PromptLoadFile()
//	description:   Prompt the player to load the ship configuration from a file
//					and wait for player to press the Y or N key
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   2/2/2016
//	Input:         Y or N 
//	Output:        A load screen prompt
//	Calls:         GetKeyboardInput()
//	Called By:     main()
//	Parameters:    None
//	Returns:       True if input Y and False if input N
//	History Log:
//	               2/2/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

bool Screen::PromptLoadFile()
{
	prompt("Load ship layout from a file? Y/N");
	for (;;)
		switch (GetKeyboardInput())
		{
		case (Y_KEY) :
			return TRUE;
		case (N_KEY) :
			return FALSE;
		}
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        Screen::PromptSize()
//	description:   Display an option for user to choose small or large
//				   grid. 
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         L or S
//	Output:        Grid size option 
//	Calls:         GetKeyboardInput()
//	Called By:     main(), PlayerCreate()
//	Parameters:    None
//	Returns:       SMALL if input S and LARGE if input L
//	History Log:
//	               1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

GameSize Screen::PromptSize()
{
	prompt("Press L/S for large/small game grid");
	for (;;)
	{
		switch (GetKeyboardInput())
		{
		case (S_KEY) :
			return SMALL;
		case (L_KEY) :
			return LARGE;
		}
	}
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        prompt(std::string msg)
//	description:   display a textbox in the screen
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        ASCII char in form of a box
//	Calls:         clearscreen()
//	Called By:     Screen::PromptPlayAgain(), GameSize Screen::PromptSize(), 
//				   PlayerSetup(), main()
//	Parameters:    GameSize size
//	Returns:       player 
//	History Log:
//	                1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------
void Screen::prompt(std::string msg)
{
	clearscreen();
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(screen_buffer, &buffer_info);
	COORD start_pos;
	start_pos.X = (buffer_info.srWindow.Right - buffer_info.srWindow.Left) / 2;
	start_pos.Y = (buffer_info.srWindow.Bottom - buffer_info.srWindow.Top) / 2;
	start_pos.X -= ((msg.length() / 2) + 2);
	start_pos.Y -= 2;
	SetConsoleCursorPosition(screen_buffer, start_pos);
	putchar(UL_ELBOW);
	for (unsigned int x = 0; x < (msg.length() + 2); x++)
		putchar(H_LINE); 
	putchar(UR_ELBOW);
	start_pos.Y++;
	SetConsoleCursorPosition(screen_buffer, start_pos);
	putchar(V_LINE);
	putchar(' ');
	std::cout << msg;
	putchar(' ');
	putchar(V_LINE);
	start_pos.Y++;
	SetConsoleCursorPosition(screen_buffer, start_pos);
	putchar(LL_ELBOW);
	for (unsigned int x = 0; x < (msg.length() + 2); x++)
		putchar(H_LINE);
	putchar(LR_ELBOW);

	//TODO: use windows console functions to draw the box instead of putchars
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        FilePrompt()
//	description:   Prompt the player to enter a file name
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   2/2/2016
//	Input:         Keyboard input
//	Output:        ASCII char in form of a box
//	Calls:         clearscreen()
//	Called By:     
//	Parameters:    None
//	Returns:       File name
//	History Log:
//	                2/2/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

std::string Screen::FilePrompt()
{
	std::string file_name;
	std::string msg = "Enter the name of the file:";
	clearscreen();
	CONSOLE_SCREEN_BUFFER_INFO buffer_info;
	GetConsoleScreenBufferInfo(screen_buffer, &buffer_info);
	COORD start_pos;
	start_pos.X = (buffer_info.srWindow.Right - buffer_info.srWindow.Left) / 2;
	start_pos.Y = (buffer_info.srWindow.Bottom - buffer_info.srWindow.Top) / 2;
	start_pos.X -= ((msg.length() / 2) + 2);
	start_pos.Y -= 2;
	SetConsoleCursorPosition(screen_buffer, start_pos);
	putchar(UL_ELBOW);
	for (unsigned int x = 0; x < (msg.length() + 2); x++)
		putchar(H_LINE);
	putchar(UR_ELBOW);
	start_pos.Y++;

	SetConsoleCursorPosition(screen_buffer, start_pos);
	putchar(V_LINE);
	putchar(' ');
	std::cout << msg;
	putchar(' ');
	putchar(V_LINE);
	start_pos.Y++;

	SetConsoleCursorPosition(screen_buffer, start_pos);
	putchar(V_LINE);
	for (unsigned int x = 0; x < (msg.length() + 2); x++)
		putchar(' ');
	putchar(V_LINE);
	start_pos.Y++;

	SetConsoleCursorPosition(screen_buffer, start_pos);
	putchar(LL_ELBOW);
	for (unsigned int x = 0; x < (msg.length() + 2); x++)
		putchar(H_LINE);
	putchar(LR_ELBOW);
	start_pos.Y--;
	start_pos.X++;
	SetConsoleCursorPosition(screen_buffer, start_pos);
	old_cursor.bVisible = TRUE;
	SetConsoleCursorInfo(screen_buffer, &old_cursor);
	getline(std::cin, file_name, '\n');
	old_cursor.bVisible = FALSE;
	SetConsoleCursorInfo(screen_buffer, &old_cursor);
	return file_name;
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        drawGameGrid(Ship** game_grid, short rows, short columns)
//	description:   draws the game grid on screen
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        ASCII char to display the ships
//	Calls:         clearscreen()   
//	Called By:     PlayerTurn(), PlayerPlaceShip()
//	Parameters:    Ship** game_grid, short rows, short columns
//	Returns:       None
//	History Log:
//	                1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void Screen::drawGameGrid(Ship** game_grid, short rows, short columns)
{
	clearscreen();
	COORD position = centroid;
	position.X -= ((columns / 2) + 2);
	position.Y -= ((rows / 2) +2);

	SetConsoleCursorPosition(screen_buffer, position);
	putchar(UL_ELBOW);
	for (int x = 0; x < columns; x++)
		putchar(H_LINE);
	putchar(UR_ELBOW); 
	position.Y++;
	SetConsoleCursorPosition(screen_buffer, position);
	for (int y = 0; y < rows; y++)
	{
		putchar(V_LINE);
		for (int x = 0; x < columns; x++)
		{
			switch (game_grid[x][y])
			{
			case (NOSHIP) :
				SetConsoleTextAttribute(
					screen_buffer, 
					FOREGROUND_BLUE | 
					FOREGROUND_GREEN | 
					FOREGROUND_INTENSITY | 
					BACKGROUND_BLUE
				);
				putchar(WAVE);
				break;
			case (MINESWEEPER) :
				SetConsoleTextAttribute(
					screen_buffer,
					FOREGROUND_BLUE |
					FOREGROUND_GREEN |
					FOREGROUND_RED |
					FOREGROUND_INTENSITY |
					BACKGROUND_INTENSITY
				);
				putchar('M');
				break;
			case (FRIGATE) :
				SetConsoleTextAttribute(
					screen_buffer,
					FOREGROUND_BLUE |
					FOREGROUND_GREEN |
					FOREGROUND_RED |
					FOREGROUND_INTENSITY |
					BACKGROUND_INTENSITY
				);
				putchar('F');
				break;
			case (SUB) :
				SetConsoleTextAttribute(
					screen_buffer,
					FOREGROUND_BLUE |
					FOREGROUND_GREEN |
					FOREGROUND_RED |
					FOREGROUND_INTENSITY |
					BACKGROUND_INTENSITY
				);
				putchar('S');
				break;
			case (BATTLESHIP) :
				SetConsoleTextAttribute(
					screen_buffer,
					FOREGROUND_BLUE |
					FOREGROUND_GREEN |
					FOREGROUND_RED |
					FOREGROUND_INTENSITY |
					BACKGROUND_INTENSITY
				);
				putchar('B');
				break;
			case (CARRIER) :
				SetConsoleTextAttribute(
					screen_buffer,
					FOREGROUND_BLUE |
					FOREGROUND_GREEN |
					FOREGROUND_RED |
					FOREGROUND_INTENSITY |
					BACKGROUND_INTENSITY
				);
				putchar('C');
				break;
			case (HIT) :
				SetConsoleTextAttribute(
					screen_buffer,
					FOREGROUND_GREEN |
					FOREGROUND_RED |
					FOREGROUND_INTENSITY |
					BACKGROUND_RED |
					BACKGROUND_INTENSITY
				);
				putchar('*');
				break;
			case (MISSED) :
				SetConsoleTextAttribute(
					screen_buffer,
					FOREGROUND_BLUE |
					FOREGROUND_GREEN |
					FOREGROUND_RED |
					FOREGROUND_INTENSITY |
					BACKGROUND_INTENSITY
				);
				putchar(SMOKE);
				break;
			case (INVALID) :
				SetConsoleTextAttribute(
				screen_buffer,
				FOREGROUND_GREEN |
				FOREGROUND_RED |
				FOREGROUND_INTENSITY |
				BACKGROUND_RED
				);
				putchar('!');
				break;
			}
			SetConsoleTextAttribute(
				screen_buffer,
				FOREGROUND_BLUE |
				FOREGROUND_GREEN |
				FOREGROUND_RED
			);
		}
		putchar(V_LINE);
		position.Y++;
		SetConsoleCursorPosition(screen_buffer, position);
	}
	putchar(LL_ELBOW);
	for (int x = 0; x < columns; x++)
		putchar(H_LINE);
	putchar(LR_ELBOW);
	//TODO: Use windows console functions to draw blocks of memory to the screen
	//		instead of using putchar, and perhaps switching screen buffers to 
	//		avoid flicker
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        Screen::GetKeyboardInput()
//	description:   Read the console input buffer for keyboard events
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         Keyboard input
//	Output:        None
//	Calls:         None
//	Called By:     PromptPlayAgain(), PromptSize(), PlayerSetup(), PlayerTurn(), 
//				   PlayerPlaceShip(), main()
//	Parameters:    None
//	Returns:       virtual scan code of the pressed key
//	History Log:
//	                1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

WORD Screen::GetKeyboardInput()
{
	INPUT_RECORD event_buffer;
	DWORD number_read;

	ReadConsoleInput(input_buffer, &event_buffer, 1, &number_read);
	if (number_read)
	{
		if (event_buffer.EventType == KEY_EVENT)
		{
			if (event_buffer.Event.KeyEvent.bKeyDown)
				return event_buffer.Event.KeyEvent.wVirtualKeyCode;
		}
	}
	return NULL;
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        SetCursor(COORD position)
//	description:   Set cursor position
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        None
//	Calls:         None		   
//	Called By:     PlayerTurn()
//	Parameters:    COORD position
//	Returns:       None
//	History Log:
//	                1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void Screen::SetCursor(COORD position)
{
	SetConsoleCursorPosition(screen_buffer, position);
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        BigCursor()
//	description:   Sets cursor size to maximum and makes it visible
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        None
//	Calls:         None
//	Called By:     PlayerTurn()
//	Parameters:    None
//	Returns:       None
//	History Log:
//	                1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void Screen::BigCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	cursor.dwSize = 100;
	cursor.bVisible = TRUE;
	SetConsoleCursorInfo(screen_buffer, &cursor);
}

//-----------------------------------------------------------------------------
//	Class:         Screen
//	method:        HideCursor()
//	description:   Turns off cursor visibility
//	Programmer:	   Simon Bennett & Reinaldo Daniswara
//	Date:		   1/31/2016
//	Input:         None
//	Output:        None
//	Calls:         None
//	Called By:     PlayerTurn()
//	Parameters:    None
//	Returns:       None
//	History Log:
//	                1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

void Screen::HideCursor()
{
	CONSOLE_CURSOR_INFO cursor;
	GetConsoleCursorInfo(screen_buffer, &cursor);
	cursor.bVisible = FALSE;
	SetConsoleCursorInfo(screen_buffer, &cursor);
}