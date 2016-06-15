/*! \file commandprompt.c
    \brief Provides command prompt interface

	These functions provide a command line interface
	for updating variables and executing functions.
*/

#include "pic.h"
#include "commandprompt.h"
#include "serial.h"


#pragma udata commands1			// Put the next variable in to its own section
_SETS CommandPrompt_Sets[COMMAND_PROMPT_MAX_SETS];	

#pragma udata commands2			// Put the next variable in to its own section
_COMMANDS CommandPrompt_Commands[COMMAND_PROMPT_MAX_COMMANDS];

#pragma udata commands3			// Put the next variable in to its own section
char szCurrentCommand[COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH];
char szLastCommand[COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH];

#pragma udata					// restore section naming

 
FARROM CommandPrompt_szValidCharacters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890 =._";
char Command_Serial_Port;




void CommandPrompt_AddCommand(FARROM *szrName,FARROM *szrHelp, void (*pFunc)(char *arg) )
{
	char t;
	for(t=0;t<COMMAND_PROMPT_MAX_COMMANDS;t++)
	{
		if(CommandPrompt_Commands[t].function==0)
		{
			CommandPrompt_Commands[t].function=pFunc;
			CommandPrompt_Commands[t].szrCommand=szrName;
			CommandPrompt_Commands[t].szrHelp=szrHelp;
			return;
		}
	}
	return;
}
void CommandPrompt_Set_Add(FARROM *szrName,void *variable, char type)
{
	char t;
	for(t=0;t<COMMAND_PROMPT_MAX_SETS;t++)
	{
		if(CommandPrompt_Sets[t].szrName==0)
		{
			CommandPrompt_Sets[t].szrName=szrName;
 			CommandPrompt_Sets[t].Variable=variable;
			CommandPrompt_Sets[t].Type=type;
			return;
		}
	}
	return;
}
void CommandPrompt_DelCommand(FARROM *szrName)
{
	char t;

	for(t=0;t<COMMAND_PROMPT_MAX_COMMANDS;t++)
	{
		if(strcmppgm(CommandPrompt_Commands[t].szrCommand,szrName) == 0)
		{
			CommandPrompt_Commands[t].function=0;
			return;
		}
	}
	return;
}
void CommandPrompt_Set_Delete(FARROM *szrName)
{
	char t;

	for(t=0;t<COMMAND_PROMPT_MAX_SETS;t++)
	{
		if(strcmppgm(CommandPrompt_Sets[t].szrName,szrName) == 0)
		{
			CommandPrompt_Sets[t].szrName=0;
			return;
		}
	}
	return;
}

void CommandPrompt_Init(char port)
{
	char t;
	Command_Serial_Port=port;
	for(t=0;t<COMMAND_PROMPT_MAX_SETS;t++)
	{
		CommandPrompt_Sets[t].szrName = 0;
	}
	for(t=0;t<COMMAND_PROMPT_MAX_COMMANDS;t++)
	{
			CommandPrompt_Commands[t].function=0;
	}
	return;
}

