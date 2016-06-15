/*! \file ansi.c
    \brief ANSI and VT100 Functions

	These functions provide control
	over the users ANSI capable console.
*/


#include "pic.h"
#include "ansi.h"
#include "serial.h"

void Console_Draw_Box(char port,char x,char y,char width,char height)
{
	char w,h;
	Console_Cursor_Set(port,y,x);

	for (w=0;w<width;w++) {
		szText[w] = 196;
	}
	szText[0]=218;
	szText[width-1]=191;
	szText[width]=0;
	Serial_Send_String(port,szText,SEND_NOW);
	Console_Cursor_Left(port,width);
	Console_Cursor_Down(port,1);

	for(h=1;h<height - 1;h++) {
		for (w=0;w<width;w++) {
			szText[w] = 32;
		}
		szText[0]=179;
		szText[width-1]=179;
		szText[width]=0;
		Serial_Send_String(port,szText,SEND_NOW);
		Console_Cursor_Left(port,width);
		Console_Cursor_Down(port,1);

	}

	for (w=0;w<width;w++) {
		szText[w] = 196;
	}
	szText[0]=192;
	szText[width-1]=217;
	szText[width]=0;
	Serial_Send_String(port,szText,SEND_NOW);
	Console_Cursor_Left(port,width);
	Console_Cursor_Down(port,1);


	//Console_Cursor_Down(port,1);

	

}
////////////////////////////////////
/// Cursor Home
////////////////////////////////////
void Console_Cursor_Home(char port)
{
	sprintf(szText,(FARROM*)"\x1b[H");
	Serial_Send_String(port,szText,SEND_NOW);

}


////////////////////////////////////
/// Repeat the last character x times
////////////////////////////////////
void Console_Repeat_Char(char port, char x)
{
	sprintf(szText,(FARROM*)"\x1b[%db",x);
	Serial_Send_String(port,szText,SEND_NOW);

}

////////////////////////////////////
/// Scroll the console down x lines
////////////////////////////////////
void Console_Scroll_Down(char port,char x)
{
	sprintf(szText,(FARROM*)"\x1b[%dT",x);
	Serial_Send_String(port,szText,SEND_NOW);

}
////////////////////////////////////
/// Scroll the console up x lines
////////////////////////////////////
void Console_Scroll_Up(char port,char x)
{
	sprintf(szText,(FARROM*)"\x1b[%dS",x);
	Serial_Send_String(port,szText,SEND_NOW);

}
////////////////////////////////////
/// Delete x characters
////////////////////////////////////
void Console_Delete_Chars(char port,char x)
{
	sprintf(szText,(FARROM*)"\x1b[%dP",x);
	Serial_Send_String(port,szText,SEND_NOW);

}

////////////////////////////////////
/// Delete x number of lines
////////////////////////////////////
void Console_Delete_Lines(char port,char x)
{
	sprintf(szText,(FARROM*)"\x1b[%dM",x);
	Serial_Send_String(port,szText,SEND_NOW);

}

////////////////////////////////////
/// Insert x number of lines
////////////////////////////////////
void Console_Insert_Lines(char port,char x)
{
	sprintf(szText,(FARROM*)"\x1b[%dL",x);
	Serial_Send_String(port,szText,SEND_NOW);

}

////////////////////////////////////
/// Erase current line
////////////////////////////////////
void Console_Erase_Line(char port)
{
	sprintf(szText,(FARROM*)"\x1b[2K");
	Serial_Send_String(port,szText,SEND_NOW);

}

////////////////////////////////////
/// Erase left
////////////////////////////////////
void Console_Erase_Left(char port)
{
	sprintf(szText,(FARROM*)"\x1b[1K");
	Serial_Send_String(port,szText,SEND_NOW);

}
////////////////////////////////////
/// Erase right
////////////////////////////////////
void Console_Erase_Right(char port)
{
	sprintf(szText,(FARROM*)"\x1b[0K");
	Serial_Send_String(port,szText,SEND_NOW);

}

////////////////////////////////////
/// Erase below
////////////////////////////////////
void Console_Erase_Below(char port)
{
	sprintf(szText,(FARROM*)"\x1b[0J");
	Serial_Send_String(port,szText,SEND_NOW);

}

////////////////////////////////////
/// Erase above
////////////////////////////////////
void Console_Erase_Above(char port)
{
	sprintf(szText,(FARROM*)"\x1b[1J");
	Serial_Send_String(port,szText,SEND_NOW);

}

