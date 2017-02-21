/*-----------------------------------------------------------------------------

File: Project1/screen.h
Contains screen class definition

-----------------------------------------------------------------------------*/

#pragma once

#include <Windows.h>
#include <string>

enum GameSize;
enum Ship;
struct Player;
//-----------------------------------------------------------------------------
//	Title:          Screen Class
//
//	Description:    This file contains the class definition for Screen
//
//	Programmer:     Simon Bennett and Reinaldo Daniswara
//   
//	Date:           2/2/2016
// 
//	Version:	1.0
//   
//	Environment:	PC 
//				Software:   MS Windows 7
//				Compiles under Microsoft Visual Studio 2013
//
//	class Screen:
//
//	Properties:
//			HANDLE screen_buffer;
//			HANDLE input_buffer;
//			void clearscreen();
//			CONSOLE_CURSOR_INFO old_cursor;
//
//  Methods :
//			Screen();
//			void prompt(std::string msg);
//			bool PromptPlayAgain();
//			GameSize PromptSize();
//			void drawGameGrid(Player* player);
//			WORD Screen::GetKeyboardInput();
//			void BigCursor();
//			void HideCursor();
//			void SetCursor(COORD position);
//			COORD centroid;
//
//  History Log:
//          	1/31/2016  SB and RD  completed version 1.0
//-----------------------------------------------------------------------------

class Screen
{
public:
	Screen();
	void prompt(std::string msg);
	std::string FilePrompt();
	bool PromptLoadFile();
	bool PromptPlayAgain();
	GameSize PromptSize();
	void drawGameGrid(Ship** game_grid, short rows, short columns);
	WORD Screen::GetKeyboardInput();
	void BigCursor();
	void HideCursor();
	void SetCursor(COORD position);
	COORD centroid;

private:
	HANDLE screen_buffer;
	HANDLE input_buffer;
	void clearscreen();
	CONSOLE_CURSOR_INFO old_cursor;
};