void CommandPrompt_TakeCommands()
{
	
	char c;						// Received character
	char ESC=27;				// Escape Character
	char code;					// Escape sequence code
	char iCursorPosition=0;		// Position of cursor during line editing
	char iFirstSpace;			// location of the first space in the line
	char iTotalChars=0;			// Line character count

	char t;

	// Clear the buffers
	memset(szCurrentCommand,0,COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH);
	memset(szLastCommand,0,COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH);
	
	
	// Send a CRLF to make sure we start on a new line;
	sprintf(szText,(FARROM*)"\r\nModule Command Mode\r\n#");
	Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);

	while (Serial_RX_Data_Waiting(Command_Serial_Port))
		Serial_GetChar(Command_Serial_Port);

	while (1)			// Continuous loop;
	{
		////////////////////////////////////////////////////
		//  Blink the status LED
		////////////////////////////////////////////////////
		if(Status_LED)
			Status_LED=0;
		else
			Status_LED=1;

		// Check for data waiting and if so proceed
		if( Serial_RX_Data_Waiting(Command_Serial_Port) )
		{

			// Get the character and capitalize it
			c=Serial_GetChar(Command_Serial_Port);

			/////////////////////////////////////////////////
			// Escape Sequences, used by terminals to move the cursor
			/////////////////////////////////////////////////
			if(c==27)		// Sequence start byte
			{
				while (!Serial_RX_Data_Waiting(Command_Serial_Port)) {}
					code=Serial_GetChar(Command_Serial_Port);

				if(code=='[')  // This means we have a sequence
				{
					//       Reset  Set
					//up     EscA   EscOA
					//down   EscB   EscOB
					//right  EscC   EscOC
					//left   EscD   EscOD
					//code=sgetcar(0);
					while (!Serial_RX_Data_Waiting(Command_Serial_Port)) {}
						code=Serial_GetChar(Command_Serial_Port);

 					if(code=='O')			// Ignore next byte if code equals O
	 					Serial_GetChar(Command_Serial_Port);
 					if(code=='A')				// Up Arrow - use last command
 					{
						// Copy previous command to current command buffer
						strcpy(szCurrentCommand,szLastCommand);

						// Display last command
						Serial_PutChar(Command_Serial_Port,13);
						Serial_Send_String(Command_Serial_Port,szCurrentCommand,SEND_NOW);
						// Put cursor at the end of the command
						iCursorPosition=strlen(szCurrentCommand);
						// Get command length
						iTotalChars=strlen(szCurrentCommand);

					}
 					if(code=='B')				// Down Arrow - use blank line
 					{
						memset(szCurrentCommand,0,COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH);

						Serial_PutChar(Command_Serial_Port,13);
						Serial_PutChar(Command_Serial_Port,'#');
						iCursorPosition=0;
						iTotalChars=0;
 					}
 					if(code=='D')				// Move Left
 					{
	 					if(iCursorPosition)
 						{
							iCursorPosition--;
							Serial_PutChar(Command_Serial_Port,27);
							Serial_PutChar(Command_Serial_Port,'[');
							Serial_PutChar(Command_Serial_Port,'D');

						}
 					}
 					if(code=='C')				// Move Left
 					{
	 					if(iCursorPosition<iTotalChars)
 						{
							iCursorPosition++;
							Serial_PutChar(Command_Serial_Port,27);
							Serial_PutChar(Command_Serial_Port,'[');
							Serial_PutChar(Command_Serial_Port,'C');

						}
 					}
				}
			}
			/////////////////////////////////////////////////
			// Backspace
			/////////////////////////////////////////////////
			if(c==8 || c==127)		// Backspace
			{
				if(iTotalChars)
				{
					if(iCursorPosition)
					{

						// Shift all characters after the cursor back one
						for(t=iCursorPosition;t<COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH;t++)
							szCurrentCommand[t-1]=szCurrentCommand[t];

						// move to the beginning of the line on the screen and print the line again
						sprintf(szText,"\r#%s ",szCurrentCommand);
						Serial_PutChar(Command_Serial_Port,13);
						Serial_Send_String(Command_Serial_Port,szCurrentCommand,SEND_NOW);

						iCursorPosition--;
						iTotalChars--;

						// Move cursor to new location
						sprintf(szText,"\r%c[%dC",ESC,iCursorPosition+1);
						Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);


					}
				}

			}
			/////////////////////////////////////////////////
			// Enter (Carriage Return)
			/////////////////////////////////////////////////
			if(c==13 || c==10)	// now we need to process the command
			{
				if(iTotalChars==0)
				{
					// Command is blank so we need to send another blank line and ignore
					memset(szCurrentCommand,0,COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH);
					iTotalChars=0;
					iCursorPosition=0;
					sprintf(szText,"\r\n#");
					Serial_PutCRLF(Command_Serial_Port);
					Serial_PutChar(Command_Serial_Port,'#');
				} else {

					// find the first space so we know where the arguments begin
					iFirstSpace=0;
					for(t=0;t<COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH;t++)
					{
						if( (szCurrentCommand[t]==32) || (szCurrentCommand[t]==0) )
							if(iFirstSpace==0)
								iFirstSpace=t;
					}

					// To save memory we will replace the first space with null so the string
					// commands think it is the end of the string
					szCurrentCommand[iFirstSpace] = 0;

					if(strcmppgm2ram(szCurrentCommand,(FARROM*)"EXIT")==0)
					{
						Serial_PutCRLF(Command_Serial_Port);
						Serial_PutCRLF(Command_Serial_Port);
						return;

					////////////////////////////////////////////////
					// SET Values
					/////////////////////////////////////////////////
					} else if(strcmppgm2ram(szCurrentCommand,(FARROM*)"SET")==0) {
						CommandPrompt_Set(&szCurrentCommand[0]+iFirstSpace+1);

						memset(szCurrentCommand,0,COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH);
						iTotalChars=0;
						iCursorPosition=0;
						Serial_PutCRLF(Command_Serial_Port);
						Serial_PutChar(Command_Serial_Port,'#');

					/////////////////////////////////////////////////
					// Help
					/////////////////////////////////////////////////
					} else if(strcmppgm2ram(szCurrentCommand,(FARROM*)"HELP")==0) {
						sprintf(szText,(FARROM*)"\r\nAvailable Commands\r\n");
						Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);
						for(t=0;t<COMMAND_PROMPT_MAX_COMMANDS;t++)
						{
							if(	CommandPrompt_Commands[t].function )
							{
						 		sprintf(szText,(FARROM*)"%HS %HS\r\n",CommandPrompt_Commands[t].szrCommand,CommandPrompt_Commands[t].szrHelp);
								Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);
							}
						}
						memset(szCurrentCommand,0,COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH);
						iTotalChars=0;
						iCursorPosition=0;
						Serial_PutCRLF(Command_Serial_Port);
						Serial_PutChar(Command_Serial_Port,'#');

					/////////////////////////////////////////////////
					// Check against commands
					/////////////////////////////////////////////////
					} else {

						for(t=0;t<COMMAND_PROMPT_MAX_COMMANDS;t++)
						{
							if(	CommandPrompt_Commands[t].function)
							{
								if(strcmppgm2ram(szCurrentCommand,CommandPrompt_Commands[t].szrCommand)==0)
								{
									CommandPrompt_Commands[t].function(&szCurrentCommand[0]+iFirstSpace+1);

								}
							}
						}
						memset(szCurrentCommand,0,COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH);
						iTotalChars=0;
						iCursorPosition=0;
						Serial_PutCRLF(Command_Serial_Port);
						Serial_PutChar(Command_Serial_Port,'#');
					}

				}
			}
			if(CommandPrompt_IsValidChar(c) && (iTotalChars<(COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH-1)))
			{
				if(iTotalChars==iCursorPosition) {
					szCurrentCommand[iTotalChars]=c;
					iTotalChars++;
					iCursorPosition++;
					Serial_PutChar(Command_Serial_Port,c);
				} else {
					for(t=(COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH-2);t>iCursorPosition;t--)
					{
						szCurrentCommand[t]=szCurrentCommand[t-1];
					}
					szCurrentCommand[iCursorPosition]=c;
					iTotalChars++;
					iCursorPosition++;

					sprintf(szText,"\r#%s",szCurrentCommand);
					Serial_PutChar(Command_Serial_Port,13);
					Serial_Send_String(Command_Serial_Port,szCurrentCommand,SEND_NOW);

					sprintf(szText,"\r%c[%dC",ESC,iCursorPosition+1);
					Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);

				}
			}
			Serial_Send_Full_TX_Buffer(Command_Serial_Port);

		}
		Serial_Send_Full_TX_Buffer(Command_Serial_Port);

	}
	Serial_Send_Full_TX_Buffer(Command_Serial_Port);

}



