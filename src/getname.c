/******************************************************************************/
/***  File:  getname.c                                                      ***/
/***                                                                        ***/
/***  Function:                                                             ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    Kevin P. Smith 09/28/88                                             ***/
/***    SRS 2/23/98 - Took code from SWINE which had been modifified by     ***/
/***                  Greg Chung to display screen nicely                   ***/
/***    ssheldon 8/00 - Cleaned up source code, added #include "proto.h"    ***/
/***               and function header comments                             ***/
/******************************************************************************/


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <io.h>
#include <errno.h>
#include <pwd.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "config.h"
#include "copyright2.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "proto.h"

static char tempname[16];
static char password1[16];
static char password2[16];
static int state;
static int autologin;

#define SIZEOF(a)       (sizeof (a) / sizeof (*(a)))
#define ST_GETNAME 0
#define ST_GETPASS 1
#define ST_MAKEPASS1 2
#define ST_MAKEPASS2 3
#define ST_DONE 4

extern HINSTANCE MyInstance;


/******************************************************************************/
/***  noautologin()                                                         ***/
/******************************************************************************/
static void
noautologin (void)
{
    static char *alf = "Automatic login failed";

    autologin = 0;
    *defpasswd = *password1 = *password2 = '\0';
    W_WriteText (w, 100, 130, textColor, alf, strlen (alf), W_BoldFont);

}

/******************************************************************************/
/***  showreadme()                                                          ***/
/******************************************************************************/
static void
showreadme (void)
{

    // Readme is displayed below prompt for username

    static char *README[] = {
        "",
        "Netrek XP Mod - the better netrek eXPerience!",
        "",
        "",
        "",
        "Note: This client is made by humans and errors are human.",
        "      No one takes responsibility for lost INL games etc.",
        "      Use at your own risk! If you don't like it, don't use it.",
        "",
        "Comments, suggestions, bugreports to: keyos@keyos.org",
        "",
        "Include version and architecture info in bug reports",
        "",
        "Patches to: keyos@keyos.org",
        "",
    };
    int i, length;


    for (i = 0; i < SIZEOF (README); i++)
    {
        length = strlen (README[i]);

        W_WriteText (w, 20, i * W_Textheight + 220, textColor, README[i],
                     length, W_RegularFont);
    }
}

/******************************************************************************/
/***  loaddude()                                                            ***/
/******************************************************************************/
static void
loaddude ()
    /*
     * Query dude.
     */
{
    if (strcmp (tempname, "Guest") == 0 || strcmp (tempname, "guest") == 0)
    {
        loginAccept = -1;
        sendLoginReq (tempname, "", (char *) login, 0);
        state = ST_DONE;
        me->p_stats.st_tticks = 1;      /* prevent overflow */
        strcpy (me->p_name, tempname);
        while (loginAccept == -1)
        {
            socketPause ();
            readFromServer (NULL);
            if (isServerDead ())
            {
                LineToConsole ("Server is dead!\n");
                exit (0);
            }
        }
        if (loginAccept == 0)
        {
            LineToConsole ("Hmmm... The SOB server won't let me log in as guest!\n");
            exit (0);
        }
        return;
    }
    /* Ask about the user */
    loginAccept = -1;
    sendLoginReq (tempname, "", login, 1);
    while (loginAccept == -1)
    {
        socketPause ();
        readFromServer (NULL);
        if (isServerDead ())
        {
            LineToConsole ("Server is dead!\n");
            exit (0);
        }
    }
    *password1 = *password2 = 0;
    if (loginAccept == 0)
    {
        state = ST_MAKEPASS1;
    }
    else
    {
        state = ST_GETPASS;
    }
}

/******************************************************************************/
/***  checkpassword()                                                       ***/
/******************************************************************************/
static void
checkpassword ()
    /*
     * Check dude's password. If he is ok, move to state ST_DONE.
     */
{
    char *s;

    sendLoginReq (tempname, password1, login, 0);
    loginAccept = -1;
    while (loginAccept == -1)
    {
        socketPause ();
        readFromServer (NULL);
        if (isServerDead ())
        {
            LineToConsole ("Server is dead!\n");
            exit (0);
        }
    }
    if (loginAccept == 0)
    {
        s = "Bad password!";
        W_WriteText (w, 100, 110, textColor, s, strlen (s), W_BoldFont);
        (void) W_EventsPending ();
        sleep (1);
        W_ClearWindow (w);
        *tempname = 0;
        state = ST_GETNAME;
        return;
    }
    strcpy (me->p_name, tempname);

    state = ST_DONE;
}