////////////////////////////////////
/// get cursor position and return it in row and column
////////////////////////////////////
char Console_Cursor_Get(char port,char *row, char *column)
{
	int iSemicolon=-1;
	int t;
	sprintf(szText,(FARROM*)"\x1b[6n");
	Serial_Send_String(port,szText,SEND_NOW);
	if(Console_query(port,'R'))
	{
		for(t=0;t<strlen(szText);t++) {
			if(szText[t]=';') {
				iSemicolon = t;
			}
		
		}
		if(iSemicolon <= 0)
			return 0;
		szText[iSemicolon] = 0;
		*row = atoi(szText);
		*column = atoi(&szText[iSemicolon+1]);
		return 1;
		

	}
	return 0;
}
////////////////////////////////////
/// Set cursor position
////////////////////////////////////
void Console_Cursor_Set(char port,char row, char column)
{
	sprintf(szText,(FARROM*)"\x1b[%d;%dH",row,column);
	Serial_Send_String(port,szText,SEND_NOW);
}
////////////////////////////////////
/// Move cursor right x chars
////////////////////////////////////
void Console_Cursor_Right(char port,char x)
{
	sprintf(szText,(FARROM*)"\x1b[%dC",x);
	Serial_Send_String(port,szText,SEND_NOW);
}

////////////////////////////////////
/// Move cursor left x chars
////////////////////////////////////
void Console_Cursor_Left(char port,char x)
{
	sprintf(szText,(FARROM*)"\x1b[%dD",x);
	Serial_Send_String(port,szText,SEND_NOW);
}

////////////////////////////////////
/// Move cursor down x chars
////////////////////////////////////
void Console_Cursor_Down(char port,char x)
{
	sprintf(szText,(FARROM*)"\x1b[%dB",x);
	Serial_Send_String(port,szText,SEND_NOW);
}

////////////////////////////////////
/// Move cursor up x chars
////////////////////////////////////
void Console_Cursor_Up(char port,char x)
{
	sprintf(szText,(FARROM*)"\x1b[%dA",x);
	Serial_Send_String(port,szText,SEND_NOW);
}

////////////////////////////////////
/// Reset console
////////////////////////////////////
void Console_reset(char port)
{
	sprintf(szText,(FARROM*)"\x1bc");
	Serial_Send_String(port,szText,SEND_NOW);
}

////////////////////////////////////
/// Restore cursor to last saved position
////////////////////////////////////
void Console_RestoreCursor(char port)
{
	sprintf(szText,(FARROM*)"\x1b[s");
	Serial_Send_String(port,szText,SEND_NOW);
}

////////////////////////////////////
/// Save cursor position
////////////////////////////////////
void Console_SaveCursor(char port)
{
	sprintf(szText,(FARROM*)"\x1b[u");
	Serial_Send_String(port,szText,SEND_NOW);
}

////////////////////////////////////
/// Clear screen
////////////////////////////////////
void Console_cls(char port)
{
	sprintf(szText,(FARROM*)"\x1b[2J");
	Serial_Send_String(port,szText,SEND_NOW);
}
////////////////////////////////////
/// Get console size
////////////////////////////////////
char Console_getsize(char port,int *width,int*height)
{
	sprintf(szText,(FARROM*)"\x1b[19t");
	Serial_Send_String(port,szText,SEND_NOW);
	if(Console_query(port,'t'))
	{
		if(strlen(szText) == 4)
		{
			*width = atoi(&szText[2]);
			szText[2] = 0;
			*height = atoi(szText);
			return 1;
		}
		if(strlen(szText) == 3)
		{
			*width = atoi(&szText[1]);
			szText[1] = 0;
			*height = atoi(szText);
			return 1;
		}

	}
	return 0;
}
////////////////////////////////////
/// This function queries the console
/// and returns the value in szText
////////////////////////////////////
char Console_query(char port,char e)
{
	long counter = 0;
	char chcount = 0;
	char code;
	memset(szText,0,64);

	while (1)			// Continuous loop;
	{
		counter=counter+1;
		if(counter == 32766)
		{
			return 0;			// return failed
		}		

		////////////////////////////////////////////////////
		//  Blink the status LED
		////////////////////////////////////////////////////
		if(Status_LED)
			Status_LED=0;
		else
			Status_LED=1;

		if( Serial_RX_Data_Waiting(port) )
		{

			code=Serial_GetChar(port);

			/////////////////////////////////////////////////
			// Escape Sequences, used by terminals to move the cursor
			/////////////////////////////////////////////////
			if(code==27)		// Sequence start byte
			{
				while (!Serial_RX_Data_Waiting(port)) {}
				code=Serial_GetChar(port);

				if(code=='[')  // This means we have a sequence
				{
					while (code != e)
					{
						if(Serial_RX_Data_Waiting(port)) 
						{
							code=Serial_GetChar(port);
							if(code == e)
								break;
							szText[chcount]=code;
							chcount++;
							if(chcount==64) {
								return 0;

							}
						}
						
					}
					return 1;
				}
			}
		}
	}

}

////////////////////////////////////
/// Set parameters for the cursor color
////////////////////////////////////
void Console_textcolor(char port,char attr,char fg, char bg)
{
	
	sprintf(szText,(FARROM*)"\x1b[%d;%d;%dm",attr, fg + 30, bg + 40);
	Serial_Send_String(port,szText,SEND_NOW);
}

////////////////////////////////////
/// Set color attribute
////////////////////////////////////
void Console_Attribute(char port,char attr)
{
	
	sprintf(szText,(FARROM*)"\x1b[%dm",attr);
	Serial_Send_String(port,szText,SEND_NOW);
}
