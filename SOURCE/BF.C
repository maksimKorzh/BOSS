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
;                              Brainfuck interpreter                              ;
;                                                                                 ;
;---------------------------------------------------------------------------------;
\*********************************************************************************/

#include "BOSS.H"

#define MEMORY_SIZE 65536

char memory[MEMORY_SIZE];
char *src;
char *mem;
int loop;

void ExecuteBrainfuck()
{
    memset(&memory[0], 0, sizeof(memory));

    src = source_buffer;
    mem = memory;

    memory[0] = -127;
    
    while(*src)
    {
        switch(*src)
        {
            case '>':
                *mem++;
                
                if((mem - memory) > 65535)
                {
                    textcolor(YELLOW);
                    cprintf("\r\n Warning: cell #%d is out of right bound,\r\n", mem - memory);
                    mem = memory;
                    cprintf("          cell ID has been set to '%d'\r\n", mem - memory);
                    textcolor(LIGHTGRAY);
                    break;
                }
                
                break;
                
            case '<':
                *mem--;
                
                if((mem - memory) < 0)
                {
                    textcolor(YELLOW);
                    cprintf("\r\n Warning: cell #%d is out of left bound,\r\n", mem - memory);
                    mem = memory + 65535;
                    cprintf("          cell ID has been set to '%d'\r\n", mem - memory);
                    textcolor(LIGHTGRAY);
                    break;
                }
                
                break;
                
            case '+':
                if(*mem == 127)
                {
                    textcolor(YELLOW);
                    cprintf("\r\n Warning: cell #%d value '%d' is out of positive range,\r\n", mem - memory, *mem + 1);
                    *mem = -128;
                    cprintf("          cell #%d value has been set to '%d'\r\n", mem - memory, *mem );
                    textcolor(LIGHTGRAY);
                    break;
                }

                ++*mem;
                break;
                
            case '-':
                if(*mem == -128)
                {
                    textcolor(YELLOW);
                    cprintf("\r\n Warning: cell #%d value '%d' is out of negative range,\r\n", mem - memory, *mem - 1);
                    *mem = 127;
                    cprintf("          cell #%d value has been set to '%d'\r\n", mem - memory, *mem );
                    textcolor(LIGHTGRAY);
                    break;
                }
                
                --*mem;
                break;
                
            case '.': putch(*mem); break;
            case ',': *mem = getch(); break;
                
            case '[':

                if(!*mem)
                {
                    loop = 1;
                    while(loop)
                    {
                        *src++;

                        if(*src == '[') loop++;
                        if(*src == ']') loop--;
                    }
                }

                break;

            case ']':

                loop = 1;
                while(loop)
                {
                    *src--;
                   
                    if(*src == '[') loop--;
                    if(*src == ']') loop++;
                }

                *src--;
                break;

            case '#':
                textcolor(GREEN); 
                cprintf("\r\n[cell #%d: %d]\r\n", mem - memory, *mem);
                textcolor(LIGHTGRAY);
                break;
        }
        
        *src++;
    }
}