/******************************************************************************/
/***  try_autologin()                                                       ***/
/******************************************************************************/
static void
try_autologin (char *defname)
{
    strcpy (tempname, defname);
    loaddude ();
    if (state != ST_DONE)
    {
        strcpy (password1, defpasswd);
        checkpassword ();
    }
    if (state == ST_GETNAME)
        noautologin ();
}

/******************************************************************************/
/***  makeNewGuy()                                                          ***/
/******************************************************************************/
static void
makeNewGuy ()
    /*
     * Make the dude with name tempname and password password1. Move to state
     * ST_DONE.
     */
{
    char *s;

    if (strcmp (password1, password2) != 0)
    {
        s = "Passwords do not match";
        W_WriteText (w, 100, 160, textColor, s, strlen (s), W_BoldFont);
        (void) W_EventsPending ();
        sleep (3);
        W_ClearWindow (w);
        *tempname = 0;
        state = ST_GETNAME;
        return;
    }
    /* same routine! */
    checkpassword ();
}

/******************************************************************************/
/***  displayFormat()                                                       ***/
/******************************************************************************/
static void
displayFormat (char *defname)
{
    char s[100];
    char *t;

    t = "Enter your character name here.  Use 'guest' to use";
    W_WriteText (w, 40, 30, textColor, t, strlen (t), W_RegularFont);

    t = "a temporary name or hit ^G (CTRL-G) to autologin as guest.";
    W_WriteText (w, 40, 40, textColor, t, strlen (t), W_RegularFont);

    t = "Or, hit ^D (CTRL-D) with a blank name to exit immediately.";
    W_WriteText (w, 40, 50, textColor, t, strlen (t), W_RegularFont);

    sprintf (s, "Character name (\"%s\"): ", defname);
    W_WriteText (w, 40, 70, textColor, s, strlen (s), W_RegularFont);

    t = ".               .";
    W_WriteText (w, 40 + ((20 + strlen (defname)) * W_Textwidth), 68,
                 textColor, t, strlen (t), W_RegularFont);
}


