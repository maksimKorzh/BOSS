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

void ViewSource()
{
    char *src = source_buffer;

    textcolor(LIGHTGRAY);

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
    	    cputs("\r\n");
    
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

    cputs("\r\n brainfuck $ ");

    while(key != 27)
    {
        textcolor(DARKGRAY);
        key = getch();

        switch(key)
        {
            case 27:
                *src = '\n';
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

                if(cursor_X > 14)
                {
                    src--;
                    cputs("\b \b");
                }

                break;
            
            case '\r':
                *src = '\n';
                textcolor(LIGHTGRAY);
                cputs("\r\n brainfuck $ ");
                src++;
                break;
        }

        if(key != '\r')
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