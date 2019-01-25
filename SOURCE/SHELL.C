/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                                                                 ;
;                                      BOSS                                       ;
;                                                                                 ;
;---------------------------------------------------------------------------------;
;---------------------------------------------------------------------------------;
;                                                                                 ;
;                           Bare Operating Shell System                           ;
;                                                                                 ;
;---------------------------------------------------------------------------------;
;---------------------------------------------------------------------------------;
;                                                                                 ;
;                                by Maksim Korzh                                  ;
;                                                                                 ;
;---------------------------------------------------------------------------------;
;*********************************************************************************;
;---------------------------------------------------------------------------------;
;                                                                                 ;
;       THIS PROGRAM IS FREE SOFTWARE. IT COMES WITHOUT ANY WARRANTY, TO          ;
;        THE EXTENT PERMITTED BY APPLICABLE LAW. YOU CAN REDISTRIBUTE IT          ;
;       AND/OR MODIFY IT UNDER THE TERMS OF THE DO WHAT THE FUCK YOU WANT         ;
;          TO PUBLIC LICENCE, VERSION 2, AS PUBLISHED BY SAM HOCEVAR.             ;
;                SEE http://www.wtfpl.net/ FOR MORE DETAILS.                      ;
;                                                                                 ;
;---------------------------------------------------------------------------------;
;*********************************************************************************;
;---------------------------------------------------------------------------------;
;                                                                                 ;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;                           Version 2, December 2004                              ;
;                                                                                 ;
;        Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>                         ;
;                                                                                 ;
;        Everyone is permitted to copy and distribute verbatim or modified        ;
;        copies of this license document, and changing it is allowed as long      ;
;        as the name is changed.                                                  ;
;                                                                                 ;
;                   DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE                   ;
;          TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION        ;
;                                                                                 ;
;         0. You just DO WHAT THE FUCK YOU WANT TO.                               ;
;                                                                                 ;
;---------------------------------------------------------------------------------;
;---------------------------------------------------------------------------------;
;             please let me know if you find this code/project useful             ;
;---------------------------------------------------------------------------------;
;*********************************************************************************;
;---------------------------------------------------------------------------------;
;                                                                                 ;
;         Copyright © 2019  Maksim Korzh  <freesoft.for.people@gmail.com>         ;
;                                                                                 ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

#include "BOSS.H"

char source_buffer[SOURCE_SIZE];
char logo_buffer[INFO_SIZE];
char info_buffer[INFO_SIZE];
char user_command[COMMAND_SIZE] = {COMMAND_SIZE};

void LoadFile(char *filename, char *buffer, int file_size)
{
    memset(&info_buffer[0], 0, sizeof(info_buffer));
    FILE *file = fopen(filename, "r");
    fread(buffer, 1, file_size, file);
    fclose(file);
}

void SaveFile(char *filename, char *buffer, int file_size)
{
    FILE *file = fopen(filename, "w");
    fwrite(buffer, 1, file_size, file);
    fclose(file);
}

void PickColor(int color, int *color_lock)
{
    if(!*color_lock)
    {
	textcolor(color);
	*color_lock ^= 1;
    }

    else if(*color_lock)
    {
	textcolor(LIGHTGRAY);
	*color_lock ^= 1;
    }
}

void PrintFile(char *file_buffer, int color_1, int color_2, int color_3)
{
    int color_lock = 0;

    while(*file_buffer)
    {
    	switch(*file_buffer)
    	{
    	    case '$':
    		PickColor(color_1, &color_lock);
    		file_buffer++;
    		break;
    
    	    case '&':
    		PickColor(color_2, &color_lock);
    		file_buffer++;
    		break;
    
    	    case '#':
    		PickColor(color_3, &color_lock);
    		file_buffer++;
    		break;
    	}
    
    	if(*file_buffer == '\n')
    	    cputs("\r\n");
    
    	else if(*file_buffer != '\n')
    	    putch(*file_buffer);
    
    	file_buffer++;
    }
}

void RefreshScreen()
{
    clrscr();
    _setcursortype(_NOCURSOR);
    PrintFile(logo_buffer, DARKGRAY, BROWN, MAGENTA);
    textcolor(GREEN);
    cputs(" BOSS $ ");
    textcolor(LIGHTGRAY);
    _setcursortype(_SOLIDCURSOR);
}