/******************************************************************************/
/***  displayStartup()                                                      ***/
/******************************************************************************/
static void
displayStartup (char *defname)
/* Draws entry screen based upon state. */
{
    char buf[100];
    char s[100];
    register char *t;
    unsigned int i;

    displayFormat (defname);

    if (state == ST_DONE || autologin)
        return;

    sprintf (buf, "Connection established to %s", serverName);
    t = buf;
    W_WriteText (w, 100, 170, textColor, t, strlen (t), W_BoldFont);


    sprintf (s, "%-15s", tempname);
    W_WriteText (w, 40 + (21 + strlen (defname)) * W_Textwidth, 70, textColor,
                 s, strlen (s), W_BoldFont);
    t = " ^ ";
    W_WriteText (w,
                 40 + (20 + strlen (defname) +
                       strlen (tempname)) * W_Textwidth, 80, textColor, t, 3,
                 W_RegularFont);

    if (state == ST_GETPASS)
    {
        t = "Enter password: ";
        W_WriteText (w, 100, 90, textColor, t, strlen (t), W_BoldFont);
        t = ".               .";
        W_WriteText (w, 100 + ((16) * W_Textwidth), 88,
                     textColor, t, strlen (t), W_RegularFont);
        for (i = 0; i < strlen (password1); i++)
            W_WriteText (w, 100 + ((16 + 1 + i) * W_Textwidth), 90,
                        textColor, "x", 1, W_BoldFont);
        t = " ^ ";
        W_WriteText (w, 100 + (16 + strlen (password1)) * W_Textwidth,
                     100, textColor, t, 3, W_RegularFont);
    }
    if (state > ST_GETPASS)
    {
        t = "You need to make a password.";
        W_WriteText (w, 100, 90, textColor, t, strlen (t), W_BoldFont);
        t = "So think of a password you can remember, and enter it.";
        W_WriteText (w, 100, 100, textColor, t, strlen (t), W_BoldFont);
        t = "What is your password? :";
        W_WriteText (w, 100, 110, textColor, t, strlen (t), W_BoldFont);
        t = ".               .";
        W_WriteText (w, 100 + ((24) * W_Textwidth), 108,
                     textColor, t, strlen (t), W_RegularFont);
        for (i = 0; i < strlen (password1); i++)
            W_WriteText (w, 100 + ((24 + 1 + i) * W_Textwidth), 110,
                        textColor, "x", 1, W_BoldFont);
        t = " ^ ";
        W_WriteText (w, 100 + (24 + strlen (password1)) * W_Textwidth,
                     120, textColor, t, 3, W_RegularFont);
    }
    if (state == ST_MAKEPASS2)
    {
        t = "Enter it again to make sure you typed it right.";
        W_WriteText (w, 100, 130, textColor, t, strlen (t), W_BoldFont);
        t = "Your password? :";
        W_WriteText (w, 100, 140, textColor, t, strlen (t), W_BoldFont);
        t = ".               .";
        W_WriteText (w, 100 + ((16) * W_Textwidth), 138,
                     textColor, t, strlen (t), W_RegularFont);
        for (i = 0; i < strlen (password2); i++)
            W_WriteText (w, 100 + ((16 + 1 + i) * W_Textwidth), 140,
                        textColor, "x", 1, W_BoldFont);
        t = " ^ ";
        W_WriteText (w, 100 + (16 + strlen (password2)) * W_Textwidth,
                     150, textColor, t, 3, W_RegularFont);

    }
}

/******************************************************************************/
/***  adjustString()                                                        ***/
/******************************************************************************/
static void
adjustString (unsigned char ch,
              char *str,
              char *defname)
{
    switch ((unsigned char) ch)
    {
    case 21:
    case 'u' + 96:
    case 'U' + 96:
        *str = '\0';
        if (state == ST_GETNAME || state == ST_GETPASS ||
            state == ST_MAKEPASS1 || state == ST_MAKEPASS2)
            displayStartup (defname);
        break;

    case 8:
    case '\177':
    case 'h' + 96:
    case 'H' + 96:
        if (strlen (str) > 0)
        {
            str[strlen (str) - 1] = '\0';
            if (state == ST_GETNAME || state == ST_GETPASS ||
                state == ST_MAKEPASS1 || state == ST_MAKEPASS2)
                displayStartup (defname);
        }
        break;

    default:
        if (strlen (str) == 15)
            return;
        str[strlen (str) + 1] = '\0';
        str[strlen (str)] = ch;
        if (state == ST_GETNAME || state == ST_GETPASS ||
            state == ST_MAKEPASS1 || state == ST_MAKEPASS2)
            displayStartup (defname);
        break;
    }
}

