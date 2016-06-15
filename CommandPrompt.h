#ifndef COMMANDPROMPT_H_
#define COMMANDPROMPT_H_


//  This section defines total commands and the maximum line length
//  The total size of the array can not be larger than a single bank, 256 bytes
//  Each pointer consumes 24 bits.  One definition uses 9 bytes since everything is
//  a pointer to a ROM location

#define COMMAND_PROMPT_MAX_COMMANDS		  10				// This  defines to total commands allowed for command prompt
#define COMMAND_PROMPT_MAX_COMMAND_LINE_LENGTH	64		// maximum length of a command line with arguments
#define COMMAND_PROMPT_MAX_SETS		16					// Maximum programmically set variables.  Variables must be global.
extern char Command_Serial_Port;
typedef struct  {
	void (*function)(char *arg);
	FARROM *szrCommand;
	FARROM *szrHelp;
} _COMMANDS; 

typedef struct {
	FARROM *szrName;
	ram void *Variable;
	char Type;						// Type is the actual C type for the variable
									// 0 - Char
									// 1 - String
									// 2 - int
								    // 3 - float
									// 4 - long
} _SETS;

#define SET_CHAR 0
#define SET_STRING 1
#define SET_INT 2
#define SET_FLOAT 3
#define SET_LONG 4

extern _SETS CommandPrompt_Sets[COMMAND_PROMPT_MAX_SETS];	
extern _COMMANDS CommandPrompt_Commands[COMMAND_PROMPT_MAX_COMMANDS];

extern void CommandPrompt_Init(char port);
extern void CommandPrompt_DeInit(void);
extern void CommandPrompt_AddCommand(FARROM *szrName,FARROM *szrHelp, void (*pFunc)(char *arg) );
extern void CommandPrompt_DelCommand(FARROM *szrName);
extern void CommandPrompt_TakeCommands(void);
extern int CommandPrompt_IsValidChar(char c);

extern void CommandPrompt_Test(char *arg);
extern void CommandPrompt_Set(char *arg);
extern void CommandPrompt_Save(char *arg);
extern void CommandPrompt_Reboot(char *arg);
extern void CommandPrompt_Zero(char *arg);
void Pause(void);

extern void CommandPrompt_Set_Add(FARROM *szrName,void *variable, char type);
extern void CommandPrompt_Set_Delete(FARROM *szrName);


#endif /*COMMANDPROMPT_H_*/