int ProcessCommand()
{
    memset(&user_command[1], 0, sizeof(user_command));
    cgets(user_command);

    if(!user_command[2]) return 1;

    else if(!strncmp(user_command + 2, "new", 3))
    {
        memset(&source_buffer[0], 0, sizeof(source_buffer));
        gotoxy(9, 24);
        cputs("Source buffer has been cleared. Press any key to continue...");
    }

	else if(!strncmp(user_command + 2, "edit", 4))
    {
        clrscr();
        ViewSource();
        EditSource();
        cputs("\n\n\r           Source buffer has been saved. Press any key to continue...\r\n");
        _setcursortype(_NOCURSOR);
    }

	else if(!strncmp(user_command + 2, "save", 4))
    {
        SaveFile("SYSTEM/SOURCE.BF", source_buffer, SOURCE_SIZE);
        gotoxy(9, 24);
        cputs("Source buffer has been written to disk. Press any key to continue...");
    }

	else if(!strncmp(user_command + 2, "load", 4))
    {
        memset(&source_buffer[0], 0, sizeof(source_buffer));
        LoadFile("SYSTEM/SOURCE.BF", source_buffer, SOURCE_SIZE);
        gotoxy(9, 24);
        cputs("Source buffer has been loaded. Press any key to continue...");
    }

	else if(!strncmp(user_command + 2, "view", 4))
    {
        clrscr();
        ViewSource();
        cputs("\n\n\r              Source buffer content. Press any key to continue...\r\n");    
        _setcursortype(_NOCURSOR);
    }

	else if(!strncmp(user_command + 2, "exit", 4))
    {
        return 0;
    }

	else if(!strncmp(user_command + 2, "run", 3))
    {
        
    }

	else if(!strncmp(user_command + 2, "help", 4))
    {
        clrscr();
        LoadFile("SYSTEM/HELP.TXT", info_buffer, INFO_SIZE);
        PrintFile(info_buffer, DARKGRAY, BROWN, MAGENTA);
        _setcursortype(_NOCURSOR);
    }

    else if(!strncmp(user_command + 2, "copy", 4))
    {
        clrscr();
        LoadFile("SYSTEM/COPYING.TXT", info_buffer, INFO_SIZE);
        PrintFile(info_buffer, DARKGRAY, BROWN, GREEN);
        _setcursortype(_NOCURSOR);
    }

	else if(!strncmp(user_command + 2, "boss", 4))
    {
        clrscr();
        LoadFile("SYSTEM/BOSS.TXT", info_buffer, INFO_SIZE);
        PrintFile(info_buffer, DARKGRAY, BROWN, MAGENTA);
        _setcursortype(_NOCURSOR);
    }

	else if(!strncmp(user_command + 2, "wiki", 4))
    {
        clrscr();
        LoadFile("SYSTEM/WIKI.TXT", info_buffer, INFO_SIZE);
        PrintFile(info_buffer, DARKGRAY, BROWN, MAGENTA);
        _setcursortype(_NOCURSOR);        
    }

	else if(!strncmp(user_command + 2, "code", 4))
    {
        clrscr();
        LoadFile("SYSTEM/CODE.TXT", info_buffer, INFO_SIZE);
        PrintFile(info_buffer, DARKGRAY, BROWN, MAGENTA);
        _setcursortype(_NOCURSOR);                
    }

	else if(!strncmp(user_command + 2, "ascii", 5))
    {
        clrscr();
        LoadFile("SYSTEM/ASCII.TXT", info_buffer, INFO_SIZE);
        PrintFile(info_buffer, DARKGRAY, BROWN, DARKGRAY);
        _setcursortype(_NOCURSOR);        
    }

    else
    {
        gotoxy(9, 24);
        printf("No such command!");
    }

    return 1;
}

void ShellLoop()
{
    LoadFile("SYSTEM/LOGO.TXT", logo_buffer, INFO_SIZE);
        
    while(1)
    {
        RefreshScreen();
        
        if(!ProcessCommand())
            break;

        //printf("test %d %d %d\r\n", user_command[0], user_command[1], user_command[2]);
        getch();
    }

    clrscr();
}