void CommandPrompt_DeInit()
{
}


int CommandPrompt_IsValidChar(char c)
{
	char t;
	for(t=0;t < strlenpgm(CommandPrompt_szValidCharacters);t++)
	{
		if( CommandPrompt_szValidCharacters[t]==c)
			return 1;
	}
	return 0;
}


void CommandPrompt_Test(char *arg)
{
	
	sprintf(szText,"\r\nCommand=TEST\r\nARGS=%s\r\n",arg);
	Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);
}

void CommandPrompt_Save(char *arg)
{

}
void CommandPrompt_Zero(char *arg)
{

}
void CommandPrompt_Reboot(char *arg)
{

	sprintf(szText,"\r\nRebooting\r\n");
	Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);

//	OSTimeDly(10000);
//	ForceReboot();
}
void Pause(void)
{

	sprintf(szText,"\r\nPress any key to continue.\r\n");
	Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);
	do {

		if(Serial_RX_Data_Waiting(Command_Serial_Port))
			break;
	} while( 1==1);

   	Serial_GetChar(Command_Serial_Port);


}

void CommandPrompt_Set(char *arg)
{

	char t;
	char *value;
	char type; 						// 0 - Char
									// 1 - String
									// 2 - int
								    // 3 - float
									// 4 - long
	long lWhole;					// used for floating point conversion
	unsigned long ulDecimal;		// used for floating point conversion
	float fTemp;

	// Find the equals sign
	int iEquals=0;
	for(t=0;t<(signed int)strlen(arg);t++)
	{
		if(arg[t]=='=')
			iEquals=t;
	}
	sprintf(szText,"\r\n");
	Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);

	if(iEquals==0)
	{
		for(t=0;t<COMMAND_PROMPT_MAX_SETS;t++)
		{
			if(CommandPrompt_Sets[t].szrName!=0)
			{
				type=CommandPrompt_Sets[t].Type;
			
				if(type==SET_CHAR)
					sprintf(szText,"%HS=%c\r\n",CommandPrompt_Sets[t].szrName,*(char *)CommandPrompt_Sets[t].Variable);
				if(type==SET_STRING)
					sprintf(szText,"%HS=%s\r\n",CommandPrompt_Sets[t].szrName,CommandPrompt_Sets[t].Variable);
				if(type==SET_INT)
					sprintf(szText,"%HS=%d\r\n",CommandPrompt_Sets[t].szrName,*(int*)CommandPrompt_Sets[t].Variable);
				if(type==SET_FLOAT) {
					fTemp = *(float*)CommandPrompt_Sets[t].Variable;
					lWhole=(long)fTemp;
					fTemp = fTemp * 1000;
 					ulDecimal=(long)fTemp;
					ulDecimal-=lWhole*1000;
					sprintf(szText,"%HS=%ld.%ld\r\n",CommandPrompt_Sets[t].szrName,lWhole,ulDecimal);
				}
				if(type==SET_LONG) 
					sprintf(szText,"%HS=%ld\r\n",CommandPrompt_Sets[t].szrName,(long *)CommandPrompt_Sets[t].Variable);
				Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);

			}
		}

	} else {
		arg[iEquals]=0;	// Null the space so it acts like an end of string to seperate the strings
		
		value = arg;	// Make the value pointer equal to the arguments
		value += iEquals + 1;		// Increment to the pointer to the next spot after the = sign
		//Copy from the next char after the equals sign.


		for(t=0;t<COMMAND_PROMPT_MAX_SETS;t++)
		{
			if( strcmppgm2ram(arg,CommandPrompt_Sets[t].szrName) == 0 ) 
			{
				type=CommandPrompt_Sets[t].Type;
			
				if(type==SET_CHAR) {
					*(char *)CommandPrompt_Sets[t].Variable=*value;
				}
				if(type==SET_STRING)
					strcpy(CommandPrompt_Sets[t].Variable,value);
				if(type==SET_INT)
					*(int*)CommandPrompt_Sets[t].Variable=atoi(value);
				if(type==SET_FLOAT) 
					*(float*)CommandPrompt_Sets[t].Variable=atof(value);
				if(type==SET_LONG) 
					*(int*)CommandPrompt_Sets[t].Variable=atol(value);


				if(type==SET_CHAR)
					sprintf(szText,"%HS=%c\r\n",CommandPrompt_Sets[t].szrName,*(char *)CommandPrompt_Sets[t].Variable);
				if(type==SET_STRING)
					sprintf(szText,"%HS=%s\r\n",CommandPrompt_Sets[t].szrName,CommandPrompt_Sets[t].Variable);
				if(type==SET_INT)
					sprintf(szText,"%HS=%d\r\n",CommandPrompt_Sets[t].szrName,*(int*)CommandPrompt_Sets[t].Variable);
				if(type==SET_FLOAT) {
					fTemp = *(float*)CommandPrompt_Sets[t].Variable;
					lWhole=(long)fTemp;
					fTemp = fTemp * 1000;
 					ulDecimal=(long)fTemp;
					ulDecimal-=lWhole*1000;
					sprintf(szText,"%HS=%ld.%ld\r\n",CommandPrompt_Sets[t].szrName,lWhole,ulDecimal);
				}
				if(type==SET_LONG) 
					sprintf(szText,"%HS=%ld\r\n",CommandPrompt_Sets[t].szrName,(long *)CommandPrompt_Sets[t].Variable);
				Serial_Send_String(Command_Serial_Port,szText,SEND_NOW);
				
			}
		}
	}
}
