/*! \file ansi.h
    \brief ANSI and VT100 Functions

	These functions provide control
	over the users ANSI capable console.
*/

#ifndef CONSOLE_H_
#define CONSOLE_H_


#define CONSOLE_ATTR_NORMAL		0  /** Normal Attribute */
#define CONSOLE_ATTR_BOLD	 	1  /** Bold */
#define CONSOLE_ATTR_DIM		2  /** Dim */
#define CONSOLE_ATTR_UNDERLINE 	4  /** Underline */
#define CONSOLE_ATTR_BLINK		5  /** Blink */
#define CONSOLE_ATTR_REVERSE	7  /** Reverse/Invert */
#define CONSOLE_ATTR_HIDDEN		8  /** Hidden */

#define CONSOLE_COLOR_BLACK 	0  /** Black */
#define CONSOLE_COLOR_RED		1  /** Red */
#define CONSOLE_COLOR_GREEN		2  /** Green */
#define CONSOLE_COLOR_YELLOW	3  /** Yellow */
#define CONSOLE_COLOR_BLUE		4  /** Blue */
#define CONSOLE_COLOR_MAGENTA	5  /** Magenta */
#define CONSOLE_COLOR_CYAN		6  /** Cyan */
#define	CONSOLE_COLOR_WHITE		7  /** White */
#define	CONSOLE_COLOR_ORIGINAL	9  /** Original */

extern void Console_textcolor(char port, char attr,char fg, char bg);
extern char Console_getsize(char port, int *width,int*height);
extern void Console_cls(char port);
extern void Console_SaveCursor(char port);
extern void Console_RestoreCursor(char port);
extern void Console_reset(char port);
extern void Console_Cursor_Home(char port);
extern void Console_Draw_Box(char port,char x,char y,char width,char height);

extern void Console_Cursor_Up(char port, char x);
extern void Console_Cursor_Down(char port, char x);
extern void Console_Cursor_Left(char port, char x);
extern void Console_Cursor_Right(char port, char x);
extern void Console_Cursor_Set(char port, char row, char column);
extern char Console_Cursor_Get(char port, char *row, char *column);
extern void Console_Erase_Above(char port);
extern void Console_Erase_Below(char port);
extern void Console_Erase_Right(char port);
extern void Console_Erase_Left(char port);
extern void Console_Erase_Line(char port);
extern void Console_Insert_Lines(char port, char x);
extern void Console_Delete_Lines(char port, char x);
extern void Console_Delete_Chars(char port, char x);
extern void Console_Scroll_Up(char port, char x);
extern void Console_Scroll_Down(char port, char x);
extern void Console_Repeat_Char(char port, char x);
extern char Console_query(char port, char e);
void Console_Attribute(char port,char attr);
#endif

