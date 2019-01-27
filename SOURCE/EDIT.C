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

/*********************************************************************************\
;---------------------------------------------------------------------------------;
;                                                                                 ;
;                                 Brainfuck shell                                 ;
;                                                                                 ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

#include "BOSS.H"

char *ScrollLine(char *src, int cur_x, int cur_y)
{
    char *start_line = src;

    int key = 0;
    
    while(key != 13)
    {
        key = getch();

        switch(key)
        {   
            case 'H':
                while(start_line != source_buffer)
                {
                    start_line--;                
                    if(*start_line == '\n') break;
                }

                clreol();
                ViewSource(start_line, EDIT);
                gotoxy(cur_x, cur_y);
                
                break;

            case 'P':
                while(*start_line)
                {
                    start_line++;
                    if(*start_line == '\n') break;
                }

                clreol();
                ViewSource(start_line, EDIT);
                gotoxy(cur_x, cur_y);
                
                break;
        }
    }

    return start_line + 1;
}

void ViewSource(char *src, int mode)
{
    cputs("");
    textcolor(DARKGRAY);

    int line_lock = 0;

    while(*src)
    {
        switch(*src)
        {
            case '>':
            case '<':
                textcolor(LIGHTGREEN);
                break;
                
            case '+':
            case '-':
                textcolor(LIGHTRED);
                break;

            case '.':
            case ',':
                textcolor(YELLOW);
                break;

            case '[':
            case ']':
                textcolor(LIGHTMAGENTA);
                break;

            case '#':
            case '!':
                textcolor(LIGHTBLUE);
                break;
        }

     	if(*src == '\n')
     	{
     	    if(mode == VIEW)
     	    {
     	        int cur_y = wherey();

     	        if(cur_y < 22)
     	        {
    	            cputs("\r\n");
                    
                }
                
    	        else
    	        {
    	            textcolor(LIGHTGRAY);
                    cputs("\n\n\r            Source buffer content. Press any key to see next page...\r\n");
                    textcolor(DARKGRAY);
                    _setcursortype(_NOCURSOR);
    	            getch();
                    clrscr();
    	        }
    	    }

    	    else if(mode == EDIT)
    	    {
    	        if(!line_lock)
    	            line_lock ^= 1;
    	        
    	        else if(line_lock == 1)
    	        {
    	            cputs("\r\n");
    	            return;
    	        }
    	    }
    	        
    	}
    
    	else if(*src != '\n')
    	{
    	    putch(*src);
    	    textcolor(DARKGRAY);
    	}

        src++;
    }

    textcolor(LIGHTGRAY);
}

void EditSource()
{
    int char_count = 0;
    char *src = source_buffer;

    while(*src) src++;
    
    int key = 0;
    int cursor_X = 0;
    int cursor_Y = 0;

    textcolor(GREEN);
    cputs("\r\n $ ");

    while(key != 27)
    {

        textcolor(DARKGRAY);
        key = getch();

        switch(key)
        {    
            case 27:
                textcolor(LIGHTGRAY);
                return;

            case '>':
            case '<':
                textcolor(LIGHTGREEN);
                break;
                
            case '+':
            case '-':
                textcolor(LIGHTRED);
                break;

            case '.':
            case ',':
                textcolor(YELLOW);
                break;

            case '[':
            case ']':
                textcolor(LIGHTMAGENTA);
                break;

            case '#':
            case '!':
                textcolor(LIGHTBLUE);
                break;

            case '\b':
                cursor_X = wherex();

                if(cursor_X > 4)
                {
                    src--;
                    cputs("\b \b");
                }

                textcolor(DARKGRAY);
                break;
            
            case '\r':
                while(*src) src++;
                while(wherex() < 79)
                {
                    *src = ' ';
                    putch(*src);
                    src++;
                }

                *src = '\n';
                textcolor(GREEN);
                cputs("\r\n $ ");
                textcolor(LIGHTGRAY);
                clreol();
                src++;
                break;

            case 'P':
            case 'H':
                _setcursortype(_NOCURSOR);
                src = ScrollLine(src, wherex(), wherey());
                _setcursortype(_SOLIDCURSOR);
                gotoxy(4, wherey());
                break;

            case 'M':
                if(wherex() < 79)
                {
                    gotoxy(wherex() + 1, wherey());

                    if(!*src)
                    {
                        *src = ' ';
                        src++;
                    }
                    
                    else
                        src++;
                }
                   
                break;

            case 'K':
                if(wherex() > 4)
                {
                    gotoxy(wherex() - 1, wherey());
                    src--;
                }

                break;
        }

        if(key == 'H') continue;

        else if(key == 'K') continue;
        else if(key == 'M') continue;
        else if(key == 'P') continue;        

        if(!(char)key) continue;

        if(key != '\r' && wherex() < 79)
        {
            (key == '\b') ? (*src-- = 0) : (*src = (char)key);
            
            if(*src)
            {            
                if(key != '\b')
                    putch(*src);
                    
                textcolor(DARKGRAY);
            }
            
            src++;
        }
    }
}