/******************************************************************************/
/***  getname()                                                             ***/
/******************************************************************************/
void
getname (char *defname,
         char *defpasswd)
    /* Let person identify themselves from w */
{
    int secondsLeft = 99;
    W_Event event;
    unsigned char ch;
    char tempstr[40];
    time_t lasttime;
    int j;
    struct timeval tv;
    fd_set mask;

    autologin = (*defpasswd && *defname) ? 1 : 0;

    MZERO (mystats, sizeof (struct stats));

    // Reset the keymap for our player record
    for (j = 0; j < 95; j++)
    {
        mystats->st_keymap[j] = (unsigned char) (j + 32);
        mystats->st_keymap[j + 96] = (unsigned char) (j + 32 + 96);

#ifdef MOUSE_AS_SHIFT
        mystats->st_keymap[j + 192] = (unsigned char) (j + 32);
        mystats->st_keymap[j + 288] = (unsigned char) (j + 32);
        mystats->st_keymap[j + 384] = (unsigned char) (j + 32);
        mystats->st_keymap[j + 480] = (unsigned char) (j + 32);
        mystats->st_keymap[j + 576] = (unsigned char) (j + 32);
#endif
    }
    mystats->st_keymap[95] = 0;
    mystats->st_tticks = 1;
    mystats->st_flags = ST_MAPMODE + ST_NAMEMODE + ST_SHOWSHIELDS +
        ST_KEEPPEACE + ST_SHOWLOCAL * 2 + ST_SHOWGLOBAL * 2;
    lasttime = time (NULL);
    tempname[0] = '\0';
    password1[0] = '\0';
    password2[0] = '\0';
    state = ST_GETNAME;

    if (autologin)
        try_autologin (defname);
    if (state == ST_DONE)
        return;

    displayStartup (defname);
    for (;;)
    {
        /* TSH: added select here so this doesn't busy wait (only done once
           but it messes up performance metrics) */

        if (state == ST_DONE)
        {
            W_ClearWindow (w);
            return;
        }

        while (!W_EventsPending ())
        {

            tv.tv_sec = 0;
            tv.tv_usec = 100000;
            FD_ZERO (&mask);
            FD_SET (sock, &mask);
            /* udp sock will not be active here */
            select (32, &mask, 0, 0, &tv);

            if (lasttime != time (NULL))
            {
                lasttime++;
                secondsLeft--;
                showreadme ();
                sprintf (tempstr, "Seconds to go: %d ", secondsLeft);
                W_WriteText (w, 150, 400, textColor, tempstr,
                             strlen (tempstr), W_BoldFont);
                if (secondsLeft == 0)
                {
                    me->p_status = PFREE;
                    LineToConsole ("Auto-Quit\n");
                    exit (0);
                }
            }

            if (FD_ISSET (sock, &mask))
            {
                readFromServer (&mask);

                if (isServerDead ())
                {
                    LineToConsole ("Ack!  We've been ghostbusted!\n");
                    exit (0);
                }
            }

            if (!FD_ISSET (W_Socket (), &mask))
                continue;
        }

        W_NextEvent (&event);
        if (event.Window != w)
            continue;
        switch ((int) event.type)
        {
        case W_EV_EXPOSE:
            displayStartup (defname);
            sprintf (tempstr, "Seconds to go: %d ", secondsLeft);
            W_WriteText (w, 150, 400, textColor, tempstr, strlen (tempstr),
                         W_BoldFont);
            break;
        case W_EV_KEY:
            ch = event.key;
            if (ch == 10)
                ch = 13;

            switch (ch)
            {
            case 4:
            case 'd' + 96:
            case 'D' + 96:
                if (state == ST_GETNAME && *tempname == 0)
                    exit (0);
                break;
            case 'g' + 96:
            case 'G' + 96:
                if (state == ST_GETNAME && *tempname == 0)
                {
                    strcpy (tempname, "guest");
                    loaddude ();
                    displayStartup (tempname);
                }
                break;
            default:
                break;
            }
            if (ch < 32 && ch != 21 && ch != 13 && ch != 8)
                break;
            switch (state)
            {
            case ST_GETNAME:
                if (ch == 13)
                {
                    if (*tempname == '\0')
                    {
                        strcpy (tempname, defname);
                    }
                    loaddude ();
                    displayStartup (defname);
                }
                else
                {
                    adjustString (ch, tempname, defname);
                }
                break;
            case ST_GETPASS:
                if (ch == 13)
                {
                    checkpassword ();
                    displayStartup (defname);
                }
                else
                {
                    adjustString (ch, password1, defname);
                }
                break;
            case ST_MAKEPASS1:
                if (ch == 13)
                {
                    state = ST_MAKEPASS2;
                    displayStartup (defname);
                }
                else
                {
                    adjustString (ch, password1, defname);
                }
                break;
            case ST_MAKEPASS2:
                if (ch == 13)
                {
                    makeNewGuy ();
                    displayStartup (defname);
                }
                else
                {
                    adjustString (ch, password2, defname);
                }
                break;
            }
        }
    }
}

/******************************************************************************/
/***  noserver()                                                            ***/
/******************************************************************************/
void
noserver (void)
{
    LineToConsole ("No server name was given. Please put a default server in\n");
    LineToConsole ("your netrekrc.txt file or specify the server in the command line.\n");
    terminate (1);
}
