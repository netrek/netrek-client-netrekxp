/******************************************************************************/
/***  File:  console.c                                                      ***/
/***                                                                        ***/
/***  Function: set of functions to manupulate text output to console       ***/
/***            window                                                      ***/
/******************************************************************************/

#include <stdio.h>
#include <signal.h>
#include <sys/types.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

/******************************************************************************/
/***  IsConsoleVisible ()                                                   ***/
/******************************************************************************/
int
IsConsoleVisible (void)
{
    char ConsoleTitle[256];
    return GetConsoleTitle (ConsoleTitle, sizeof (ConsoleTitle));
}

/******************************************************************************/
/***  ShowConsole ()                                                        ***/
/******************************************************************************/
void
ShowConsole (void)
{
    if (IsConsoleVisible ())
        return;

    /* If the console is not visible it means that we freed it, so we'll have
       to allocate it once again and write everything there */
    AllocConsole ();

    BufferToConsole ();
}

/******************************************************************************/
/***  LineToConsole ()                                                      ***/
/******************************************************************************/
void
LineToConsole (char * format, ...)
{
    struct cons_buffer * tmp;     // temporary pointer to our linked list;
    char str[1024];
    DWORD numChars;
    va_list args;

    va_start (args, format);
    vsprintf (str, format, args);
    va_end (args);
    
    /* First of all let's put the string into our linked list */
    tmp = consTail;
    if (tmp == NULL)
    {
        consHead = (struct cons_buffer *) malloc (sizeof (struct cons_buffer));
        consTail = consHead;
        consHead->string = (char *) malloc (sizeof (char) * strlen (str) + 1);
        consHead->next = NULL;

        strcpy (consHead->string, str);
    }
    else
    {
        tmp = (struct cons_buffer *) malloc (sizeof (struct cons_buffer));
        consTail = consTail->next;
        consTail = tmp;
        consTail->string = (char *) malloc (sizeof (char) * strlen (str) + 1);
        consTail->next = NULL;

        strcpy (consTail->string, str);
    }

    /* Now if console is visible we can also output the string to the console */
    if (IsConsoleVisible ())
        WriteConsole (GetStdHandle (STD_OUTPUT_HANDLE), str, strlen (str), &numChars, NULL);
}

/******************************************************************************/
/***  BufferToConsole ()                                                    ***/
/******************************************************************************/
void
BufferToConsole (void)
{
    struct cons_buffer * tmp;
    DWORD numChars;

    if (!IsConsoleVisible ())
        return;

    tmp = consHead;

    while (tmp)
    {
        WriteConsole (GetStdHandle (STD_OUTPUT_HANDLE), tmp->string, strlen (tmp->string), 
                      &numChars, NULL);
        tmp = tmp->next;
    }
}