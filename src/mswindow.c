/******************************************************************************/
/***  File:  mswindow.c
/***
/***  Function: Windowing code for Win32 API (aka Windows NT)
/***
/***  Revisions:
/***	Jonathan Shekter Aug 94
/***	Updates for COW version, Aug 1995
/***	Shawn Collenburg
/***	Lots of stuff, Aug 1996
/***    ssheldon - Cleaned up source code, added #include "proto.h"
/***               and function header comments
/******************************************************************************/

/* WINVER and _WIN32_WINNT are for wheel mouse and 5 mouse button support */
#ifndef WINVER
#define WINVER 0x5000
#endif
#define _WIN32_WINNT 0x0500

#define NO_BOOLEAN              //So defs.h doesn't screw us over

#include <windows.h>
#include <windowsx.h>
#include <winuser.h>            // Had to include to support wheel mouse
#include <stdio.h>
#include <limits.h>
#include <string.h>
#include <richedit.h>
#include <math.h>

#include "copyright2.h"
#include "config.h"
#include "Wlib.h"
#include "defs.h"
#include "struct.h"
#include "data.h"
#include "xclrs.h"
#include "cursors.h"
#include "proto.h"
#include "resource.h"
#include "math.h"

#define RaceDefaultOffset (C_ROM - RED)


#define WIN_EDGE 1              //Width or window border edge
#define MENU_PAD 4
#define MENU_BAR 4

#define WIN_GRAPH       1
#define WIN_TEXT        2
#define WIN_MENU        3
#define WIN_SCROLL      4
#define WIN_RICHTEXT    5

#define MoveTo(dc, x, y) MoveToEx(dc, x, y, NULL);

// Custom window messages, used for communicating between threads
#define WM_TERMINATE_WAIT 0x8000
#define WM_CROSS_THREAD_DESTROY 0x8001

#define EVENT_Q_SIZE 15
//The number of events our custom queue will hold

struct menuItem
{
    char *string;
    W_Color color;
    short len;
};

struct stringList
{
    char *string;
    W_Color color;
    struct stringList *next;
};

// Structure to contain window coordinates before movement
RECT movingr;

/*
typedef struct tagWindow
   {
   HWND hwnd;
   short type;
   short border;
   W_Color BorderColor;
   short UsingCreatedCursor;
   HCURSOR cursor;
   short tiled;
   struct Icon *TileIcon;
   short NumItems;
   struct menuItem *items;
   struct stringList *strings;
   short AddedStrings;
   short TextHeight;
   short TextWidth;
   RECT ClipRect;
   W_Callback HandleKeydown;
   W_Callback HandleKeyup;
   W_Callback HandleButton;
   W_Callback HandleExpose;
   }
Window;
*/
static Window myroot;

#define FNHEADER\
   register Window *win;\
   if (!window)\
      return(0);\
   win = (Window *)window

#define FNHEADER_VOID\
   register Window *win;\
   if (!window)\
      return;\
   win = (Window *)window

#define DBHEADER_VOID\
   HDC hdc;\
   SDBUFFER * sdb;\
   register Window *win;\
   if (!window)\
       return;\
   sdb = SDB_lookup (window);\
   if (doubleBuffering && sdb && ingame)\
   {\
       win = ((Window *)sdb->window);\
       hdc = sdb->mem_dc;\
   }\
   else\
   {\
       win = (Window *)window;\
       hdc = GetDC (win->hwnd);\
   }

#define DBICONHEADER_VOID\
   HDC hdc;\
   SDBUFFER * sdb;\
   register struct Icon *bitmap = (struct Icon *) icon;\
   register Window *win;\
   int usebitmaphwnd = 0;\
   if (!window)\
       return;\
   sdb = SDB_lookup (window);\
   if (doubleBuffering && sdb && ingame)\
   {\
       win = ((Window *)sdb->window);\
       hdc = sdb->mem_dc;\
       if (bitmap->hwnd != win->hwnd)\
           usebitmaphwnd = 0;\
       else\
           usebitmaphwnd = 1;\
   }\
   else\
   {\
       win = (Window *)window;\
       if (bitmap->hwnd != win->hwnd)\
       {\
           hdc = GetDC (win->hwnd);\
           usebitmaphwnd = 0;\
       }\
       else\
       {\
           hdc = GetDC (bitmap->hwnd);\
           usebitmaphwnd = 1;\
       }\
   }

/******************************* Globals ****************************/
extern HINSTANCE MyInstance;
char ClassName[] = "NetrekWindow";
char FontFileName[] = "\\ntfonts2.fon";
extern int DefaultsLoaded;
HWND AnyWindow;

int W_FastClear = 0;
W_Font W_BigFont, W_SmallFont, W_RegularFont;
W_Font W_HighlightFont, W_UnderlineFont;
W_Color W_White = WHITE, W_Black = BLACK, W_Red = RED, W_Green = GREEN;
W_Color W_Yellow = YELLOW, W_Cyan = CYAN, W_Grey = GREY;
W_Color W_God = C_GOD, W_Ind = C_IND, W_Fed = C_FED, W_Rom = C_ROM, W_Kli = C_KLI, W_Ori =
    C_ORI;

int W_Textwidth = 6, W_Textheight = 10;
int forceMono = 0;

const int SysColorNames[] = { COLOR_BTNFACE, COLOR_BTNTEXT, COLOR_3DFACE,
    COLOR_3DDKSHADOW, COLOR_3DHILIGHT
};
DWORD TrekSysColors[] = { 0, 0xffffff, 0, 0xc0c0c0, 0x808080 };
DWORD SysColors[sizeof (TrekSysColors)];

HDC GlobalMemDC, GlobalMemDC2, GreyBitmapDC;
HBITMAP GlobalOldMemDCBitmap, GlobalOldMemDC2Bitmap;
HCURSOR TrekCursor, WarnCursor;
HCURSOR LocalCursor, MapCursor;
HCURSOR FedCursor, KliCursor, OriCursor, RomCursor;

struct
{
    COLORREF rgb;
    HBRUSH brush;
    HPEN pen;
    HPEN dashedpen;
}
colortable[COLORS] =
{
    {
    RGB (0xff, 0xff, 0xff), 0, 0, 0}
    ,                           //White
    {
    RGB (0x00, 0x00, 0x00), 0, 0, 0}
    ,                           //Black
    {
    RGB (0xff, 0x5f, 0x5f), 0, 0, 0}
    ,                           //Red
    {
    RGB (0x5f, 0xff, 0x5f), 0, 0, 0}
    ,                           //Green
    {
    RGB (0xff, 0xff, 0x5f), 0, 0, 0}
    ,                           //Yellow
    {
    RGB (0x5f, 0xff, 0xff), 0, 0, 0}
    ,                           //Cyan
    {
    RGB (0xa0, 0xa0, 0xa0), 0, 0, 0}    //Light grey
    ,
    {
    RGB (0xff, 0xff, 0xff), 0, 0, 0}    //God
    ,
    {
    RGB (0xff, 0x5f, 0x5f), 0, 0, 0}
    ,                           //Rom
    {
    RGB (0x5f, 0xff, 0x5f), 0, 0, 0}
    ,                           //Kli
    {
    RGB (0xff, 0xff, 0x5f), 0, 0, 0}
    ,                           //Fed
    {
    RGB (0x5f, 0xff, 0xff), 0, 0, 0}
    ,                           //Ori
    {
    RGB (0xa0, 0xa0, 0xa0), 0, 0, 0}    //Ind
}

,
// Used when we're in fixed-16 color mode - color values altered so that the color
// mapper picks the proper color. These are "intense" versions of the standard colors
    altcolortable[COLORS] =
{
    {
    RGB (0xff, 0xff, 0xff), 0, 0, 0}
    ,                           //White
    {
    RGB (0x00, 0x00, 0x00), 0, 0, 0}
    ,                           //Black
    {
    RGB (0xff, 0x00, 0x00), 0, 0, 0}
    ,                           //Red
    {
    RGB (0x00, 0xff, 0x00), 0, 0, 0}
    ,                           //Green
    {
    RGB (0xff, 0xff, 0x00), 0, 0, 0}
    ,                           //Yellow
    {
    RGB (0x00, 0xff, 0xff), 0, 0, 0}
    ,                           //Cyan
    {
    RGB (0xa0, 0xa0, 0xa0), 0, 0, 0}    //Light grey
    ,
    {
    RGB (0xff, 0xff, 0xff), 0, 0, 0}    //God
    ,
    {
    RGB (0xff, 0x00, 0x00), 0, 0, 0}
    ,                           //Rom
    {
    RGB (0x00, 0xff, 0x00), 0, 0, 0}
    ,                           //Kli
    {
    RGB (0xff, 0xff, 0x00), 0, 0, 0}
    ,                           //Fed
    {
    RGB (0x00, 0xff, 0xff), 0, 0, 0}
    ,                           //Ori
    {
    RGB (0xa0, 0xa0, 0xa0), 0, 0, 0}    //Ind
};

char *colornames[COLORS] = {
    {"white"},
    {"black"},
    {"red"},
    {"green"},
    {"yellow"},
    {"cyan"},
    {"light grey"},
    {"God"},
    {"Rom"},
    {"Kli"},
    {"Fed"},
    {"Ori"},
    {"Ind"}
};

HPALETTE NetrekPalette = 0;

//A structure that the W_Icon type (really a bitmap, not an icon) points to
struct Icon
{
    HWND hwnd;                  //The window this is going into
    HBITMAP bm;                 //The Windows bitmap it has been stored in
    int x, y;                   //The source position in this bitmap
    int width, height;          //guess...
    RECT *ClipRectAddr;         //Address of the window clip region
};

#ifndef SCALE_BITMAPS
#define BIGBITMAP_WIDTH 500
#define BIGBITMAP_HEIGHT 500
#else
static int BIGBITMAP_WIDTH = 500;
static int BIGBITMAP_HEIGHT = 500;
#endif

struct BitmapList
{
    HBITMAP bm;
    struct BitmapList *next;    //Put these in a LL for later de-allocation
};

struct BitmapList *CurrentBitmap = NULL;
int bmlCount = 0;

//Table that we will build use for virtual key code conversion. We can't
//just let Windows do it as we want to proces  strange things like
// ^@ or ^# which the standard windows TranslateMessage() / ToAscii()
//will not do. So, we process VK codes instead, which means we have
//to manually map the SHIFT key. We create this mapping by asking for
// the VK + shift state for each ascii char during init.

//Mapping tables for VK codes to ASCII. We can't just let windows do
//it as we want strange combinations like ^@, so we build these maps
//ourselves.
char VKMap[256];
char VKShiftMap[256];

//WinKey Kill Library Functions
const int KILL_WINKEY  = 0x0001;
const int KILL_CTRLESC = 0x0002;
const int KILL_COMBOS  = 0x0004;
const int KILL_CONTEXT = 0x0008;
HINSTANCE hWinkeyLibrary;
typedef void (__fastcall *FASTCALL_KILL)(int iFlags);
FASTCALL_KILL pfnFastCallKill;

/******************************************************************************/
/***  W_Cleanup()
/*** Does all the resource de-allocation needed
/*** The rest of the silly code doesn't play nice and call DestroyIcon(), etc,
/*** so we have to track resource usage ourself.
/******************************************************************************/
void
W_Cleanup (void)
{
    int i, j, k;
    struct BitmapList *tmp, *p = CurrentBitmap;
    char FileName[100];

    // Reset the system colors to something recognizable
    ResetSysColors ();

    //Get rid of the color pens, etc.
    for (i = 0; i < COLORS; i++)
    {
        DeleteObject (colortable[i].brush);
        DeleteObject (colortable[i].pen);
        DeleteObject (colortable[i].dashedpen);
    }

    //Delete our two loaded (and shared, and hence not deleted when the window is
    //destroyed) cursors
    DestroyCursor (TrekCursor);
    DestroyCursor (WarnCursor);
    DestroyCursor (LocalCursor);
    DestroyCursor (MapCursor);
    DestroyCursor (FedCursor);
    DestroyCursor (KliCursor);
    DestroyCursor (OriCursor);
    DestroyCursor (RomCursor);


    //Delete our fonts
    DeleteObject ((HFONT) W_RegularFont);
    DeleteObject ((HFONT) W_HighlightFont);
    DeleteObject ((HFONT) W_UnderlineFont);
    DeleteObject ((HFONT) W_SmallFont);
    DeleteObject ((HFONT) W_BigFont);

    strcpy (FileName, GetExeDir ());
    strcat (FileName, FontFileName);
//  Breaks client in Windows 95, 98, and possibly ME
//  RemoveFontResourceEx (FileName, FR_PRIVATE | FR_NOT_ENUM, 0);
    RemoveFontResource (FileName);
    //Select the original bitmaps back and delete our memory DCs
    SelectObject (GlobalMemDC, GlobalOldMemDCBitmap);
    DeleteDC (GlobalMemDC);
    SelectObject (GlobalMemDC2, GlobalOldMemDC2Bitmap);
    DeleteDC (GlobalMemDC2);
    DeleteDC (GreyBitmapDC);

    //Destory our custom palette
    if (NetrekPalette)
        DeleteObject (NetrekPalette);

    // Remove the ship bitmap structures we've created
    for (j = 0; j < NUM_TYPES; j++)
    {
        for (k = 0; k < SHIP_VIEWS; k++)
        {
            free (fed_bitmapsT[j][k]);
            free (fed_bitmapsM[j][k]);
            free (fed_bitmapsG[j][k]);
            free (fed_bitmaps1[j][k]);
            free (fed_bitmaps[j][k]);
            free (ind_bitmapsT[j][k]);
            free (ind_bitmapsM[j][k]);
            free (ind_bitmapsG[j][k]);
            free (ind_bitmaps1[j][k]);
            free (ind_bitmaps[j][k]);
            free (kli_bitmapsT[j][k]);
            free (kli_bitmapsM[j][k]);
            free (kli_bitmapsG[j][k]);
            free (kli_bitmaps1[j][k]);
            free (kli_bitmaps[j][k]);
            free (ori_bitmapsT[j][k]);
            free (ori_bitmapsM[j][k]);
            free (ori_bitmapsG[j][k]);
            free (ori_bitmaps1[j][k]);
            free (ori_bitmaps[j][k]);
            free (rom_bitmapsT[j][k]);
            free (rom_bitmapsM[j][k]);
            free (rom_bitmapsG[j][k]);
            free (rom_bitmaps1[j][k]);
            free (rom_bitmaps[j][k]);
        }
        free (fed_bitmapsHR[j]);
        free (ind_bitmapsHR[j]);
        free (kli_bitmapsHR[j]);
        free (ori_bitmapsHR[j]);
        free (rom_bitmapsHR[j]);
    }
    free (ship_bitmaps[0]);
    free (ship_bitmaps[1]);
    free (ship_bitmaps[2]);
    free (ship_bitmaps[3]);
    free (ship_bitmaps[4]);

    // Free other bitmaps
    free (fedteam);
    free (romteam);
    free (kliteam);
    free (oriteam);
    free (clockpic);
    free (clockhandpic);
    
    for (j = 0; j < NUM_CTORP_TYPES; j++)
    {
        for (i = 0; i < BMP_TORPDET_FRAMES; i++)
        {
            free (cloudC[j][i]);
            free (plcloudC[j][i]);
        }
        for (i = 0; i < BMP_TORP_FRAMES; i++)
        {
            free (torpC[j][i]);
            free (mtorpC[j][i]);
            free (plasmaC[j][i]);
            free (mplasmaC[j][i]);
        }

    }
    free (torp_cloud_bitmaps);
    free (plasma_cloud_bitmaps);
    free (mtorp_bitmaps);
    free (torp_bitmaps);
    free (mplasma_bitmaps);
    free (plasma_bitmaps);

    for (i = 0; i < BMP_TORPDET_FRAMES; i++)
    {
        free (cloud[i]);
        free (plasmacloud[i]);
    }
    free (etorp);
    free (mtorp);
    free (eplasmatorp);
    free (mplasmatorp);

    for (i = 0; i < NUMTEAMS; i++)
    {
    	free (planet_earth[i]);
    	free (planet_klingus[i]);
    	free (planet_orion[i]);
    	free (planet_romulus[i]);
    	free (planet_agri1[i]);
    	free (planet_agri2[i]);
    	free (planet_rock1[i]);
    	free (planet_rock2[i]);
    	free (mplanet_earth[i]);
    	free (mplanet_klingus[i]);
    	free (mplanet_orion[i]);
    	free (mplanet_romulus[i]);
    	free (mplanet_agri1[i]);
    	free (mplanet_agri2[i]);
    	free (mplanet_rock1[i]);
    	free (mplanet_rock2[i]);
    }
    free (planet_bitmaps[0]);
    free (planet_bitmaps[1]);
    free (planet_bitmaps[2]);
    free (planet_bitmaps[3]);
    free (planet_bitmaps[4]);
    free (planet_bitmaps[5]);
    free (planet_bitmaps[6]);
    free (planet_bitmaps[7]);
    for (i = 0; i < CPLANET_VIEWS; i++)
    {
        free (planet_unknown[i]);
    }
    free (planet_unknown_NR);
    free (mplanet_bitmaps[0]);
    free (mplanet_bitmaps[1]);
    free (mplanet_bitmaps[2]);
    free (mplanet_bitmaps[3]);
    free (mplanet_bitmaps[4]);
    free (mplanet_bitmaps[5]);
    free (mplanet_bitmaps[6]);
    free (mplanet_bitmaps[7]);
    free (mplanet_unknown);

    free (army_bitmap);
    free (marmy_bitmap);
    free (wrench_bitmap);
    free (mwrench_bitmap);
    free (fuel_bitmap);
    free (mfuel_bitmap);

    for (i = 0; i < PLANET_VIEWS; i++)
        free (bplanets[i]);
    for (i = 0; i < MPLANET_VIEWS; i++)
        free (bmplanets[i]);

    for (i = 0; i < BMP_SHIPEXPL_FRAMES; i++)
        free (expview[i]);
    for (i = 0; i < BMP_SBEXPL_FRAMES; i++)
        free (sbexpview[i]);

#ifdef VSHIELD_BITMAPS
    for (i = 0; i < SHIELD_FRAMES; i++)
        free (shield[i]);
#else
    free (shield);
#endif

    for (i = 0; i < BMP_HULL_FRAMES; i++)
        free (hull[i]);

    free (cloakicon);
    free (stipple);
    free (genopic);
    free (genopic2);

    //Remove default objects
    while (defaults)
    {
        struct stringlist *tmp;
        tmp = defaults->next;
        free (defaults->string);
        free (defaults->value);
        free (defaults);
        defaults = tmp;
    }

    //Remove tractor linked list
    while (tracthead)
    {
        struct tractor *tmp;
        tmp = tracthead->next;
        free (tracthead);
        tracthead = tmp;
    }

    //Remove the bitmap structures we've created, by traversing the linked list
    while (p)
    {
        tmp = p->next;
        DeleteObject (p->bm);
        free (p);
        p = tmp;
    }

    if (localSDB)
    {
        HBITMAP localObj;

        localObj = SelectObject (localSDB->mem_dc, localSDB->old_bmp);
        DeleteObject (localObj);
        DeleteObject (localSDB->mem_bmp);
        ReleaseDC (((Window *)localSDB->window)->hwnd, localSDB->win_dc);
        DeleteDC (localSDB->mem_dc);
        free (localSDB);
    }

    if (mapSDB)
    {
        HBITMAP mapObj;

        mapObj = SelectObject (mapSDB->mem_dc, mapSDB->old_bmp);
        DeleteObject (mapObj);
        DeleteObject (mapSDB->mem_bmp);
        ReleaseDC (((Window *)mapSDB->window)->hwnd, mapSDB->win_dc);
        DeleteDC (mapSDB->mem_dc);
        free (mapSDB);
    }

    //WinKey Kill Library Stop
    if (pfnFastCallKill != NULL)
    {
        pfnFastCallKill(0);
        FreeLibrary(hWinkeyLibrary);
    }

    /* empty console buffer */
    while (consHead)
    {
        struct cons_buffer * tmp;
        tmp = consHead->next;
        free (consHead->string);
        free (consHead);
        consHead = tmp;
    }

    if (saveFile)
        free (saveFile);
}

#define MakeTeamCursor(upper, team) \
void W_Define##upper##Cursor(W_Window window) \
   { \
   FNHEADER_VOID; \
   if (win->UsingCreatedCursor) \
      { \
      DestroyCursor( win->cursor ); \
      win->UsingCreatedCursor = FALSE; \
      } \
   win->cursor = upper##Cursor; \
   }

MakeTeamCursor (Fed, fed)
MakeTeamCursor (Kli, kli) MakeTeamCursor (Rom, rom) MakeTeamCursor (Ori, ori)
/******************************************************************************/
/***  W_Initialize()
/******************************************************************************/
void
W_Initialize (char *display)
{
    int i;
    HBITMAP junk;
    LOGFONT lf;
    char FileName[100];
    WNDCLASSEX wc;
    static int InitDone = 0;
    HMODULE hCursorLibrary;
    HMODULE hRichTextLibrary;

    GetColors ();

    if (InitDone)
        return;
    InitDone = -1;

#ifdef DEBUG
    LineToConsole ("Initializing windowing system\n");
#endif

    //Register our class
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_NOCLOSE | CS_HREDRAW | CS_VREDRAW;    //Don't allow them to close windows 
    wc.lpfnWndProc = NetrekWndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = sizeof (Window *);  //Store Window * in extra space
    wc.hInstance = MyInstance;
    wc.hIcon = (HICON) LoadImage (MyInstance, MAKEINTRESOURCE(MAIN), IMAGE_ICON, 32, 32, 0);
    wc.hIconSm = (HICON) LoadImage (MyInstance, MAKEINTRESOURCE(MAIN), IMAGE_ICON, 16, 16, 0);
    wc.hCursor = NULL;          //We're handling our own cursors
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = ClassName;

    RegisterClassEx (&wc);
    if ((hCursorLibrary = LoadLibrary ("bitmaps/CURSLIB.DLL")) == NULL)
    {
        LineToConsole ("Could not load curslib.dll\n");
        exit (1);
    }

    if ((hRichTextLibrary = LoadLibrary ("riched20.dll")) == NULL)
    {
        LineToConsole ("Could not load riched20.dll\n");
        exit (1);
    }

    //Load our cursors - the ones we use from a resource file, anyway
    TrekCursor =
        LoadImage (hCursorLibrary, MAKEINTRESOURCE (CUR_TREK), IMAGE_CURSOR,
                   32, 32, LR_MONOCHROME);
    WarnCursor =
        LoadImage (hCursorLibrary, MAKEINTRESOURCE (CUR_WARN), IMAGE_CURSOR,
                   32, 32, LR_MONOCHROME);
    LocalCursor =
        LoadImage (hCursorLibrary, MAKEINTRESOURCE (CUR_LOCAL), IMAGE_CURSOR,
                   32, 32, LR_MONOCHROME);
    MapCursor =
        LoadImage (hCursorLibrary, MAKEINTRESOURCE (CUR_MAP), IMAGE_CURSOR,
                   32, 32, LR_MONOCHROME);
    FedCursor =
        LoadImage (hCursorLibrary, MAKEINTRESOURCE (CUR_FED), IMAGE_CURSOR,
                   32, 32, LR_MONOCHROME);
    KliCursor =
        LoadImage (hCursorLibrary, MAKEINTRESOURCE (CUR_KLI), IMAGE_CURSOR,
                   32, 32, LR_MONOCHROME);
    OriCursor =
        LoadImage (hCursorLibrary, MAKEINTRESOURCE (CUR_ORI), IMAGE_CURSOR,
                   32, 32, LR_MONOCHROME);
    RomCursor =
        LoadImage (hCursorLibrary, MAKEINTRESOURCE (CUR_ROM), IMAGE_CURSOR,
                   32, 32, LR_MONOCHROME);

    if (RomCursor == NULL)
        LineToConsole ("Whoops, there it is!\n");

    FreeLibrary (hCursorLibrary);

    //Create the fonts that we need. The fonts are actually in our resource file
    strcpy (FileName, GetExeDir ());
    strcat (FileName, FontFileName);
//  Breaks client in Windows 95, 98, and possibly ME
//  AddFontResourceEx (FileName, FR_PRIVATE | FR_NOT_ENUM, 0);
    AddFontResource (FileName);
    memset (&lf, 0, sizeof (LOGFONT));

    lf.lfCharSet = ANSI_CHARSET;
    lf.lfOutPrecision = OUT_TT_PRECIS;
    lf.lfClipPrecision = CLIP_DEFAULT_PRECIS;
    lf.lfQuality = DEFAULT_QUALITY;
    lf.lfPitchAndFamily = FF_MODERN | FIXED_PITCH;

    strcpy (lf.lfFaceName, "Netrek");
    lf.lfHeight = -intDefault ("fontSize", fontSize);
    lf.lfWeight = FW_REGULAR;

    W_RegularFont = (W_Font) CreateFontIndirect (&lf);

    strcpy (lf.lfFaceName, "Netrek");
    lf.lfWeight = FW_BOLD;
    W_HighlightFont = (W_Font) CreateFontIndirect (&lf);

    strcpy (lf.lfFaceName, "Netrek");
    lf.lfWeight = FW_REGULAR;
    lf.lfUnderline = TRUE;
    W_UnderlineFont = (W_Font) CreateFontIndirect (&lf);

    //Make a small font for whatever we need
    strcpy (lf.lfFaceName, "Netrek");
    lf.lfUnderline = FALSE;
    lf.lfHeight = 10;
    lf.lfWidth = 6;
    W_SmallFont = (W_Font) CreateFontIndirect (&lf);

    //Use arial for the BigFont
    strcpy (lf.lfFaceName, "Arial");
    lf.lfWeight = FW_MEDIUM;
    lf.lfHeight = 52;
    lf.lfWidth = 32;
    W_BigFont = (W_Font) CreateFontIndirect (&lf);

    //Set up or memory DCs. We need to get the default bitmap and store it,
    //for later use. The only way we can do this is to select another bitmap
    //into it... So we'll create a junk one, then delete it.
    GlobalMemDC = CreateCompatibleDC (NULL);
    GlobalMemDC2 = CreateCompatibleDC (NULL);
    GreyBitmapDC = CreateCompatibleDC (NULL);
    junk = CreateBitmap (1, 1, 1, 1, NULL);
    GlobalOldMemDCBitmap = (HBITMAP) SelectObject (GlobalMemDC, junk);
    SelectObject (GlobalMemDC, GlobalOldMemDCBitmap);
    GlobalOldMemDC2Bitmap = (HBITMAP) SelectObject (GlobalMemDC2, junk);
    SelectObject (GlobalMemDC2, GlobalOldMemDC2Bitmap);
    DeleteObject (junk);

    {
        //Find out just how big the font is
        TEXTMETRIC ftm;

        SelectObject (GlobalMemDC, W_HighlightFont);    // Should be the biggest
        if (GetTextMetrics (GlobalMemDC, &ftm))
        {
            W_Textwidth = ftm.tmAveCharWidth;
            W_Textheight = ftm.tmHeight;
        }
    }

    //Create Virtual Key mapping table
    memset (VKMap, 0, sizeof (VKMap));
    memset (VKShiftMap, 0, sizeof (VKShiftMap));
    for (i = 0; i < 128; i++)
    {
        SHORT res = VkKeyScan ((TCHAR) i);

        /* Converted SHR's to TESTs, faster on x86 -SAC */
        if (!(res & 0xff00))    //!highbyte == no shift,ctrl,alt
            VKMap[res] = (char) i;
        else if (res & 0x100)   //Bit 1 of high byte = shift key
            VKShiftMap[res & 0xff] = (char) i;
    }
    VKMap[VK_ESCAPE] = 27;      // 27 is mapped as Ctrl-[ by Windows
    VKMap[VK_TAB] = (char) 201; //'i'+96;     // Make it look like '^i' so macroKey: TAB will work
    VKMap[VK_UP] = '>'; // accelerate
    VKMap[VK_DOWN] = '<'; // decelerate
    VKMap[VK_LEFT] = (char) 187; // turn left ^[
    VKMap[VK_RIGHT] = (char) 189; // turn right ^]
    VKMap[VK_NUMPAD0] = '0';    // I want to use Numeric Keypad!
    VKMap[VK_NUMPAD1] = '1';    // Added these mappings SRS 4/10/98
    VKMap[VK_NUMPAD2] = '2';
    VKMap[VK_NUMPAD3] = '3';
    VKMap[VK_NUMPAD4] = '4';
    VKMap[VK_NUMPAD5] = '5';
    VKMap[VK_NUMPAD6] = '6';
    VKMap[VK_NUMPAD7] = '7';
    VKMap[VK_NUMPAD8] = '8';
    VKMap[VK_NUMPAD9] = '9';
    VKMap[VK_MULTIPLY] = '*';
    VKMap[VK_ADD] = '+';
    VKMap[VK_SUBTRACT] = '-';
    VKMap[VK_DECIMAL] = '.';
    VKMap[VK_DIVIDE] = '/';

    VKShiftMap[VK_SPACE] = ' '; // Map shift+space -> space
    VKShiftMap[VK_RETURN] = '\r';       // Map shift+return -> return
    VKShiftMap[VK_ESCAPE] = 27; // Map shift+escape-> escape

    MainThreadID = GetCurrentThreadId ();       // Save main thread ID so we can tell
    MainThread = GetCurrentThread ();              // Also save main thread handle
    // which thread we're in later

    // Get the current system colors
    for (i = 0; i < (sizeof (SysColorNames) / sizeof (SysColorNames[0])); i++)
    {
        SysColors[i] = GetSysColor (SysColorNames[i]);
    }

    // WinKey Kill Library Init
    if (disableWinkey = booleanDefault ("disableWinkey", disableWinkey))
    {
        hWinkeyLibrary = LoadLibrary ("bitmaps/winkey.dll");

        if (hWinkeyLibrary < (HINSTANCE) 32)
        {
            //Just continue if there is a problem
            LineToConsole ("Unable to load __fastcall DLL");
        }
        else 
        {
            pfnFastCallKill = (FASTCALL_KILL) GetProcAddress (hWinkeyLibrary, (LPCSTR)1);

            if (!pfnFastCallKill)
            {
                //Just continue if there is a problem
                LineToConsole ("Unable to initialize __fastcall");
            }
            else
            {
                pfnFastCallKill (KILL_WINKEY | KILL_CONTEXT);
            }
        }
    }

    atexit (W_Cleanup);
}


   /*
    * Figure out what kind of display we have, for color mapping
    * There are basically three possible situations:
    * 1) A fixed 16 color display (use alternate colors so they map well)
    * 2) A paletted 8-bit display (create a custom palette and use it)
    * 3) A 16 (15?) or 24 bit true-color display (no problem...)
    * We also let the use force a selection on the above.
    */

#define hexdig(a) ( ((a)>='a' && (a)<='f') ? ((a)-'a'+10) : \
                    ( ((a)>='A' && (a)<='F') ? ((a)-'A'+10)  : ((a)-'0') ) )
#define hexbyte(a) ( hexdig(*(a))*16 + hexdig(*((a)+1)) )

/******************************************************************************/
/***  GetColors()
/******************************************************************************/
void
GetColors ()
{
    HDC hdc;
    int i, j;
    char *def;

    if (DefaultsLoaded)
    {
        forceDisplay = intDefault ("forceDisplay", forceDisplay);
        if (forceDisplay == 0)
        {
            if (forceMono = booleanDefault ("forceMono", forceMono))
                forceDisplay = 1;
            else
            {
                //Look at display hardware and make a choice
                hdc = GetDC (HWND_DESKTOP);

                i = GetDeviceCaps (hdc, BITSPIXEL) * GetDeviceCaps (hdc,
                                                                    PLANES);
#ifdef DEBUG
                LineToConsole ("Bits per pixel detected: %d\n", i);
#endif

                if ((i <= 4)
                    || ((i < 15)
                        && !(GetDeviceCaps (hdc, RASTERCAPS) & RC_PALETTE)))
                    forceDisplay = 1;
                else if ((i < 15)
                         && (GetDeviceCaps (hdc, RASTERCAPS) & RC_PALETTE))
                    forceDisplay = 2;
                else
                    forceDisplay = 3;

                ReleaseDC (HWND_DESKTOP, hdc);
            }
        }

        if (forceDisplay == 1)         // need extra bright clrs on 4 plane display
            memcpy (colortable, altcolortable, sizeof (colortable));

        //Modify colors from netrekrc file directives
        for (i = 0; i < COLORS; i++)
        {
            char buf[30];
            sprintf (buf, "color.%s", colornames[i]);   //Check netrekrc file
            def = stringDefault (buf);
            if (!def && i >= COLORS - RaceDefaultOffset)  // For race colors we use default color
            {                   // color for that race if not set, i.e.
                sprintf (buf, "color.%s", colornames[i - RaceDefaultOffset]);
                def = stringDefault (buf); // if "color.rom" is not set, "color.red"
            }
            if (def)
                if (def[0] == '#')      //Explicit RGB color
                {
                    colortable[i].rgb =
                        RGB (hexbyte (def + 1), hexbyte (def + 3),
                             hexbyte (def + 5));
                }
                else
                {
                    for (j = 0; j < XCLRS; j++) //Else assume color name
                        if (!stricmp (def, xclrs[j].name))
                            break;
                    if (j != XCLRS)
                        colortable[i].rgb =
                            RGB (xclrs[j].r, xclrs[j].g, xclrs[j].b);
                    else
                    {
                        LineToConsole ("Color '%s' unknown\n", def);
                    }
                }
        }

        switch (forceDisplay)
        {
        case 1:
#ifdef DEBUG
            LineToConsole ("16 color display detected.\n");
#endif
            break;

        case 2:
            //Paletted display. Create a custom palette, and set the RGB indices
            //in the colortable to palette indices instead of explicit colors
            {
                char space[sizeof (LOGPALETTE) +
                           sizeof (PALETTEENTRY) * (COLORS)];
                LOGPALETTE *pal = (LOGPALETTE *) space;

                pal->palVersion = 0x300;        //Windows version >=3.0 palette
                pal->palNumEntries = COLORS;
                for (i = 0; i < COLORS; i++)
                {
                    pal->palPalEntry[i].peRed = GetRValue (colortable[i].rgb);
                    pal->palPalEntry[i].peGreen =
                        GetGValue (colortable[i].rgb);
                    pal->palPalEntry[i].peBlue =
                        GetBValue (colortable[i].rgb);
                    pal->palPalEntry[i].peFlags = 0;
                    colortable[i].rgb = PALETTEINDEX (i);
                }
                NetrekPalette = CreatePalette (pal);

#ifdef DEBUG
                LineToConsole ("Paletted color display detected.\n");
#endif
            }
            break;

            //else, everything's okey dokey, we have a true-color display
        }

        //Create the various pens and brushes for each color
        for (i = 0; i < COLORS; i++)
        {
            colortable[i].brush = CreateSolidBrush (colortable[i].rgb);
            colortable[i].pen =
                CreatePen (PS_SOLID | PS_INSIDEFRAME, 1, colortable[i].rgb);
            colortable[i].dashedpen =
                CreatePen (PS_DOT, 1, colortable[i].rgb);
        }
    }
}

/******************************************************************************/
/***  W_RenameWindow()
/******************************************************************************/
W_Window
W_RenameWindow (W_Window window,
                char *str)
{
    FNHEADER;
    SetWindowText (win->hwnd, str);
    return window;
}

/******************************************************************************/
/***  checkParent()
/******************************************************************************/
void
checkParent (char *name,
             W_Window * parent)
{
    char *adefault;
    char buf[100];
    HWND found;

    sprintf (buf, "%s.parent", name);
    adefault = stringDefault (buf);
    if (adefault == NULL)
        return;

    /* parent must be name of other window or "root" */
    if (strcmpi (adefault, "root") == 0)
    {
        //*parent = (W_Window) &myroot;
        *parent = NULL;
        return;
    }
    else if (strcmpi (adefault, "netrek") == 0)
    {
        //we changed the title of the netrek window,
        //so FindWindow won't work. Do it manually
        *parent = baseWin;
        return;
    }

    /* find the window with the name specified */
    if (found = FindWindow (ClassName, adefault))
        *parent = (W_Window) GetWindowLongPtr (found, GWLP_USERDATA);  //Return struct ptr

    return;
}


/******************************************************************************/
/***  newWindow()
/***   Internal function - used as the base for all window creation
/******************************************************************************/
Window *
newWindow (char *name,
           int x,
           int y,
           int width,
           int height,
           W_Window parent,
           int border,
           W_Color color,
           int type)
{
    Window *window, *parentwin = 0;
    HDC hdc;
    char title_buff[100];
    char *s;
    int actual_x, actual_y, actual_width, actual_height;
    DWORD SpecialStyle = 0;

    if (!(window = (Window *) malloc (sizeof (Window))))
    {
        LineToConsole ("Not enough memory to create a new window.");
        return 0;
    }
    memset (window, 0, sizeof (Window));

    //Stuff ripped from x11window.c - provides nicer titles when the window
    // is a wait window or the main window.
    if (strcmp (name, "netrek") == 0)
    {
        if (title)
            s = title;
        else
        {
            s = title_buff;
            sprintf (title_buff, "Netrek  @  %s", serverName);
        }
        // WS_THICKFRAME adds resizing frame to window without adding titlebar
        SpecialStyle |= WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU;
        if (mainResizeable)
            SpecialStyle |= WS_THICKFRAME;
        if (mainTitleBar)
            SpecialStyle |= WS_CAPTION;
    }
    else if (strcmp (name, "wait") == 0)
    {
        if (title)
            s = title;
        else
            s = serverName;
        width += GetSystemMetrics (SM_CXFRAME);
        //Make the wait window show up in the task list
        SpecialStyle =
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        parentwin = &myroot;
    }
    else if (strncmp (name, "Meta", 4) == 0)
    {
        s = name;
        height += GetSystemMetrics (SM_CYFRAME) + border;
        //Make the Metaserver window show up in the task list
        SpecialStyle =
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
        parentwin = &myroot;
    }
    else
        s = name;

    //Mask border - 15 pixels max - and make sure it's at least 1, and that it's not black
    if (border > 15)
        border = 15;
    if (!border)
        border = 1;
    if (color == BLACK)
        color = WHITE;

    //Set the various attributes to default states
    window->type = (short) type;
    window->border = (short) border;
    window->BorderColor = color;
    window->cursor = LoadCursor (NULL, IDC_ARROW);

    //Check the parent - parent to root (==desktop) if null
    if (!parentwin)
    {
        if (!parent)
        {
            parentwin = (baseWin ? (Window *) baseWin : &myroot);
//            SpecialStyle |= baseWin ? WS_POPUP | WS_CAPTION : WS_POPUP;
            SpecialStyle |= baseWin ? WS_POPUP | WS_BORDER : WS_POPUP;
        }
        else
        {
            parentwin = (Window *) parent;
            SpecialStyle |= WS_CHILD;
        }
    }

    /* Here we have name and parent, so let's set them */
    strcpy (window->name, name);
    window->parent = parentwin;

    /* Preprocess the actual coordinates */
    actual_x = x + parentwin->border;
    actual_y = y + parentwin->border;
    actual_width = width + border * 2;
    actual_height = height + border * 2;

    //Actually create the window
    //Hacked to allow negative create locations -SAC
    if (window->type == WIN_RICHTEXT)
    {
        window->hwnd = CreateWindowEx (0, RICHEDIT_CLASS, "",
                                       WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CHILD |
                                       WS_VSCROLL | WS_BORDER | ES_READONLY | ES_MULTILINE,
                                       actual_x, actual_y, actual_width, actual_height,
                                       parentwin->hwnd, NULL, MyInstance, (void *) window);
    }
    else
    {
        window->hwnd = CreateWindow (ClassName, s, WS_CLIPSIBLINGS | WS_CLIPCHILDREN | SpecialStyle, 
                                    actual_x, actual_y, actual_width, actual_height + 
                                    ((SpecialStyle & WS_CAPTION) ? GetSystemMetrics (SM_CYCAPTION) : 0), 
                                    parentwin->hwnd, NULL, MyInstance, (void *) window);      
                                    //Pass Window struct * as user param
    }

    if (!window->hwnd)
    {
        LineToConsole ("CreateWindow() for %s failed...", name);
        return (0);
    }

    /* Set actual locations for later comparison, note we don't
       record any height adjustment caused by title bar (WS_CAPTION) */
    window->actual_x = actual_x;
    window->actual_y = actual_y;
    window->actual_width = actual_width;
    window->actual_height = actual_height;
    
    //Select the custom palette if we're in a paletted mode
    hdc = GetDC (window->hwnd);
    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    ReleaseDC (window->hwnd, hdc);

    if (!AnyWindow)
        AnyWindow = window->hwnd;

    return window;
}

W_Window
W_MakeWindow (char *name,
              int x,
              int y,
              int width,
              int height,
              W_Window parent,
              int border,
              W_Color color)
{
    Window *newwin;

    //Get the default position, etc.
    checkGeometry (name, &x, &y, &width, &height);

    //Get the default parent..
    checkParent (name, &parent);

    if (!
        (newwin =
         newWindow (name, x, y, width, height, parent, border, color,
                    WIN_GRAPH)))
        return (0);
        
    /* Set original coordinates, so we will be able to restore to them */
    newwin->orig_x = x;
    newwin->orig_y = y;
    newwin->orig_width = width;
    newwin->orig_height = height;

    //Map (show) the window if the user spec'd it
    if (checkMapped (name))
        W_MapWindow ((W_Window) newwin);

    return (W_Window) newwin;
}


/******************************************************************************/
/***  W_MakeTextWindow()
/***   Make a window that contains fixed-spaced text, where columns must be
/***   aligned, etc. The x and y passed are in character cells, not pixels.
/******************************************************************************/
W_Window
W_MakeTextWindow (char *name,
                  int x,
                  int y,
                  int width,
                  int height,
                  W_Window parent,
                  int border)
{
    Window *newwin;

    //Get the default position, etc.
    checkGeometry (name, &x, &y, &width, &height);

    //Get the default parent..
    checkParent (name, &parent);

    if (!(newwin = newWindow (name, x, y,
                              width * W_Textwidth + WIN_EDGE * 2,
                              MENU_PAD * 2 + height * W_Textheight,
                              parent, border, W_Black, WIN_TEXT)))
        return (0);

    //Store the original textheight, width
    newwin->TextHeight = (short) height;
    newwin->TextWidth = (short) width;
    
    /* Set original coordinates, so we will be able to restore to them */
    newwin->orig_x = x;
    newwin->orig_y = y;
    newwin->orig_width = width * W_Textwidth + WIN_EDGE * 2;
    newwin->orig_height = MENU_PAD * 2 + height * W_Textheight;

    //Map (show) the window if the user spec'd it
    if (checkMapped (name))
        W_MapWindow ((W_Window) newwin);

    return (W_Window) newwin;
}

// Make a WIN_SCROLL type window.
// We use a scrollbar so we can look through the text, something the X version
// didn't have. Nyah, nyah.
W_Window
W_MakeScrollingWindow (char *name,
                       int x,
                       int y,
                       int width,
                       int height,
                       W_Window parent,
                       int border)
{
    Window *newwin;

    //Get the default position, etc.
    checkGeometry (name, &x, &y, &width, &height);

    //Get the default parent..
    checkParent (name, &parent);

    if (!(newwin = newWindow (name, x, y,
                              width * W_Textwidth + WIN_EDGE * 2 +
                              GetSystemMetrics (SM_CXVSCROLL),
                              height * W_Textheight + MENU_PAD * 2, parent,
                              border, W_White, WIN_SCROLL)))
        return (0);

    //Store the original textheight, width
    newwin->TextHeight = (short) height;
    newwin->TextWidth = (short) width;
    
    /* Set original coordinates, so we will be able to restore to them */
    newwin->orig_x = x;
    newwin->orig_y = y;
    newwin->orig_width = width * W_Textwidth + WIN_EDGE * 2 +
                              GetSystemMetrics (SM_CXVSCROLL);
    newwin->orig_height = height * W_Textheight + MENU_PAD * 2;

    //Give it a scroll bar, and set the range (to zero, initially)
    SetWindowLongPtr (newwin->hwnd, GWL_STYLE,
                      GetWindowLongPtr (newwin->hwnd, GWL_STYLE) | WS_VSCROLL);
    SetScrollRange (newwin->hwnd, SB_VERT, 0, 0, FALSE);

    //Map (show) the window if the user spec'd it
    if (checkMapped (name))
        W_MapWindow ((W_Window) newwin);

    return (W_Window) newwin;
}


//Make a menu window. This is similar to a text window in that it has a 1x1 mapping
//between coordinates and character cells. Note that the height parameter
//specified here indicates the number of items
W_Window
W_MakeMenu (char *name,
            int x,
            int y,
            int width,
            int height,
            W_Window parent,
            int border)
{
    struct menuItem *items;
    Window *newwin;
    int i;

    //Get the default position, etc.
    checkGeometry (name, &x, &y, &width, &height);

    //Get the default parent..
    checkParent (name, &parent);

    if (!(newwin = newWindow (name, x, y,
                              width * W_Textwidth + WIN_EDGE * 2,
                              height * (W_Textheight + MENU_PAD * 2) +
                              (height - 1) * MENU_BAR, parent, border,
                              W_Black, WIN_MENU)))
        return (0);

    if (!
        (items =
         (struct menuItem *) malloc (height * sizeof (struct menuItem))))
        LineToConsole ("Could not allocate storage for menu window");
    else
        for (i = 0; i < height; i++)
        {
            items[i].string = NULL;
            items[i].color = W_White;
        }
    newwin->items = items;
    newwin->NumItems = (short) height;
    newwin->TextHeight = (short) height;
    
    /* Set original coordinates, so we will be able to restore to them */
    newwin->orig_x = x;
    newwin->orig_y = y;
    newwin->orig_width = width * W_Textwidth + WIN_EDGE * 2;
    newwin->orig_height = height * (W_Textheight + MENU_PAD * 2) +
                              (height - 1) * MENU_BAR;

    //Map (show) the window if the user spec'd it
    if (checkMapped (name))
        W_MapWindow ((W_Window) newwin);

    return (W_Window) newwin;
}

void
W_ChangeBorder (W_Window window,
                W_Color color)
{
    DBHEADER_VOID;
    
    win->BorderColor = color;
    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    DrawBorder (win, hdc);
    
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}


//Displays the window.
void
W_MapWindow (W_Window window)
{
    FNHEADER_VOID;

    ShowWindow (win->hwnd, SW_SHOWNORMAL);
    BringWindowToTop (win->hwnd);
    
    if (window == baseWin && mainMaximized)
        ShowWindow (((Window *) baseWin)->hwnd, SW_MAXIMIZE);
}

//Hides the window.
void
W_UnmapWindow (W_Window window)
{
    FNHEADER_VOID;

    ShowWindow (win->hwnd, SW_HIDE);
}

//Returns TRUE if the window is visible, and also not minimized
int
W_IsMapped (W_Window window)
{
    FNHEADER;

    return (IsWindowVisible (win->hwnd) && !IsIconic (win->hwnd));
}

void
W_FillArea (W_Window window,
            int x,
            int y,
            int width,
            int height,
            int color)
{
    RECT r;
    register int border;
    DBHEADER_VOID;

    border = win->border;

    //do a rectangle intersection with the clipping rect, inlined for speed
    x += border;
    y += border;
    r.left = max (x, border);
    r.right = min (x + width, win->ClipRect.right);
    if (r.right < r.left)
        return;                 //Horizontal extents do not overlap
    r.top = max (y, border);
    r.bottom = min (y + height, win->ClipRect.bottom);
    if (r.bottom < r.top)
        return;                 //Vertical extents do not overlap

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    FillRect (hdc, &r, colortable[color].brush);
    
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}

void
W_ClearArea (W_Window window,
             int x,
             int y,
             int width,
             int height)
{
    RECT r;
    register int border;
    DBHEADER_VOID;
    
    border = win->border;

    if (win->type == WIN_TEXT)
    {
        x = x * W_Textwidth + WIN_EDGE;
        y = y * W_Textheight + MENU_PAD;
        width *= W_Textwidth;
        height *= W_Textheight;
    }

    //do a rectangle intersection with the clipping rect, inlined for speed
    x += border;
    y += border;
    r.left = max (x, border);
    r.right = min (x + width, win->ClipRect.right);
    if (r.right < r.left)
        return;                 //Horizontal extents do not overlap
    r.top = max (y, border);
    r.bottom = min (y + height, win->ClipRect.bottom);
    if (r.bottom < r.top)
        return;                 //Vertical extents do not overlap
   
    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    FillRect (hdc, &r, colortable[W_Black].brush);
    
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}

//We don't need to cache...
void
W_CacheClearArea (W_Window window,
                  int x,
                  int y,
                  int width,
                  int height)
{
    W_ClearArea (window, x, y, width, height);
}

// Clear multiple areas
void
W_ClearAreas (W_Window window,
              int *xs,
              int *ys,
              int *widths,
              int *heights,
              int num)
{
    RECT r;
    register int border, x, y, width, height;
    DBHEADER_VOID;
    
    border = win->border;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }

    while (num)
    {
        num--;

        x = xs[num];
        y = ys[num];
        width = widths[num];
        height = heights[num];

        if (win->type == WIN_TEXT)
        {
            x = x * W_Textwidth + WIN_EDGE;
            y = y * W_Textheight + MENU_PAD;
            width *= W_Textwidth;
            height *= W_Textheight;
        }

        //do a rectangle intersection with the clipping rect, inlined for speed
        x += border;
        y += border;
        r.left = max (x, border);
        r.right = min (x + width, win->ClipRect.right);
        if (r.right < r.left)
            continue;           //Horizontal extents do not overlap
        r.top = max (y, border);
        r.bottom = min (y + height, win->ClipRect.bottom);
        if (r.bottom < r.top)
            continue;           //Vertical extents do not overlap

        FillRect (hdc, &r, colortable[W_Black].brush);
    }

    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);

    return;
}

void
W_ClearWindow (W_Window window)
{
    DBHEADER_VOID;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    
    if (!win->tiled)
        FillRect (hdc, &win->ClipRect, colortable[BLACK].brush);
    else
    {
        struct Icon *icon = win->TileIcon;
        RECT r;
        int i, j;

        GetClientRect (win->hwnd, &r);

        //Bitmap should be white on window bk color
        SetTextColor (hdc, colortable[BLACK].rgb);
        SetBkColor (hdc, colortable[WHITE].rgb);

        //Select the bitmap
        SelectObject (GlobalMemDC, icon->bm);

        //Paste the bitmap into the window
        for (i = 0; i < r.bottom; i += icon->height)
            for (j = 0; j < r.right; j += icon->width)
                BitBlt (hdc, j, i,
                        icon->width, icon->height, GlobalMemDC, 0, 0,
                        SRCCOPY);
        DrawBorder (win, hdc);
    }
    
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}


//These globals used for communication between the following three fns
//We implement a custom queue of only those message we are interested in.
//The event (message) processing would really be quite simple, except
//for the fact that many messages in Windows are _sent_, rather than
//retrieved via GetMessage(), so the window proc must be able to post
//events to the queue on its own without calling W_NextEvent().
W_Event EventQueue[EVENT_Q_SIZE];
int EventHead = 0, EventTail = 0;

//Are there events (messages) in our queue?
//Here is the main message loop of our program. We first a)check
//to see if there is an event in the queue, and then if there isn't b) do
//the PeekMessage() thing, then c) check the queue again
int
W_EventsPending ()
{
    MSG msg;

    if ((GetAsyncKeyState (VK_SHIFT) & ~0x1)
        && (GetAsyncKeyState (27) & ~0x1))
        exit (-1);              //Safety device - Shft+ESC = clean termination


    if (EventHead != EventTail) //Event already in queue
        return (1);

    //Essentially, while there is a message in the application queue
    //and we haven't got and "event", continue processing messages
    while (PeekMessage (&msg, NULL, 0, 0, PM_REMOVE))
    {
        //TranslateMessage(&msg);          //Yes, translate keyboard messages,
        DispatchMessage (&msg); //dispatch the message to the window proc
        if (EventHead != EventTail)
            return (1);
    }

    return (0);                 //Nothing waiting
}

// Wait for an event to become available; it returns zero if it recieves a
// WM_TERMINATE_WAIT (defined as 0x8000) message,
// which we use to reset the game (it's issued by another thread)
int
W_WaitForEvent ()
{
    MSG msg;

    while (EventHead == EventTail)      // Get an event
    {
        GetMessage (&msg, NULL, 0, 0);
        if (msg.message == WM_TERMINATE_WAIT || exitFlag)   // Quit message recieved
            return 0;

        //TranslateMessage(&msg);          //translate keyboard messages,
        DispatchMessage (&msg); //dispatch the message to the window proc
    }
    return 1;
}

// Force W_WaitForEvent to terminate. Notice we send the message to 
// any window, the point is that it will be processed in the main thread.
void
W_TerminateWait ()
{
    while (PostMessage (AnyWindow, WM_TERMINATE_WAIT, 0, 0) == 0)
        ;
}

//Get the next event (message). Simply copies the event *, and advances the pointer
void
W_NextEvent (W_Event * event)
{
    if (EventHead == EventTail) //Make sure there actually is a message!
        W_WaitForEvent ();

    EventHead = (EventHead + 1) % EVENT_Q_SIZE; //Advance queue pointer
    memcpy (event, &EventQueue[EventHead], sizeof (W_Event));
}

//Return any already queued events
int
W_SpNextEvent (W_Event * event)
{
    if (W_EventsQueued ())
    {
        W_NextEvent (event);
        return 1;
    }
    return 0;
}


//The main window proc for all of the game's windows. Strange design, this netrek code...
//But I guess this is really just a dispatcher function - the events are actually
//"processed" in the rest of the code. Also, all these #ifdef's clutter it needlessly...

//Two very useful defines. These capture an event, after it is determined to be
//valid. Doing this AFTER the event (message) is determined valid increases
//the code size, and makes for the macros, but it also greatly speeds up
//processing of the zillion other messages that go through the loop.

#define GET_STRUCT_PTR\
   win = (Window *) GetWindowLongPtr (hwnd, GWLP_USERDATA)

#define STORE_EVENT\
   i = (EventTail + 1) % EVENT_Q_SIZE;\
   if (i != EventHead)\
      EventTail = i;\
   if (InSendMessage())\
      GetCursorPos(&pos);\
   else\
      {\
      *((DWORD *)&spos) = GetMessagePos();\
      pos.x = spos.x; pos.y = spos.y;\
      }\
   ScreenToClient(hwnd, &pos);\
   EventQueue[EventTail].x = pos.x - win->border;\
   EventQueue[EventTail].y = pos.y - win->border;\
   EventQueue[EventTail].Window=(W_Window) win;\
   if ( win->type == WIN_MENU )\
      {\
      if (EventQueue[EventTail].y % (W_Textheight + MENU_PAD * 2 + MENU_BAR) >=\
            W_Textheight + MENU_PAD * 2)\
       return (0);\
      EventQueue[EventTail].y /= (W_Textheight + MENU_PAD * 2 + MENU_BAR) ;\
      }

//A similar version of the above that gets the mouse pos from LPARAM -
//used in processing mouse messages
#define STORE_EVENT_MOUSE\
   i = (EventTail + 1) % EVENT_Q_SIZE;\
   if (i != EventHead)\
      EventTail = i;\
   if (InSendMessage())\
      GetCursorPos(&pos);\
   else\
      {\
      *((DWORD *)&spos) = GetMessagePos();\
      pos.x = spos.x; pos.y = spos.y;\
      }\
   ScreenToClient(hwnd, &pos);\
   EventQueue[EventTail].x = pos.x - win->border;\
   EventQueue[EventTail].y = pos.y - win->border;\
   EventQueue[EventTail].Window=(W_Window) win;\
   if ( win->type == WIN_MENU )\
      {\
      if (EventQueue[EventTail].y % (W_Textheight + MENU_PAD * 2 + MENU_BAR) >=\
            W_Textheight + MENU_PAD * 2)\
       return (0);\
      EventQueue[EventTail].y /= (W_Textheight + MENU_PAD * 2 + MENU_BAR) ;\
      }

LRESULT CALLBACK
NetrekWndProc (HWND hwnd,
               UINT msg,
               WPARAM wParam,
               LPARAM lParam)
{
    register Window *win;
    POINT pos;
    POINTS spos;
    HDC hdc;
    PAINTSTRUCT ps;
    int i, j, thresh;
    short wheel;
    struct Icon *icon;
    RECT r;
    WORD lwParam;
#if defined(MOTION_MOUSE) || defined(XTRA_MESSAGE_UI)
    static POINTS prev_pos;
    static HWND LastPressHwnd;
    static int delaytime = 0;
#endif

    switch (msg)
    {
    case WM_CREATE:
        //Store our structure pointer - passed from newWindow() as final CreateWindow() param
        SetWindowLongPtr (hwnd, GWLP_USERDATA,
                         (LONG) (Window *) ((CREATESTRUCT *) lParam)->lpCreateParams);
        return (0);

    case WM_SIZE:
        //Create/adjust our clipping rect, which is used to prevent drawing into
        //the border area.
        GET_STRUCT_PTR;

        win->ClipRect.left = win->ClipRect.top = win->border;
        win->ClipRect.right = LOWORD (lParam) - win->border;
        win->ClipRect.bottom = HIWORD (lParam) - win->border;
        // Update SDB as size/borders of window have changed
        if ((Window *) w != NULL && win->hwnd == ((Window *) w)->hwnd && localSDB)
            W_UpdateSDB((W_Window) w, localSDB);
        else if ((Window *) mapw != NULL && win->hwnd == ((Window *) mapw)->hwnd && mapSDB)
            W_UpdateSDB((W_Window) mapw, mapSDB);
        break;

    case WM_SYSCOMMAND:
        // Just tracking main window maximum/restore status here
        GET_STRUCT_PTR;

        if (win->hwnd == ((Window *) baseWin)->hwnd)
        {
            if (wParam == SC_MAXIMIZE)
                mainMaximized = 1;
            else if (wParam == SC_RESTORE)
                mainMaximized = 0;
        }
        break;

    case WM_ENTERSIZEMOVE:
        //Disable possibility to move internal windows
        {   // have to add bracket to be able to declare variables
        RECT baseRect;  // baseWin rectangle
        RECT winRect;   // current window rectangle       

        GET_STRUCT_PTR;

        /* this has to be the same as for WM_EXITSIZEMOVE */
        if (windowMove || 
            (win->hwnd == ((Window *) baseWin)->hwnd) ||
            (((Window *) metaWin != NULL && win->hwnd == ((Window *) metaWin)->hwnd)) ||
            (((Window *) waitWin != NULL && win->hwnd == ((Window *) waitWin)->hwnd)) ||
            (((Window *) waitqWin != NULL && win->hwnd == ((Window *) waitqWin)->hwnd)) ||
            (((Window *) countWin != NULL && win->hwnd == ((Window *) countWin)->hwnd)) ||
            (((Window *) motdButtonWin != NULL && win->hwnd == ((Window *) motdButtonWin)->hwnd)) ||
            (((Window *) motdWin != NULL && win->hwnd == ((Window *) motdWin)->hwnd)))
            break;

        /* Note that this routine fails miserably for windows which are not set as
           WS_CHILD as a result of not having a parent window defiined.  Certain windows
           like sound, UDP options, short packets default (in the code) to no parent window,
           though I should mention that the current netrekrc assigns a parent window to all
           windows.  If WS_CHILD is NOT set, these windows can exist outside the boundaries
           of the main client window (baseWin).  Which then in turn makes all these bounding
           calculations wrong.  */
        GetWindowRect (win->hwnd, &winRect);
        GetWindowRect (((Window *) baseWin)->hwnd, &baseRect);

        movingr.left = winRect.left - baseRect.left;
        movingr.top = winRect.top - baseRect.top;
        movingr.right = winRect.right - baseRect.left;
        movingr.bottom = winRect.bottom - baseRect.top;

        /* In case of WS_CAPTION (titlebar on) we have to subtract caption size and
           additional borders to get screen coordinates.
           If the main window is also resizeable then border sizes change from SM_CnFIXEDFRAME
           to SM_CnSIZEFRAME
        */
        if (GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION && !mainResizeable)
        {
            movingr.left -= GetSystemMetrics (SM_CXFIXEDFRAME);
            movingr.top -= (GetSystemMetrics (SM_CYFIXEDFRAME) + GetSystemMetrics (SM_CYCAPTION));
            movingr.right -= GetSystemMetrics (SM_CXFIXEDFRAME);
            movingr.bottom -= (GetSystemMetrics (SM_CYFIXEDFRAME) + GetSystemMetrics (SM_CYCAPTION));
        }
        else if (GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION && mainResizeable)
        {
            movingr.left -= GetSystemMetrics (SM_CXSIZEFRAME);
            movingr.top -= (GetSystemMetrics (SM_CYSIZEFRAME) + GetSystemMetrics (SM_CYCAPTION));
            movingr.right -= GetSystemMetrics (SM_CXSIZEFRAME);
            movingr.bottom -= (GetSystemMetrics (SM_CYSIZEFRAME) + GetSystemMetrics (SM_CYCAPTION));
        }
        else if (!(GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION) && mainResizeable)
        {
            movingr.left -= GetSystemMetrics (SM_CXSIZEFRAME) - 1;
            movingr.top -= GetSystemMetrics (SM_CYSIZEFRAME) - 1;
            movingr.right -= GetSystemMetrics (SM_CXSIZEFRAME) - 1;
            movingr.bottom -= GetSystemMetrics (SM_CYSIZEFRAME) - 1;
        }

        /* If our window has parent we have to subtract parent's border as well */
        if (win->parent != (Window *) baseWin)
        {
            movingr.left -= win->parent->border / 2;
            movingr.top -= win->parent->border / 2;
            movingr.right -= win->parent->border / 2;
            movingr.bottom -= win->parent->border / 2;
        }
        }
        break;

    case WM_EXITSIZEMOVE:
        //Disable possibility to move internal windows
        {   // have to add bracket to be able to declare variables
        int width, height;
        RECT baseRect;  // baseWin rectangle
        RECT winRect;   // current window rectangle       

        GET_STRUCT_PTR;

        // Adjust window to be square
        // Adjust TWINSIDE or GWINSIDE
        // Reinitialize whatever is necessary
        // Redo other critical windows
        // Move and refresh window
        if (windowMove && (Window *) w != NULL && win->hwnd == ((Window *) w)->hwnd)
        { 
            GetWindowRect (((Window *) w)->hwnd, &winRect);
            GetWindowRect (((Window *) baseWin)->hwnd, &baseRect);

            // Keep window square
            width = winRect.right - winRect.left;
            height = winRect.bottom - winRect.top;
            if (width > height)
                width = height;
            else
                height = width;
            TWINSIDE = width - 2 * win->border;

            // Have to reinitialize stars
            initStars();
 
            /* Beeplite TTS may need to be adjusted */
            tts_ypos = intDefault("tts_ypos", TWINSIDE / 2 - 16);

            /* Have to take into account basewin border */
            winRect.left -= baseRect.left;
            winRect.top -= baseRect.top;

            /* In case of WS_CAPTION (titlebar on) we have to subtract caption size and
               additional borders to get screen coordinates.
               If the main window is also resizeable then border sizes change from SM_CnFIXEDFRAME
               to SM_CnSIZEFRAME
            */
            if (GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION && !mainResizeable)
            {
                winRect.left -= GetSystemMetrics (SM_CXFIXEDFRAME);
                winRect.top -= (GetSystemMetrics (SM_CYFIXEDFRAME) + GetSystemMetrics (SM_CYCAPTION));
            }
            else if (GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION && mainResizeable)
            {
                winRect.left -= GetSystemMetrics (SM_CXSIZEFRAME);
                winRect.top -= (GetSystemMetrics (SM_CYSIZEFRAME) + GetSystemMetrics (SM_CYCAPTION));
            }
            else if (!(GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION) && mainResizeable)
            {
                winRect.left -= GetSystemMetrics (SM_CXSIZEFRAME) - 1;
                winRect.top -= GetSystemMetrics (SM_CYSIZEFRAME) - 1;
            }

            // All windows based on TWINSIDE are out of position now, but the team
            // select/quit windows are now the wrong size too, so we need to redo them
            for (i = 0; i < 4; i++)
            {
                W_UnmapWindow (teamWin[i]);
                teamWin[i] = W_MakeWindow (teamshort[1 << i], i * (TWINSIDE / 5), TWINSIDE - (TWINSIDE / 5),
                                           (TWINSIDE / 5), (TWINSIDE / 5), w, 1, foreColor);
                if (!ingame)
                    W_MapWindow (teamWin[i]);
            }
            W_UnmapWindow (qwin);
            qwin = W_MakeWindow ("quit", 4 * (TWINSIDE / 5), TWINSIDE - (TWINSIDE / 5), (TWINSIDE / 5),
                                 (TWINSIDE / 5), w, 1, foreColor);
            if (!ingame)
                W_MapWindow (qwin);

            MoveWindow (((Window *) w)->hwnd, winRect.left, winRect.top,
                        width, height, TRUE);

            W_FastClear = 1;
            if (viewBox)
                redrawall = 1;
        }
        else if (windowMove && (Window *) mapw != NULL && win->hwnd == ((Window *) mapw)->hwnd)
        {
            GetWindowRect (((Window *) mapw)->hwnd, &winRect);
            GetWindowRect (((Window *) baseWin)->hwnd, &baseRect);

            // Keep window square
            width = winRect.right - winRect.left;
            height = winRect.bottom - winRect.top;
            if (width > height)
                width = height;
            else
                height = width;
            GWINSIDE = width - 2 * win->border;

            /* Have to take into account basewin border */
            winRect.left -= baseRect.left;
            winRect.top -= baseRect.top;

            /* In case of WS_CAPTION (titlebar on) we have to subtract caption size and
               additional borders to get screen coordinates.
               If the main window is also resizeable then border sizes change from SM_CnFIXEDFRAME
               to SM_CnSIZEFRAME
            */
            if (GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION && !mainResizeable)
            {
                winRect.left -= GetSystemMetrics (SM_CXFIXEDFRAME);
                winRect.top -= (GetSystemMetrics (SM_CYFIXEDFRAME) + GetSystemMetrics (SM_CYCAPTION));
            }
            else if (GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION && mainResizeable)
            {
                winRect.left -= GetSystemMetrics (SM_CXSIZEFRAME);
                winRect.top -= (GetSystemMetrics (SM_CYSIZEFRAME) + GetSystemMetrics (SM_CYCAPTION));
            }
            else if (!(GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION) && mainResizeable)
            {
                winRect.left -= GetSystemMetrics (SM_CXSIZEFRAME) - 1;
                winRect.top -= GetSystemMetrics (SM_CYSIZEFRAME) - 1;
            }

            MoveWindow (((Window *) mapw)->hwnd, winRect.left, winRect.top,
                        width, height, TRUE);

            redrawall = 1;
        }

        /* this has to be the same as for WM_ENTERSIZEMOVE */
        if (windowMove || 
            (win->hwnd == ((Window *) baseWin)->hwnd) ||
            (((Window *) metaWin != NULL && win->hwnd == ((Window *) metaWin)->hwnd)) ||
            (((Window *) waitWin != NULL && win->hwnd == ((Window *) waitWin)->hwnd)) ||
            (((Window *) waitqWin != NULL && win->hwnd == ((Window *) waitqWin)->hwnd)) ||
            (((Window *) countWin != NULL && win->hwnd == ((Window *) countWin)->hwnd)) ||
            (((Window *) motdButtonWin != NULL && win->hwnd == ((Window *) motdButtonWin)->hwnd)) ||
            (((Window *) motdWin != NULL && win->hwnd == ((Window *) motdWin)->hwnd)))
            break;

        //Get window position from RECT to restore original location
        MoveWindow (win->hwnd, movingr.left, movingr.top, 
                    movingr.right - movingr.left,
                    movingr.bottom - movingr.top,
                    TRUE);
        }
        break;

    case WM_PAINT:
        GET_STRUCT_PTR;

        hdc = BeginPaint (hwnd, &ps);   //Do the beginpaint/endpaint thing
        //so that the invalid rectangle is updated
        if (NetrekPalette)
        {
            SelectPalette (hdc, NetrekPalette, FALSE);
            RealizePalette (hdc);
        }

        i = TRUE;               //Store this event
        if (win->type == WIN_MENU)
        {
            RedrawMenu (win, hdc);
            i = FALSE;          //Don't store this event
        }
        else if (win->type == WIN_SCROLL)
        {
            RedrawScrolling (win, hdc);
            i = FALSE;          //Don't store this event
        }

        DrawBorder (win, hdc);

        EndPaint (hwnd, &ps);   //Update the invalid region, etc.

        if (i)
        {
            i = (EventTail + 1) % EVENT_Q_SIZE;
            if (i != EventHead)
                EventTail = i;
            EventQueue[EventTail].type = W_EV_EXPOSE;
            EventQueue[EventTail].Window = (W_Window) GetWindowLongPtr (hwnd, GWLP_USERDATA);
            EventQueue[EventTail].x = ps.rcPaint.left;  //- win->border;
            EventQueue[EventTail].y = ps.rcPaint.top;   //- win->border;
/*            EventQueue[EventTail].width = ps.rcPaint.right - ps.rcPaint.left;
            EventQueue[EventTail].height = ps.rcPaint.bottom - ps.rcPaint.top;*/
        }
        return (0);

    case WM_LBUTTONDOWN:
        BringWindowToTop (hwnd);
        GET_STRUCT_PTR;

        STORE_EVENT_MOUSE;
        LastPressHwnd = hwnd;

#ifdef SHIFTED_MOUSE
        if (shiftedMouse)
        {
            if ((wParam & MK_SHIFT) && (wParam & MK_CONTROL))
                EventQueue[EventTail].key = W_LBUTTON4;
            else if (wParam & MK_SHIFT)
                EventQueue[EventTail].key = W_LBUTTON2;
            else if (wParam & MK_CONTROL)
                EventQueue[EventTail].key = W_LBUTTON3;
            else
                EventQueue[EventTail].key = W_LBUTTON;
        }
        else
            EventQueue[EventTail].key = W_LBUTTON;

#else
        EventQueue[EventTail].key = W_LBUTTON;
#endif /* SHIFTED_MOUSE */

#ifdef MOTION_MOUSE
        prev_pos = MAKEPOINTS (lParam);
        delaytime = udcounter;
#endif
        EventQueue[EventTail].type = W_EV_BUTTON;
        return (0);

    case WM_NCRBUTTONDOWN:
        BringWindowToTop (hwnd);
        GET_STRUCT_PTR;

        if (mainResizeable && (((Window *) mapw != NULL && win->hwnd == ((Window *) mapw)->hwnd)
             || ((Window *) w != NULL && win->hwnd == ((Window *) w)->hwnd)) )
        {
            if (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_SIZEBOX)
            {
                // remove sizing border and redraw
                SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
                SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
                // Flag local window as needing redraw
                if (win->hwnd == ((Window *) w)->hwnd)
                    W_FastClear = 1;
            }
            else
            {
                // set sizing border and redraw
                SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) | WS_SIZEBOX);
                SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
            }
        }
        break;
    case WM_MBUTTONDOWN:
        BringWindowToTop (hwnd);
        GET_STRUCT_PTR;

        STORE_EVENT_MOUSE;
        LastPressHwnd = hwnd;

#ifdef SHIFTED_MOUSE
        if (shiftedMouse)
        {
            if ((wParam & MK_SHIFT) && (wParam & MK_CONTROL))
                EventQueue[EventTail].key = W_MBUTTON4;
            else if (wParam & MK_SHIFT)
                EventQueue[EventTail].key = W_MBUTTON2;
            else if (wParam & MK_CONTROL)
                EventQueue[EventTail].key = W_MBUTTON3;
            else
                EventQueue[EventTail].key = W_MBUTTON;
        }
        else
            EventQueue[EventTail].key = W_MBUTTON;
#else
        EventQueue[EventTail].key = W_MBUTTON;
#endif /* SHIFTED_MOUSE */

#ifdef MOTION_MOUSE
        prev_pos = MAKEPOINTS (lParam);
        delaytime = udcounter;
#endif
        EventQueue[EventTail].type = W_EV_BUTTON;
        return (0);

    case WM_RBUTTONDOWN:
        BringWindowToTop (hwnd);
        GET_STRUCT_PTR;

        STORE_EVENT_MOUSE;
        LastPressHwnd = hwnd;

        if (mainResizeable && (((Window *) mapw != NULL && win->hwnd == ((Window *) mapw)->hwnd)
             || ((Window *) w != NULL && win->hwnd == ((Window *) w)->hwnd)) )
        {
            if (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_SIZEBOX)
            {
                // remove sizing border and redraw
                SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
                SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
                // Flag local window as needing redraw
                if (win->hwnd == ((Window *) w)->hwnd)
                    W_FastClear = 1;
            }
        }

#ifdef SHIFTED_MOUSE
        if (shiftedMouse)
        {
            if ((wParam & MK_SHIFT) && (wParam & MK_CONTROL))
                EventQueue[EventTail].key = W_RBUTTON4;
            else if (wParam & MK_SHIFT)
                EventQueue[EventTail].key = W_RBUTTON2;
            else if (wParam & MK_CONTROL)
                EventQueue[EventTail].key = W_RBUTTON3;
            else
                EventQueue[EventTail].key = W_RBUTTON;
        }
        else
            EventQueue[EventTail].key = W_RBUTTON;
#else
        EventQueue[EventTail].key = W_RBUTTON;
#endif /* SHIFTED_MOUSE */

#ifdef MOTION_MOUSE
        prev_pos = MAKEPOINTS (lParam);
        delaytime = udcounter;
#endif
        EventQueue[EventTail].type = W_EV_BUTTON;
        return (0);

    case WM_XBUTTONDOWN:
        //BringWindowToTop (hwnd);
        GET_STRUCT_PTR;

        STORE_EVENT_MOUSE;
        LastPressHwnd = hwnd;
        lwParam = LOWORD (wParam);

#ifdef SHIFTED_MOUSE
        if (shiftedMouse)
        {
            if ((lwParam & MK_SHIFT) && (lwParam & MK_CONTROL))
            {
            	if (lwParam & MK_XBUTTON1)
            	    EventQueue[EventTail].key = W_XBUTTON1_4;
            	else if (lwParam & MK_XBUTTON2)
            	    EventQueue[EventTail].key = W_XBUTTON2_4;
            }
            else if (lwParam & MK_SHIFT)
            {
            	if (lwParam & MK_XBUTTON1)
            	    EventQueue[EventTail].key = W_XBUTTON1_2;
            	else if (lwParam & MK_XBUTTON2)
            	    EventQueue[EventTail].key = W_XBUTTON2_2;
            }
            else if (lwParam & MK_CONTROL)
            {
            	if (lwParam & MK_XBUTTON1)
            	    EventQueue[EventTail].key = W_XBUTTON1_3;
            	else if (lwParam & MK_XBUTTON2)
            	    EventQueue[EventTail].key = W_XBUTTON2_3;
            }
            else
            {
            	if (lwParam & MK_XBUTTON1)
            	    EventQueue[EventTail].key = W_XBUTTON1;
            	else if (lwParam & MK_XBUTTON2)
            	    EventQueue[EventTail].key = W_XBUTTON2;
            }
        }
        else
            if (lwParam & MK_XBUTTON1)
                EventQueue[EventTail].key = W_XBUTTON1;
            else if (lwParam & MK_XBUTTON2)
                EventQueue[EventTail].key = W_XBUTTON2;
#else
        if (lwParam & MK_XBUTTON1)
            EventQueue[EventTail].key = W_XBUTTON1;
		else if (lwParam & MK_XBUTTON2)
			EventQueue[EventTail].key = W_XBUTTON2;
#endif /* SHIFTED_MOUSE */

#ifdef MOTION_MOUSE
        prev_pos = MAKEPOINTS (lParam);
        delaytime = udcounter;
#endif
        EventQueue[EventTail].type = W_EV_BUTTON;
        return (TRUE);	/* MSDN says you SHOULD return TRUE if processing WM_XBUTTONDOWN */

        // Watch for stuff like ALT-ENTER
    case WM_SYSKEYDOWN:
        if (wParam == VK_RETURN)
        {
            LONG wl;
            RECT r;

            wl = GetWindowLong (((Window *) baseWin)->hwnd, GWL_STYLE);
            GetWindowRect (((Window *) baseWin)->hwnd, &r);
            if (wl & WS_CAPTION)
            {
                wl &= ~WS_CAPTION;
                r.left += GetSystemMetrics (SM_CXBORDER);
                r.top += GetSystemMetrics (SM_CYBORDER);
            }
            else
            {
                wl |= WS_CAPTION;
                r.left -= GetSystemMetrics (SM_CXBORDER);
                r.top -= GetSystemMetrics (SM_CYBORDER);
            }
            mainTitleBar = !mainTitleBar;
            SetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE, wl);
            // Update the window since the height has changed
            MoveWindow (((Window *) baseWin)->hwnd, r.left, r.top,
                        r.right - r.left, r.bottom - r.top, TRUE);
            return (0);
        }
        else if (wParam == VK_END)
        {
            if (IsIconic (((Window *) baseWin)->hwnd))
                ShowWindow (((Window *) baseWin)->hwnd, SW_RESTORE);
            else
                ShowWindow (((Window *) baseWin)->hwnd, SW_MINIMIZE);
        }
        break;

        //Keyboard event - also catch the mousebuttons, #ifdef
    case WM_KEYDOWN:
        //Ugly, but I don't want to rebuild the whole keyboard processing
        if (wParam == VK_F1)
        {
            char found[256];
            if (findfile ("netrek.chm", found))
                ShellExecute (NULL, "open", found, NULL, NULL, SW_SHOWNORMAL);
            return (0);
        }
        //Manually map key to ASCII based on shift state
        j = ((GetKeyState (VK_SHIFT) & ~0x1) ? VKShiftMap : VKMap)[wParam];

        if (!j)                 //No event if we can't map the key to ASCII
            break;

        //If we're not supposed to ignore caps lock, xlate keys on Caps Lock
        if (GetKeyState (VK_CAPITAL) & 0x1)
            if (!ignoreCaps)
                j = (islower ((char) j) ? toupper ((char) j) :
                     tolower ((char) j));

        //Modified version of STORE_EVENT... we flip window pointers
        //in middle of processing after we get the mouse pos.  This
        //lets us emulate X-style keyboard focus

        i = (EventTail + 1) % EVENT_Q_SIZE;
        if (i != EventHead)
            EventTail = i;
        if (InSendMessage ())
            GetCursorPos (&pos);
        else
        {
            *((DWORD *) & spos) = GetMessagePos ();
            pos.x = spos.x;
            pos.y = spos.y;
        }

        //Change the source window in the middle of all this to the one
        //the mouse is in.

        hwnd = WindowFromPoint (pos);
        if (GetWindowLongPtr (hwnd, GWLP_WNDPROC) != (LONG) NetrekWndProc)
        {
            if (GetWindowLongPtr (hwnd, GWLP_WNDPROC) != (LONG) RichTextWndProc)
                return 0;           //Mouse is not in one of our windows
        }

        if (!hwnd || hwnd == ((Window *) baseWin)->hwnd)
            hwnd = ((Window *) w)->hwnd;

        LastPressHwnd = hwnd;

        ScreenToClient (hwnd, &pos);

        GET_STRUCT_PTR;

        EventQueue[EventTail].x = pos.x - win->border;
        EventQueue[EventTail].y = pos.y - win->border;
        EventQueue[EventTail].Window = (W_Window) win;
        if (win->type == WIN_MENU)
        {
            if (EventQueue[EventTail].y %
                (W_Textheight + MENU_PAD * 2 + MENU_BAR) >=
                W_Textheight + MENU_PAD * 2)
                return (0);
            EventQueue[EventTail].y /= (W_Textheight + MENU_PAD * 2 +
                                        MENU_BAR);
        }

        EventQueue[EventTail].type = W_EV_KEY;

#ifdef CONTROL_KEY
        if (use_control_key && (GetKeyState (VK_CONTROL) & ~0x1))
            EventQueue[EventTail].key = (unsigned char) (j + 96);
        else
            EventQueue[EventTail].key = (unsigned char) j;
#else
        EventQueue[EventTail].key = (unsigned char) j;
#endif
        return (0);

    case WM_MOUSEMOVE:
        GET_STRUCT_PTR;
        if (richText /* Richtext windows set focus, we need to get it back
                         if mouse moves out of the window */
#ifdef RECORDGAME
        && !playback
#endif
        )
        {
            if (win->hwnd != GetFocus ())
                SetFocus (win->hwnd);
        }
        SetCursor (win->cursor);

#if defined(MOTION_MOUSE) || defined(XTRA_MESSAGE_UI)
// SRS placed this here to fix the not being able to disable continuous mouse functions
        if (!continuousMouse ||
            (!motion_mouse_enablable && !motion_mouse_steering))
            return (0);

        if ((win->type != WIN_GRAPH) || (hwnd != LastPressHwnd))
            return (0);         // only allow in graphics windows and must be the window last clicked in
        // (the latter avoids a bug where Windows sends a mousemove message
        //  to a window exposed when the one on top is hidden/destroyed)

        //Check to see if we are entering a message and should be looking
        //to place the message on hold
        if (messageon && messageHoldThresh)
        {
            if (messMouseDelta)
            {
                /* a^2 + b^2 = c^2 - messageHoldThresh is c^2.
                 * Keep a running total of (a^2 + b^2) */
                int x;

                x = prev_pos.x - LOWORD (lParam);
                messMouseDelta += x * x;
                x = prev_pos.y - HIWORD (lParam);
                messMouseDelta += x * x;
            }
            else
            {
                messMouseDelta++;
                prev_pos = MAKEPOINTS (lParam);
            }
            if ((messMouseDelta >= messageHoldThresh) ||
                (wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)))
                message_hold ();
            return (0);
        }

        if (!(wParam & (MK_LBUTTON | MK_MBUTTON | MK_RBUTTON | MK_XBUTTON1 | MK_XBUTTON2)))
            return (0);         //We don't want to know about it if no mouse buttons are down

        //Check to see if the mouse has moved >= motionThresh
        thresh =
            abs (prev_pos.x - LOWORD (lParam)) + abs (prev_pos.y -
                                                      HIWORD (lParam));
        if (thresh < motionThresh)
            return (0);

        //Check to see if click delay in effect.  If so, return.  If not, reset timer.
        //Only works if mouse is moving, but that is true for all continuous mouse events,
        //i.e. holding down torp button without moving the mouse will not release a continuous
        //stream of torps.  This problem is due to a design flaw in the Windows event loop.
        if (clickDelay)
        {
            if (delaytime + clickDelay > udcounter)
                return (0);
            else
                delaytime = udcounter;
        }

        prev_pos = MAKEPOINTS (lParam);

#ifdef SHIFTED_MOUSE
        //Turn shift+button into a differnt code, and that sort of thing...
        if (shiftedMouse)
        {
            if (continuousMouseFix)
            {
                if (wParam & MK_CONTROL && wParam & MK_SHIFT)
                {
                    if ((wParam & MK_LBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_LBUTTON4;
                    }
                    if ((wParam & MK_MBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_MBUTTON4;
                    }
                    if ((wParam & MK_RBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_RBUTTON4;
                    }
                    if ((wParam & MK_XBUTTON1) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON1_4;
                    }
                    if ((wParam & MK_XBUTTON2) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON2_4;
                    }
    
                    return (0);
                }
    
                if (wParam & MK_SHIFT)
                {
                    if ((wParam & MK_LBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_LBUTTON2;
                    }
                    if ((wParam & MK_MBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_MBUTTON2;
                    }
                    if ((wParam & MK_RBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_RBUTTON2;
                    }
                    if ((wParam & MK_XBUTTON1) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON1_2;
                    }
                    if ((wParam & MK_XBUTTON2) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON2_2;
                    }
    
                    return (0);
                }

                if (wParam & MK_CONTROL)
                {
                    if ((wParam & MK_LBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_LBUTTON3;
                    }
                    if ((wParam & MK_MBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_MBUTTON3;
                    }
                    if ((wParam & MK_RBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_RBUTTON3;
                    }
                    if ((wParam & MK_XBUTTON1) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON1_3;
                    }
                    if ((wParam & MK_XBUTTON2) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON2_3;
                    }
    
                    return (0);
                }
            }
            else
            {
                if (wParam & MK_CONTROL && wParam & MK_SHIFT)
                {
                    if ((wParam & MK_LBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_LBUTTON4;
                    }
                    else if ((wParam & MK_MBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_MBUTTON4;
                    }
                    else if ((wParam & MK_RBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_RBUTTON4;
                    }
                    else if ((wParam & MK_XBUTTON1) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON1_4;
                    }
                    else if ((wParam & MK_XBUTTON2) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON2_4;
                    }
    
                    return (0);
                }

                if (wParam & MK_SHIFT)
                {
                    if ((wParam & MK_LBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_LBUTTON2;
                    }
                    else if ((wParam & MK_MBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_MBUTTON2;
                    }
                    else if ((wParam & MK_RBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_RBUTTON2;
                    }
                    else if ((wParam & MK_XBUTTON1) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON1_2;
                    }
                    else if ((wParam & MK_XBUTTON2) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON2_2;
                    }
    
                    return (0);
                }

                if (wParam & MK_CONTROL)
                {
                    if ((wParam & MK_LBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_LBUTTON3;
                    }
                    else if ((wParam & MK_MBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_MBUTTON3;
                    }
                    else if ((wParam & MK_RBUTTON) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_RBUTTON3;
                    }
                    else if ((wParam & MK_XBUTTON1) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON1_3;
                    }
                    else if ((wParam & MK_XBUTTON2) && motion_mouse_enablable)
                    {
                        //Okay, we have an event - store and process further
                        STORE_EVENT_MOUSE;
                        EventQueue[EventTail].type = W_EV_BUTTON;
                        EventQueue[EventTail].key = W_XBUTTON2_3;
                    }
    
                    return (0);
                }
            }
        }

#endif /* SHIFTED_MOUSE */

        //Whew! If, after checking for all that, there are no shift keys,
        //no controls keys, process this normally
        if (continuousMouseFix)
        {
            if ((wParam & MK_LBUTTON) && motion_mouse_enablable)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_LBUTTON;
            }
            if ((wParam & MK_MBUTTON) && motion_mouse_enablable)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_MBUTTON;
            }
            if ((wParam & MK_RBUTTON) && motion_mouse_steering)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_RBUTTON;
            }
            if ((wParam & MK_XBUTTON1) && motion_mouse_enablable)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_XBUTTON1;
            }
            if ((wParam & MK_XBUTTON2) && motion_mouse_enablable)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_XBUTTON2;
            }
        }
        else
        {
            if ((wParam & MK_LBUTTON) && motion_mouse_enablable)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_LBUTTON;
            }
            else if ((wParam & MK_MBUTTON) && motion_mouse_enablable)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_MBUTTON;
            }
            else if ((wParam & MK_RBUTTON) && motion_mouse_steering)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_RBUTTON;
            }
            else if ((wParam & MK_XBUTTON1) && motion_mouse_enablable)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_XBUTTON1;
            }
            else if ((wParam & MK_XBUTTON2) && motion_mouse_enablable)
            {
                //Okay, we have an event - store and process further
                STORE_EVENT_MOUSE;
                EventQueue[EventTail].type = W_EV_BUTTON;
                EventQueue[EventTail].key = W_XBUTTON2;
            }
        }
        return (0);

#endif /* MOTION_MOUSE */

        return (0);


        //And now, for your amusement and edification, some of the more exotic
        //mesages handled here for things like our scroll windows,
        //background bitmap tiling, and dragging windows by their borders

        //Scroll bar moved message - from a scrolling window
    case WM_VSCROLL:
        GET_STRUCT_PTR;
        i = GetScrollPos (hwnd, SB_VERT);
        switch (LOWORD (wParam))
        {
        case SB_LINEDOWN:
            i += 1;
            break;
        case SB_LINEUP:
            i -= 1;
            break;
        case SB_PAGEDOWN:
            i += win->TextHeight;
            break;
        case SB_PAGEUP:
            i -= win->TextHeight;
            break;
        case SB_THUMBTRACK:
        case SB_THUMBPOSITION:
            i = HIWORD (wParam);
            break;
        default:
            return (0);
        }
        SetScrollPos (hwnd, SB_VERT, i, TRUE);  //Move scroll
        InvalidateRect (hwnd, &win->ClipRect, TRUE);    //Redraw text in window
        UpdateWindow (hwnd);
        return (0);

    case WM_MOUSEWHEEL:
        {
        RECT baseRect;
        // wheel could be 1/-1 to show the direction of wheel move
        wheel = (short) (GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
        pos.x = (long) LOWORD (lParam);
        pos.y = (long) HIWORD (lParam);

        /* Fix in case main window isn't at (0,0) coordinates */
        GetWindowRect (((Window *) baseWin)->hwnd, &baseRect);
        pos.x -= baseRect.left;
        pos.y -= baseRect.top;
        
        /* Fix in case main window has caption or resizeable */
        if (GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION && !mainResizeable)
        {
            pos.x -= GetSystemMetrics (SM_CXFIXEDFRAME);
            pos.y -= (GetSystemMetrics (SM_CYFIXEDFRAME) + GetSystemMetrics (SM_CYCAPTION));
        }
        else if (GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION && mainResizeable)
        {
            pos.x -= GetSystemMetrics (SM_CXSIZEFRAME);
            pos.y -= (GetSystemMetrics (SM_CYSIZEFRAME) + GetSystemMetrics (SM_CYCAPTION));
        }
        else if (!(GetWindowLongPtr (((Window *) baseWin)->hwnd, GWL_STYLE) & WS_CAPTION) && mainResizeable)
        {
            pos.x -= GetSystemMetrics (SM_CXSIZEFRAME) - 1;
            pos.y -= GetSystemMetrics (SM_CYSIZEFRAME) - 1;
        }

        hwnd = ChildWindowFromPoint (((Window *) baseWin)->hwnd, pos);

        if (!hwnd)
            return (0);

        GET_STRUCT_PTR;

        // If we're not in message windows then we'll map wheel up and
        // wheel down events as regular mouse events
        if (win->type == WIN_GRAPH || win->type == WIN_TEXT || win->type == WIN_MENU)
        {
            //BringWindowToTop (hwnd);
            //GET_STRUCT_PTR;

            /* Let's see whether we should process wheel messages */
            if (!allowWheelActions)
                return (1);

            STORE_EVENT_MOUSE;
            LastPressHwnd = hwnd;

            if (wheel > 0)
            {
                EventQueue[EventTail].key = W_WHEELUP;
                EventQueue[EventTail].type = W_EV_BUTTON;
            }
            else if (wheel < 0)
            {
                EventQueue[EventTail].key = W_WHEELDOWN;
                EventQueue[EventTail].type = W_EV_BUTTON;
            }
            return (0);
        }
        else if (win->type == WIN_SCROLL)
        {
            i = GetScrollPos (hwnd, SB_VERT);
        
            switch (wheel)
            {
            case -1:
                i += 1;
                break;
            case 1:
                i -= 1;
                break;
            default:
                return (0);
            }

            SetScrollPos (hwnd, SB_VERT, i, TRUE);  //Move scroll
            InvalidateRect (hwnd, &win->ClipRect, TRUE);    //Redraw text in window
            UpdateWindow (hwnd);
            return (0);
        }
        else if (win->type == WIN_RICHTEXT)
        {
            POINT p;
            RECT r;
            int lines, visline, maxscroll;

            SendMessage (win->hwnd, EM_GETSCROLLPOS, 0, (LPARAM) &p);
            SendMessage (win->hwnd, EM_GETRECT, 0, (LPARAM) &r);
            visline = SendMessage (win->hwnd, EM_GETFIRSTVISIBLELINE, 0, 0);
            lines = SendMessage (win->hwnd, EM_GETLINECOUNT, 0, 0);
            //LineToConsole("%d visline, %d total lines\n", visline, lines);
            maxscroll = ((lines - visline) - (r.bottom / W_Textheight)) * W_Textheight;

            switch (wheel)
            {
            case -1:
                p.y += W_Textheight;
                break;
            case 1:
                p.y -= W_Textheight;
                break;
            default:
                return (0);
            }

            if (p.y > maxscroll)
                return (0);

            SendMessage (win->hwnd, EM_SETSCROLLPOS, 0, (LPARAM) &p);
            //InvalidateRect (hwnd, &win->ClipRect, TRUE);    //Redraw text in window
            //UpdateWindow (hwnd);
            return (0);
        }
        else
            return (1);
        }
        //Trap WM_ERASEBKGRND, to handle windows with tiled backgrounds
    case WM_ERASEBKGND:
        GET_STRUCT_PTR;

        GetClientRect (hwnd, &r);
        if (NetrekPalette)
        {
            SelectPalette ((HDC) wParam, NetrekPalette, FALSE);
            RealizePalette ((HDC) wParam);
        }

        if (!win->tiled)
        {
            FillRect ((HDC) wParam, &r, colortable[BLACK].brush);
        }
        else
        {
            //As far as I can tell, the bitmap should be white on window bk color...
            SetTextColor ((HDC) wParam, colortable[BLACK].rgb);
            SetBkColor ((HDC) wParam, colortable[WHITE].rgb);

            //Select the bitmap
            icon = win->TileIcon;
            SelectObject (GlobalMemDC, icon->bm);

            //Paste the bitmap into the window
            for (i = 0; i < r.bottom; i += icon->height)
                for (j = 0; j < r.right; j += icon->width)
                    BitBlt ((HDC) wParam, j, i,
                            icon->width, icon->height, GlobalMemDC, 0, 0,
                            SRCCOPY);
        }

        return TRUE;

        //Return "in caption" whenever the mouse is on the border, or in client otherwise
    case WM_NCHITTEST:
        i = DefWindowProc (hwnd, WM_NCHITTEST, wParam, lParam);
        if (i == HTCLIENT)
        {
            GET_STRUCT_PTR;
            pos.x = LOWORD (lParam);
            pos.y = HIWORD (lParam);
            ScreenToClient (hwnd, &pos);
            return (((pos.x < win->border) || (pos.x >= win->ClipRect.right)
                     || (pos.y < win->border)
                     || (pos.y >=
                         win->ClipRect.bottom)) ? HTCAPTION : HTCLIENT);
        }
        else if (i == HTBORDER)
            return HTCAPTION;
        else
            return i;

        // When PostMessage sends us WM_TERMINATE_WAIT it isn't always reaching the right
        // thread (somehow ;), so to fix the die while scrolling bug I had to add this stupid
        // flag
    case WM_TERMINATE_WAIT:
        exitFlag = 1;
        WaitForSingleObject (InputThread, INFINITE);
        return 0;

        //This message sent from another thread to indicate a window should be destroyed,
        // which only the main thread can do
    case WM_CROSS_THREAD_DESTROY:
        DestroyWindow ((HWND) wParam);
        return 0;

        //Delete all of our data, etc, when the window is killed
        //DON'T PRINTF() HERE!!! Unless of course you feel like crashing...
    case WM_DESTROY:
        GET_STRUCT_PTR;

        if (win->UsingCreatedCursor)
            DeleteObject (win->cursor);

        //If it is a menu or scroll window, free the menu item data
        if (win->type == WIN_MENU)
        {
            for (i = 0; i < win->NumItems; i++)
                if (win->items[i].string)
                    free ((void *) win->items[i].string);
            free ((void *) win->items);
        }
        else if (win->type == WIN_SCROLL)
        {
            struct stringList *p = win->strings;
            struct stringList *tmp;
            while (p)
            {
                tmp = p->next;
                free ((void *) p->string);
                free ((void *) p);
                p = tmp;
            }
        }

        //Free the Window structure
        free ((void *) win);

        return (0);

    case WM_SETFOCUS:
        if (hwnd == ((Window *) baseWin)->hwnd)
        {
            SetTrekSysColors ();
            return (0);
        }
        break;

    case WM_KILLFOCUS:
        //if (baseWin && (((void *)wParam)==((Window *)baseWin)->hwnd))
        if (hwnd == ((Window *) baseWin)->hwnd)
        {
            ResetSysColors ();
            return (0);
        }
        break;

    case WM_NOTIFY:
        if (((NMHDR *) lParam)->code == EN_LINK)
        {
            ENLINK *link = (ENLINK *) lParam;
            CHARRANGE cr;
            char str[250];
            HWND hwndFrom = link->nmhdr.hwndFrom;

            if (link->msg == WM_LBUTTONDOWN || link->msg == WM_LBUTTONDBLCLK)
            {
                SendMessage (hwndFrom, EM_EXGETSEL, 0, (LPARAM) &cr);
                SendMessage (hwndFrom, EM_EXSETSEL, (WPARAM) 0, (LPARAM) &(link->chrg));
                SendMessage (hwndFrom, EM_GETSELTEXT, 0, (LPARAM) str);
                SendMessage (hwndFrom, EM_EXSETSEL, 0, (LPARAM) &cr);
                ShellExecute (NULL, "open", str, NULL, NULL, SW_SHOWNORMAL);
            }
        }
        break;

    }

    return DefWindowProc (hwnd, msg, wParam, lParam);
}

//Slightly diffent from W_EventsPending -- does not get any new events, merely
//reads from out queue.
int
W_EventsQueued ()
{
    return (EventHead != EventTail);
}


//Draw a line.. yay, what a challenge
//Well, considering XDrawLine goes from pt to pt, and
//LineTo doesn't draw the last point, a bigger one -SAC 07 Aug 1996
//Changed to SACLineTo 25 Aug 96
void
W_MakeLine (W_Window window,
            int x0,
            int y0,
            int x1,
            int y1,
            W_Color color)
{
    register int border;
    DBHEADER_VOID;
    
    border = win->border;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SelectObject (hdc, colortable[color].pen);
    MoveTo (hdc, x0 + border, y0 + border);
    LineTo (hdc, x1 + border, y1 + border);
    /* Get that last point in there ... -SAC */
    SetPixel (hdc, x1 + border, y1 + border, colortable[color].rgb);

    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}

void
W_MakeDashedLine (W_Window window,
                  int x0,
                  int y0,
                  int x1,
                  int y1,
                  W_Color color)
{
    register int border;
    DBHEADER_VOID;
    
    border = win->border;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SetBkColor (hdc, colortable[BLACK].rgb);
    SelectObject (hdc, colortable[color].dashedpen);
    MoveTo (hdc, x0 + border, y0 + border);
    LineTo (hdc, x1 + border, y1 + border);
    /* Get that last point in there ... -SAC */
    SetPixel (hdc, x1 + border, y1 + border, colortable[color].rgb);

    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}

//We don't need to cache...
void
W_CacheLine (W_Window window,
             int x0,
             int y0,
             int x1,
             int y1,
             W_Color color)
{
    W_MakeLine (window, x0, y0, x1, y1, color);
}

void
W_MakePoint (W_Window window,
            int x0,
            int y0,
            W_Color color)
{
    register int border;
    DBHEADER_VOID;
    
    border = win->border;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SelectObject (hdc, colortable[color].pen);
    SetPixel (hdc, x0 + border, y0 + border, colortable[color].rgb);
    
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}

void
W_CachePoint (W_Window window,
              int x0,
              int y0,
              W_Color color)
{
    W_MakePoint (window, x0, y0, color);
}

// Make *many* lines :)
void
W_MakeLines (W_Window window,
             int *x0,
             int *y0,
             int *x1,
             int *y1,
             int num,
             W_Color color)
{
    register int border;
    DBHEADER_VOID;
    
    border = win->border;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SelectObject (hdc, colortable[color].pen);

    while (num)
    {
        num--;
        MoveTo (hdc, x0[num] + border, y0[num] + border);
        LineTo (hdc, x1[num] + border, y1[num] + border);
        SetPixel (hdc, x1[num] + border, y1[num] + border,
                  colortable[color].rgb);
    }
    
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
    return;
}


//Draw a dashed line, inidicating a tractor/pressor beam
//Do it manually though, as Win32 sucks at creating arbitrary pens
//Apologies if this isn't perfect, it's been a while since I had to
//write an integer linedraw routine.
//Whoops! Can't swap the points - 26 Aug 96
#define CINIT 1<<31
void
W_MakeTractLine (W_Window window,
                 int x0,
                 int y0,
                 int x1,
                 int y1,
                 W_Color color)
{
    int border;
    unsigned __int32 d1, d2, d3;
    int dp /* Dash pointer */ , dc /* Dash counter */ ;
    register int Md /* Major direction */ , md; /* minor direction */
/* 3 blank, 1 solid... etc. -SAC */
    int dashdesc[] = { 10, 1 };
    RECT r;
    DBHEADER_VOID;

    dashdesc[0] = tpDotDist;

    md = x0;
    x0 = x1;
    x1 = md;
    md = y0;
    y0 = y1;
    y1 = md;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    
    border = win->border;
    x0 += border;
    y0 += border;
    x1 += border;
    y1 += border;
    d2 = abs (x0 - x1);
    d3 = abs (y0 - y1);
    // Get cliprect so tractor lines don't overdraw border
    r.left = border;
    r.right = win->ClipRect.right;
    r.top = border;
    r.bottom = win->ClipRect.bottom;

    if (d2 > d3)
    {
        /* Major axis is x */

        if (d3)
            d1 = (unsigned __int32) ((((__int64) d3) << 32) / d2);
        else
            d1 = 1;
        d2 = (unsigned __int32) CINIT;
        Md = x0 < x1 ? 1 : -1;
        md = y0 < y1 ? 1 : -1;
        dp = 0;
        dc = dashdesc[dp];

        while ((Md == 1 ? x0 <= x1 : x0 >= x1))
        {
            if (dp & 1 && (x0 > r.left && x0 < r.right))         // An odd number
                SetPixel (hdc, x0, y0, colortable[color].rgb);
            dc--;
            if (dc < 1)
            {
                if (++dp >= 2)
                {
                    dp = 0;
                    dashdesc[dp] += 2;
                }
                dc = dashdesc[dp];
            }

            x0 += Md;
            d3 = d2;
            d2 += d1;
            if (!d1 || (d3 > d2))
            {
                y0 += md;
                dc--;
            }
        }
    }
    else
    {
        /* Major axis is y */

        if (d2)
            d1 = (unsigned __int32) ((((__int64) d2) << 32) / d3);
        else
            d1 = 1;
        d2 = (unsigned __int32) CINIT;

        Md = y0 < y1 ? 1 : -1;
        md = x0 < x1 ? 1 : -1;
        dp = 0;
        dc = dashdesc[dp];

        while ((Md == 1 ? y0 <= y1 : y0 >= y1))
        {
            if (dp & 1 && (y0 > r.top && y0 < r.bottom))         // An odd number
                SetPixel (hdc, x0, y0, colortable[color].rgb);
            dc--;
            if (dc < 1)
            {
                if (++dp >= 2)
                {
                    dp = 0;
                    dashdesc[dp] += 2;
                }
                dc = dashdesc[dp];
            }

            y0 += Md;
            d3 = d2;
            d2 += d1;
            if (!d1 || (d3 > d2))
            {
                x0 += md;
                dc--;
            }
        }
    }
    
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
    return;
}

//Draw a dashed line, inidicating a tractor/pressor beam
void
Old_W_MakeTractLine (W_Window window,
                     int x0,
                     int y0,
                     int x1,
                     int y1,
                     W_Color color)
{
    register HDC hdc;
    register int border;
    FNHEADER_VOID;
    border = win->border;

    hdc = GetDC (win->hwnd);
    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SetBkMode (hdc, TRANSPARENT);
    SelectObject (hdc, colortable[color].dashedpen);
    MoveTo (hdc, x0 + border, y0 + border);
    LineTo (hdc, x1 + border, y1 + border);
    ReleaseDC (win->hwnd, hdc);

}

//The same as MakeLine, for now... at least, for the X code it was.
void
W_MakePhaserLine (W_Window window,
                  int x0,
                  int y0,
                  int x1,
                  int y1,
                  W_Color color)
{
    register int border;
    DBHEADER_VOID;
    
    border = win->border;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SelectObject (hdc, colortable[color].pen);
    MoveTo (hdc, x0 + border, y0 + border);
    LineTo (hdc, x1 + border, y1 + border);
    SetPixel (hdc, x1 + border, y1 + border, colortable[color].rgb);
    
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}

void W_WriteCircle (W_Window window,
                    int x,
                    int y,
                    int r,
                    int highlight,
                    int dashed,
                    W_Color color)
{
    DBHEADER_VOID;

    x += win->border;
    y += win->border;
    
    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }

    if (dashed)
    {
        SetBkColor (hdc, colortable[BLACK].rgb);
        SelectObject (hdc, colortable[color].dashedpen);
    }
    else
        SelectObject (hdc, colortable[color].pen);
    SelectObject (hdc, GetStockObject (NULL_BRUSH));

    Ellipse (hdc, x - r, y - r, x + r, y + r);
    if (highlight) // Add extra white circle
    {
        SelectObject (hdc, colortable[W_White].pen);
        Ellipse (hdc, x - r - 1, y - r - 1, x + r + 1, y + r + 1);
    }
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}

//Draw a triangle. Yay.
void
W_WriteTriangle (W_Window window,
                 int x,
                 int y,
                 int s,
                 int t,
                 W_Color color)
{
    POINT points[3];
    DBHEADER_VOID;

    x += win->border;
    y += win->border;

    if (t == 0)
    {
        points[0].x = x;
        points[0].y = y;
        points[1].x = x + s;
        points[1].y = y - s;
        points[2].x = x - s;
        points[2].y = y - s;
    }
    else
    {
        points[0].x = x;
        points[0].y = y;
        points[1].x = x + s;
        points[1].y = y + s;
        points[2].x = x - s;
        points[2].y = y + s;
    }

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SelectObject (hdc, colortable[color].pen);
    SelectObject (hdc, GetStockObject (NULL_BRUSH));

    Polygon (hdc, points, 3);

    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}

//Draw a rectangle
void
W_WriteRectangle (W_Window window,
                 int x,
                 int y,
                 int width,
                 int height,
                 int dashed,
                 W_Color color)
{
    POINT points[4];
    DBHEADER_VOID;

    x += win->border;
    y += win->border;

    points[0].x = x;
    points[0].y = y;
    points[1].x = x + width;
    points[1].y = y;
    points[2].x = x + width;
    points[2].y = y + height;
    points[3].x = x;
    points[3].y = y + height;
    
    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    if (dashed)
    {
        SetBkColor (hdc, colortable[BLACK].rgb);
        SelectObject (hdc, colortable[color].dashedpen);
    }
    else
        SelectObject (hdc, colortable[color].pen);
    SelectObject (hdc, GetStockObject (NULL_BRUSH));

    Polygon (hdc, points, 4);

    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}

//Put some text in a window. This is a straight ExtTextOut call if it's a
//graphics window, a ExtTextOut call with mapped coordinates if it's a
//text window, an addition to a listbox if it's a scrolling window
//(in which case the position is ignored: it always goes at the end
//of the list) and a call to set a menu item if it's a menu window.
void
W_WriteText (W_Window window,
             int x,
             int y,
             W_Color color,
             char *str,
             int len,
             W_Font font)
{
    RECT r;
    SIZE ext;
    register int border;
    DBHEADER_VOID;
    
    border = win->border;

    switch (win->type)
    {
    case WIN_TEXT:
        x = x * W_Textwidth + WIN_EDGE;
        y = y * W_Textheight + MENU_PAD;
        /* fall through */

    case WIN_GRAPH:
        if (NetrekPalette)
        {
            SelectPalette (hdc, NetrekPalette, FALSE);
            RealizePalette (hdc);
        }

        SelectObject (hdc, (HFONT) font);
        SetTextColor (hdc, colortable[color].rgb);
        SetBkColor (hdc, colortable[BLACK].rgb);

        GetTextExtentPoint (hdc, str, len, &ext);

        //do a rectangle intersection with the clipping rect, inlined for speed
        x += border;
        y += border;
        r.left = max (x, border);
        r.right = min (x + ext.cx, win->ClipRect.right);
        if (r.right < r.left)
            return;             //Horizontal extents do not overlap
        r.top = max (y, border);
        r.bottom = min (y + ext.cy, win->ClipRect.bottom);
        if (r.bottom < r.top)
            return;             //Vertical extents do not overlap

        ExtTextOut (hdc, x, y, ETO_CLIPPED | ETO_OPAQUE, &r, str, len, NULL);
        break;

    case WIN_SCROLL:
//         str[len]=0;
        AddToScrolling (win, color, str, len);
        break;

    case WIN_RICHTEXT:
//         str[len]=0;
        AddToRichText (win, color, str, len);
        break;

    case WIN_MENU:
//         str[len]=0;
        ChangeMenuItem (win, y, str, len, color);
        break;

    default:
        LineToConsole ("Unknown window type in W_WriteText");
    }
     
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}


void
W_MaskText (W_Window window,
            int x,
            int y,
            W_Color color,
            char *str,
            int len,
            W_Font font)
{
    DBHEADER_VOID;

    switch (win->type)
    {
    case WIN_TEXT:
        x = x * W_Textwidth + WIN_EDGE;
        y = y * W_Textheight + MENU_PAD;
        /* fall through */

    case WIN_GRAPH:
        if (NetrekPalette)
        {
            SelectPalette (hdc, NetrekPalette, FALSE);
            RealizePalette (hdc);
        }
        SelectObject (hdc, (HFONT) font);
        SetTextColor (hdc, colortable[color].rgb);
        SetBkMode (hdc, TRANSPARENT);

        ExtTextOut (hdc, x + win->border, y + win->border, ETO_CLIPPED,
                    &(win->ClipRect), str, len, NULL);
        break;

    case WIN_SCROLL:
        str[len] = 0;
        AddToScrolling (win, color, str, len);
        break;

    case WIN_RICHTEXT:
        str[len]=0;
        AddToRichText (win, color, str, len);
        break;

    case WIN_MENU:
        str[len] = 0;
        ChangeMenuItem (win, y, str, len, color);
        break;

    default:
        LineToConsole ("Unknown window type in W_WriteText");
    }
    
    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}


#ifdef SHOWBITMAPS
// Succesively dump each BitmapList
void
W_ShowBitmaps ()
{
    static struct BitmapList *pBM = 0;
    static W_Window tmp = 0;
    HDC hdc;
    int t;

    LineToConsole ("pBM: %p\nbmlCount: %d", pBM, bmlCount);

    if (!pBM)
    {
        pBM = CurrentBitmap;
        if (!tmp)
            tmp =
                W_MakeWindow ("Bitmap", 10, 10, BIGBITMAP_WIDTH + 5,
                              BIGBITMAP_HEIGHT + 5, NULL, 0, W_Black);
        if (tmp)
            W_MapWindow (tmp);
    }
    else
    {
        pBM = pBM->next;
    }

    if (pBM && tmp)
    {
        LineToConsole ("tmp: %p\n", tmp);

        if (!SelectObject (GlobalMemDC, pBM->bm))
            goto fail;
        if (!(hdc = GetDC (((Window *) tmp)->hwnd)))
            goto fail;
        BitBlt (hdc, 0, 0, BIGBITMAP_WIDTH, BIGBITMAP_HEIGHT, GlobalMemDC, 0,
                0, SRCCOPY);
        SelectObject (GlobalMemDC, GlobalOldMemDCBitmap);
        ReleaseDC (((Window *) tmp)->hwnd, hdc);
      fail:;

    }
    else
    {
        // We've reached the end, kill the window.
        if (tmp)
            W_DestroyWindow (tmp);
        pBM = NULL;
    }

    return;
}
#endif

//Creates a bitmap object out of X11 bitmap data...
//What we actually do is allocate a number of 240x240 bitmaps, and copy
//each image to a new section of the bitmap. Part of the W_Icon structure
//that we return has an x,y index into this bitmap. In this fashion,
//we avoid allocating every bitmap handle in the system for the 1,000 or so
//bitmaps that we need.
//Fixed to actually work as intended - 19 Aug 96 SAC
W_Icon
W_StoreBitmap (int width,
               int height,
               char *bits,
               W_Window window)
{
    static int CurX = 0, CurY = -1, MaxY = 0;
    struct Icon *bitmap = 0;
    struct BitmapList *NewBitmap = 0;
    unsigned char *bits2 = 0;
    HBITMAP temp = 0, temp2 = 0;
    int newwidth;

    FNHEADER;

    if (CurY == -1)
        CurY = BIGBITMAP_HEIGHT + 1;    // Force the first bitmap to be created

    //Allocate memory for the bitmap structure and convert the
    //X11 bitmap into a useful form
    if (!(bitmap = (struct Icon *) malloc (sizeof (struct Icon))) ||
        !(bits2 = X11toDIB (bits, width, height)))
        goto memfail;

    newwidth = ((width + 0xf) & ~0xf);
    //Create a temporary Windows bitmap object
    if (!(temp = CreateBitmap (newwidth, height, 1, 1, bits2)))
        goto memfail;

    free (bits2);

    //Save the width, height, window...
    bitmap->hwnd = win->hwnd;
    bitmap->width = width;
    bitmap->height = height;
    bitmap->ClipRectAddr = &win->ClipRect;

    //Is the bitmap large enough to justify it's own HBitmap, or
    //should we make it part of a "Big Bitmap"?
    if ((width > (BIGBITMAP_WIDTH / 2)) && (height > (BIGBITMAP_HEIGHT / 2)))   //if it's big....
    {
        if (!
            (NewBitmap =
             (struct BitmapList *) malloc (sizeof (struct BitmapList))))
            goto memfail;

        //Copy the bitmap to new location so we can mirror it
        if (!(temp2 = CreateBitmap (width, height, 1, 1, NULL)))
            goto memfail;

        LineToConsole (".");
        bmlCount++;

        //Mirror bitmap
        SelectObject (GlobalMemDC, temp);
        SelectObject (GlobalMemDC2, temp2);
        StretchBlt (GlobalMemDC2, 0, 0, width, height, GlobalMemDC,
                    newwidth - 1, 0, -width, height, SRCCOPY);

        NewBitmap->bm = temp2;
        bitmap->bm = temp2;
        bitmap->x = 0;
        bitmap->y = 0;

        if (CurrentBitmap)
        {
            NewBitmap->next = CurrentBitmap->next;      //Insert the entry in the linked list
            CurrentBitmap->next = NewBitmap;    //as the item after CurrentBitmap
        }
        else
        {
            LineToConsole
                ("Got impossible internal error in W_StoreBitmap(). Please report to author.\n");
            exit (-1);
        }

    }
    else                        // Small(ish)
    {
        //Figure out where to store the image in the current big bitmap
        width = (width + 0x7) & ~0x7;   //Round up to nearest eight bits
        //height = (height + 0x7) & ~0x7;
        if ((CurX + width) > BIGBITMAP_WIDTH
            || (CurY + height) > BIGBITMAP_HEIGHT)
        {
            CurX = 0;           //Advance to next line
            CurY += MaxY;
            MaxY = 0;
            if ((CurY + height) > BIGBITMAP_HEIGHT)
            {
                HBRUSH SpeckleBr;
                HBITMAP hb;
                WORD mask[] = { 0xaaaa, 0x5555 };
                RECT bmrect;

                if (!
                    (NewBitmap =
                     (struct BitmapList *)
                     malloc (sizeof (struct BitmapList)))
                    || !(NewBitmap->bm =
                         CreateBitmap (BIGBITMAP_WIDTH, BIGBITMAP_HEIGHT, 1,
                                       1, NULL)))
                    goto memfail;

                bmlCount++;

                NewBitmap->next = CurrentBitmap;        //Advance to next bitmap
                CurrentBitmap = NewBitmap;      //Store in LL

                CurX = 0;
                CurY = 0;
                MaxY = 0;
                bmrect.top = 0;
                bmrect.left = 0;
                bmrect.right = BIGBITMAP_WIDTH - 1;
                bmrect.bottom = BIGBITMAP_HEIGHT - 1;

                hb = CreateBitmap (2, 2, 1, 1, (const void *) mask);
                SpeckleBr = CreatePatternBrush (hb);
                DeleteObject (hb);
                SelectObject (GlobalMemDC2, CurrentBitmap->bm);
                FillRect (GlobalMemDC2, &bmrect, SpeckleBr);
                DeleteObject (SpeckleBr);
                SelectObject (GlobalMemDC2, GlobalOldMemDC2Bitmap);

            }
        }

        //Copy the bits to their new location, mirroring as we go
        if (!SelectObject (GlobalMemDC, temp))
            LineToConsole ("SelectObject(DC, temp) bombed");
        if (!SelectObject (GlobalMemDC2, CurrentBitmap->bm))
            LineToConsole ("SelectObject(DC2,CurrentBitmap->tm) bombed");

        StretchBlt (GlobalMemDC2, CurX, CurY, width, height, GlobalMemDC,
                    newwidth - 1, 0, -width, height, SRCCOPY);

        MaxY = max (MaxY, height);      //The current row (of bitmaps) max height

        //Store the position in the big bitmap, and increment the current X pointer
        bitmap->bm = CurrentBitmap->bm;
        bitmap->x = CurX;
        bitmap->y = CurY;
        CurX += width;
    }

    SelectObject (GlobalMemDC, GlobalOldMemDCBitmap);   //So we don't crunch on the next line...
    DeleteObject (temp);
    SelectObject (GlobalMemDC2, GlobalOldMemDC2Bitmap); //Bitmaps can only be selected into one
    //DC at a time, so deselect it from here.
    return (W_Icon) bitmap;

  memfail:
    LineToConsole ("Memory allocation or CreateBitmap() failure in StoreBitmap\n");
    if (bits2)
        free (bits2);
    if (bitmap)
        free (bitmap);
    if (NewBitmap)
        free (NewBitmap);
    if (temp)
        DeleteObject (temp);
    if (temp2)
        DeleteObject (temp2);
    return NULL;
}


// Simplified version of StoreBitmap
// Loads bitmap from resource file
// This one is a bit more of a hog, no more giant bitmaps
W_Icon
W_StoreBitmap2 (HINSTANCE hDLLInstance,
                int width,
                int height,
                int bits,
                W_Window window,
                UINT BmpType)
{
    struct Icon *bitmap = 0;
    HBITMAP temp = 0;

    FNHEADER;
    //Allocate memory for the bitmap structure
    if (!(bitmap = (struct Icon *) malloc (sizeof (struct Icon))))
        goto memfail;

    // Load bitmap into memory
// Change to LR_MONOCHROME for mono images
//   if (! (temp = LoadImage(hDLLInstance, MAKEINTRESOURCE(bits), IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR)))
//   if (! (temp = LoadImage(hDLLInstance, MAKEINTRESOURCE(bits), IMAGE_BITMAP, 0, 0, LR_MONOCHROME)))
    if (BmpType == LR_MONOCHROME)
    {
        if (!
            (temp =
             LoadImage (hDLLInstance, MAKEINTRESOURCE (bits), IMAGE_BITMAP, 0,
                        0, BmpType)))
            goto memfail;
    }
    else
    {
        if (!
            (temp =
             LoadImage (hDLLInstance, MAKEINTRESOURCE (bits), IMAGE_BITMAP, 0,
                        0, BmpType | LR_CREATEDIBSECTION)))
            goto memfail;
    }
    //Save the width, height, window...
    bitmap->hwnd = win->hwnd;
    bitmap->width = width;
    bitmap->height = height;
    bitmap->ClipRectAddr = &win->ClipRect;
    bitmap->bm = temp;
    bitmap->x = 0;
    bitmap->y = 0;

    return (W_Icon) bitmap;

  memfail:
    LineToConsole ("Memory allocation or CreateBitmap() failure in StoreBitmap2()\n");
    LineToConsole ("Id: %d %d %d\n", height, width, bits);

    if (bitmap)
        free (bitmap);
    if (temp)
        DeleteObject (temp);
    return NULL;
}


// Simplified version of StoreBitmap
// Loads bitmap from .BMP file
W_Icon
W_StoreBitmap3 (char *BMPfile,
                int width,
                int height,
                int bits,
                W_Window window,
                UINT BmpType)
{
    struct Icon *bitmap = 0;
    HBITMAP temp = 0;

    FNHEADER;
    //Allocate memory for the bitmap structure
    if (!(bitmap = (struct Icon *) malloc (sizeof (struct Icon))))
        goto memfail;

    // Load bitmap into memory
    if (BmpType == LR_MONOCHROME)
    {
        if (!
            (temp =
             LoadImage (0, BMPfile, IMAGE_BITMAP, 0, 0,
                        LR_LOADFROMFILE | BmpType)))
            goto memfail;
    }
    else
    {
        if (!
            (temp =
             LoadImage (0, BMPfile, IMAGE_BITMAP, 0, 0,
                        LR_LOADFROMFILE | BmpType | LR_CREATEDIBSECTION)))
            goto memfail;
    }
    //Save the width, height, window...
    bitmap->hwnd = win->hwnd;
    bitmap->width = width;
    bitmap->height = height;
    bitmap->ClipRectAddr = &win->ClipRect;
    bitmap->bm = temp;
    bitmap->x = 0;
    bitmap->y = 0;

    return (W_Icon) bitmap;

  memfail:
    LineToConsole ("Memory allocation or CreateBitmap() failure in StoreBitmap3()\n");
    LineToConsole ("Id: %d %d %s\n", height, width, BMPfile);

    if (bitmap)
        free (bitmap);
    if (temp)
        DeleteObject (temp);
    return NULL;
}


// Sort of the second part of StoreBitmap
// The Bitmap is loaded, this points to the bitmap in a larger bitmap
W_Icon
W_PointBitmap2 (W_Icon bigbitmap,
                int col,
                int row,
                int width,
                int height)
{
    struct Icon *bitmap = 0;
    struct Icon *temp = (struct Icon *) bigbitmap;

    if (!(bitmap = (struct Icon *) malloc (sizeof (struct Icon))))
        goto memfail;

    bitmap->hwnd = temp->hwnd;
    bitmap->bm = temp->bm;
    bitmap->x = col * width;
    bitmap->y = row * height;
    bitmap->ClipRectAddr = temp->ClipRectAddr;
    bitmap->width = width;
    bitmap->height = height;

    return (W_Icon) bitmap;

  memfail:
    LineToConsole ("Memory allocation or malloc() failure in PointBitmap2()\n");
    LineToConsole ("Id: %d %d %d %d \n", col, row, height, width);

    if (bitmap)
        free (bitmap);
    return NULL;
}


//Put an image on the screen (yay!). Because of all the #$%#$ work that
//has gone into storing the bitmaps (see above), this is thankfully
//a low-overhead call - almost directly translates into the GDI BitBlt
void
W_WriteBitmap (int x,
               int y,
               W_Icon icon,
               W_Color color,
               W_Window window)
{
    register int border, width, height;
    register int srcx, srcy;
    DBICONHEADER_VOID;

    //Fast (I hope) rectangle intersection, don't overwrite our borders
    srcx = bitmap->x;
    srcy = bitmap->y;
    if (usebitmaphwnd)
        border = bitmap->ClipRectAddr->top;
    else
        border = win->ClipRect.top;
    x += border;
    y += border;

    if (x < border)
    {
        width = bitmap->width - (border - x);
        srcx += border - x;
        x = border;
    }
    else if ((width = (usebitmaphwnd ? bitmap->ClipRectAddr->right : win->ClipRect.right) - x) > bitmap->width)
        width = bitmap->width;
    if (y < border)
    {
        height = bitmap->height - (border - y);
        srcy += (border - y);
        y = border;
    }
    else if ((height = (usebitmaphwnd ? bitmap->ClipRectAddr->bottom : win->ClipRect.bottom) - y) > bitmap->height)
        height = bitmap->height;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SelectObject (GlobalMemDC, bitmap->bm);

    //Set the color of the bitmap
    //(oddly enough, 1-bit = bk color, 0-bit = text (fg) color)
    SetBkColor (hdc, colortable[color].rgb);
    SetTextColor (hdc, colortable[BLACK].rgb);

    BitBlt (hdc, x, y,          //Copy the bitmap
            width, height, GlobalMemDC, srcx, srcy, SRCPAINT);  // <-- using OR mode

    if (!sdb || !doubleBuffering || !ingame)
    {
    	if (usebitmaphwnd)
    	    ReleaseDC (bitmap->hwnd, hdc);
    	else
            ReleaseDC (win->hwnd, hdc);
    }
}


// Modified WriteBitmap for scaling and rotating images - BB, 5/1/2006
void
W_WriteScaleBitmap (int x,
                    int y,
                    int destwidth,
                    int destheight,
                    int srcwidth,
                    int srcheight,
                    int angle,
                    W_Icon icon,
                    W_Color color,
                    W_Window window)
{
    register int border;
    register int srcx, srcy;
    HBITMAP newbmp;
    XFORM xForm;
    int newwidth, newheight;
    double radians;
    float cosine, sine, xscale, yscale, eDx, eDy;
    DBICONHEADER_VOID;

    // First copy bitmap into new bitmap, and scale it.  This makes life
    // easier for doing border intersection
    srcx = bitmap->x;
    srcy = bitmap->y;

    newbmp = CreateCompatibleBitmap ( hdc, destwidth, destheight );
    
    SelectObject (GlobalMemDC, bitmap->bm);
    SelectObject (GlobalMemDC2, newbmp);

    // Need to setup colors and stuff
    if (NetrekPalette)
    {
        SelectPalette (GlobalMemDC2, NetrekPalette, FALSE);
        RealizePalette (GlobalMemDC2);
    }

    SetBkColor (GlobalMemDC2, colortable[color].rgb);
    SetTextColor (GlobalMemDC2, colortable[BLACK].rgb);

    // Copy selected section of main bitmap into newbmp before rotation
    SetStretchBltMode(GlobalMemDC2, COLORONCOLOR);
    StretchBlt(GlobalMemDC2, 0, 0, destwidth, destheight, GlobalMemDC,
               srcx, srcy, srcwidth, srcheight, SRCCOPY);
    
    //Fast (I hope) rectangle intersection, don't overwrite our borders
    if (usebitmaphwnd)
        border = bitmap->ClipRectAddr->top;
    else
        border = win->ClipRect.top;
    
    // Reset srcx and srcy as our new source bitmap starts at 0,0
    srcx = 0;
    srcy = 0;
    x += border;
    y += border;
    
    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    //Set the color of the bitmap
    //(oddly enough, 1-bit = bk color, 0-bit = text (fg) color)
    SetBkColor (hdc, colortable[color].rgb);
    SetTextColor (hdc, colortable[BLACK].rgb);

    if (angle != 0)
    {
        //Convert angle to radians 
        radians=(2*3.14159*angle/360);

        cosine=(float)cos(radians);
        sine=(float)sin(radians);
        
        /*  Can't get this to work
         *  Feel free to try and fix it - BB

        int x1, x2, x3, y1, y2, y3, minx, maxx, miny, maxy, w, h;

        // Time to find out the new width and height of the rotated bitmap!
        // First get the coordinates of the 3 corners (assume 1 corner at 0,0)
        x1 = (int)(destheight * sine);
        y1 = (int)(destheight * cosine);
        x2 = (int)(destwidth * cosine + destheight * sine);
        y2 = (int)(destheight * cosine - destwidth * sine);
        x3 = (int)(destwidth * cosine);
        y3 = (int)(-destwidth * sine);
        
        minx = min(0,min(x1, min(x2,x3)));
        miny = min(0,min(y1, min(y2,y3)));
        maxx = max(0,max(x1, max(x2,x3)));
        maxy = max(0,max(y1, max(y2,y3)));
        
        w = abs (maxx - minx);
        h = abs (maxy - miny);

        // Calculate border constraints with rotated bitmap width/height
        if (x < border)
        {
            newwidth = w - (border - x);
            srcx += border - x;
            x = border;
        }
        else if ((newwidth = (usebitmaphwnd ? bitmap->ClipRectAddr->right : win->ClipRect.right) - x) > w)
            newwidth = w;
        if (y < border)
        {
            newheight = h - (border - y);
            srcy += (border - y);
            y = border;
        }
        else if ((newheight = (usebitmaphwnd ? bitmap->ClipRectAddr->bottom : win->ClipRect.bottom) - y) > h)
            newheight = h;
        */
        newwidth = destwidth;
        newheight = destheight;

        // Scale used to find bitmap center
        xscale = (float)(destwidth/2);
        yscale = (float)(destheight/2);

        eDx = x + xscale - cosine*(xscale) + sine*(yscale);
        eDy = y + yscale - cosine*(yscale) - sine*(xscale);
        SetGraphicsMode(hdc,GM_ADVANCED);

        xForm.eM11=cosine;
        xForm.eM12=sine;
        xForm.eM21=-sine;
        xForm.eM22=cosine;
        xForm.eDx = eDx;
        xForm.eDy = eDy;

        SetWorldTransform(hdc,&xForm);
        BitBlt(hdc, 0, 0, newwidth, newheight, GlobalMemDC2, srcx, srcy, SRCPAINT);

        // Reset xForm
        xForm.eM11 = (FLOAT) 1.0; 
        xForm.eM12 = (FLOAT) 0.0; 
        xForm.eM21 = (FLOAT) 0.0; 
        xForm.eM22 = (FLOAT) 1.0; 
        xForm.eDx  = (FLOAT) 0.0; 
        xForm.eDy  = (FLOAT) 0.0; 

        SetWorldTransform(hdc,&xForm); 
        SetGraphicsMode(hdc,GM_COMPATIBLE);
    }
    else
    {
    	// Calculate border constraints with bitmap width/height
        if (x < border)
        {
            newwidth = destwidth - (border - x);
            srcx += border - x;
            x = border;
        }
        else if ((newwidth = (usebitmaphwnd ? bitmap->ClipRectAddr->right : win->ClipRect.right) - x) > destwidth)
            newwidth = destwidth;
        if (y < border)
        {
            newheight = destheight - (border - y);
            srcy += (border - y);
            y = border;
        }
        else if ((newheight = (usebitmaphwnd? bitmap->ClipRectAddr->bottom : win->ClipRect.bottom) - y) > destheight)
            newheight = destheight;

        BitBlt(hdc, x, y, newwidth, newheight, GlobalMemDC2, srcx, srcy, SRCPAINT);
    }
    SelectObject (GlobalMemDC2, GlobalOldMemDC2Bitmap);   //So we don't crunch on the next line...
    DeleteObject (newbmp);

    if (!sdb || !doubleBuffering || !ingame)
    {
    	if (usebitmaphwnd)
    	    ReleaseDC (bitmap->hwnd, hdc);
    	else
            ReleaseDC (win->hwnd, hdc);
    }
/*  Alternative method using PlgBlt, left in for reference purposes
    
    P[0].x = (long)(x + xscale - xscale*cos(angle) + yscale*sin(angle));
    P[0].y = (long)(y + yscale - yscale*cos(angle) - xscale*sin(angle));
    P[1].x = (long)(x + xscale - xscale*cos(angle+PI/2) + yscale*sin(angle+PI/2));
    P[1].y = (long)(y + yscale - yscale*cos(angle+PI/2) - xscale*sin(angle+PI/2));
    P[2].x = (long)(x + xscale - xscale*cos(angle-PI/2) + yscale*sin(angle-PI/2));
    P[2].y = (long)(y + yscale - yscale*cos(angle-PI/2) - xscale*sin(angle-PI/2));

    PlgBlt (hdc, P, GlobalMemDC, srcx, srcy, width, height, 0, 0, 0 );
*/
}

void
W_WriteBitmapGrey (int x,
                   int y,
                   W_Icon icon,
                   W_Color color,
                   W_Window window)
{
    register int border, width, height;
    register int srcx, srcy;
    HBITMAP newcopy;
    int i, colorsum;
    RGBQUAD rgbq[257];
    DBICONHEADER_VOID;

    //Fast (I hope) rectangle intersection, don't overwrite our borders
    srcx = bitmap->x;
    srcy = bitmap->y;
    if (usebitmaphwnd)
        border = bitmap->ClipRectAddr->top;
    else
        border = win->ClipRect.top;
    x += border;
    y += border;
    if (x < border)
    {
        width = bitmap->width - (border - x);
        srcx += border - x;
        x = border;
    }
    else if ((width = (usebitmaphwnd ? bitmap->ClipRectAddr->right : win->ClipRect.right) - x) > bitmap->width)
        width = bitmap->width;
    if (y < border)
    {
        height = bitmap->height - (border - y);
        srcy += (border - y);
        y = border;
    }
    else if ((height = (usebitmaphwnd ? bitmap->ClipRectAddr->bottom : win->ClipRect.bottom) - y) > bitmap->height)
        height = bitmap->height;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }

    newcopy = CopyImage (bitmap->bm, IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

    SelectObject (GreyBitmapDC, newcopy);

    GetDIBColorTable (GreyBitmapDC, 0, 256, (LPRGBQUAD) rgbq);
    for (i = 0; i < 256; i++)
    {
        colorsum = rgbq[i].rgbBlue + rgbq[i].rgbRed + rgbq[i].rgbGreen;
        if (colorsum != 0)
            colorsum = (colorsum / 3 + 96);
        if (colorsum > 255)
            colorsum = 255;
        rgbq[i].rgbRed = (unsigned char) colorsum;
        rgbq[i].rgbBlue = (unsigned char) colorsum;
        rgbq[i].rgbGreen = (unsigned char) colorsum;
    }

    SetDIBColorTable (GreyBitmapDC, 0, 256, (LPRGBQUAD) rgbq);


    //Set the color of the bitmap
    //(oddly enough, 1-bit = bk color, 0-bit = text (fg) color)
//   SetBkColor(hdc, colortable[BLACK].rgb);
//   SetTextColor(hdc, colortable[WHITE].rgb);

    BitBlt (hdc, x, y,          //Copy the bitmap
            width, height, GreyBitmapDC, srcx, srcy, SRCPAINT); // <-- using OR mode
    
    DeleteObject (newcopy);
    
    if (!sdb || !doubleBuffering || !ingame)
    {
    	if (usebitmaphwnd)
    	    ReleaseDC (bitmap->hwnd, hdc);
    	else
            ReleaseDC (win->hwnd, hdc);
    }
}


void
W_TileWindow (W_Window window,
              W_Icon icon)
{
    FNHEADER_VOID;

    win->tiled = TRUE;
    win->TileIcon = (struct Icon *) icon;

    InvalidateRect (win->hwnd, NULL, TRUE);
    UpdateWindow (win->hwnd);
}

void
W_UnTileWindow (W_Window window)
{
    FNHEADER_VOID;

    if (!win->tiled)
        return;

    win->tiled = FALSE;

    InvalidateRect (win->hwnd, NULL, TRUE);
    UpdateWindow (win->hwnd);
}


int
W_WindowWidth (W_Window window)
{
    FNHEADER;

    return (win->ClipRect.right - win->border);
}

int
W_WindowHeight (W_Window window)
{
    FNHEADER;

    return (win->ClipRect.bottom - win->border);
}

//We fake window events as a socket, by using this magic number
//and a fudge in our implementation of select()... 
int
W_Socket ()
{
    return (0x0CABBABE);        //Magic number, can be anything but must match winmain.c's select()
}

void
W_Beep ()
{
    MessageBeep (MB_ICONEXCLAMATION);
}

//Easy enough... except for the fact that only the thread which created the window
// can destroy it. So if we are not the main thread we post a message
void
W_DestroyWindow (W_Window window)
{
    FNHEADER_VOID;
    if (GetCurrentThreadId () == MainThreadID)
        DestroyWindow (win->hwnd);
    else
        PostMessage (AnyWindow, WM_CROSS_THREAD_DESTROY, (WPARAM) win->hwnd,
                     0);
}

//Redraw a menu window.
void
RedrawMenu (Window * win,
            HDC hdc)
{
    int count;
    RECT r;

    SelectObject (hdc, (HFONT) W_RegularFont);

    r.left = win->border;
    r.right = win->ClipRect.right;

    for (count = 1; count < win->NumItems; count++)
    {
        r.top =
            count * (W_Textheight + MENU_PAD * 2) + (count - 1) * MENU_BAR +
            win->border;
        r.bottom = r.top + MENU_BAR;
        FillRect (hdc, &r, colortable[W_White].brush);
    }

    SetBkMode (hdc, TRANSPARENT);

    for (count = 0; count < win->NumItems; count++)
        if ((win->items[count]).string)
        {
            SetTextColor (hdc, colortable[(win->items[count]).color].rgb);
            TextOut (hdc, WIN_EDGE + win->border,
                     count * (W_Textheight + MENU_PAD * 2 + MENU_BAR) +
                     MENU_PAD + win->border, win->items[count].string,
                     win->items[count].len);
        }
}

//Change a specific menu item, and redraw.
void
ChangeMenuItem (Window * win,
                int n,
                char *str,
                int len,
                W_Color color)
{
    register struct menuItem *p = &(win->items[n]);
    HDC hdc;
    RECT r;

    if (n >= win->NumItems)
        return;

    if (p->string)
    {
        free (p->string);       // Free memory
    }

    p->string = (char *) malloc (len + 1);

    strncpy (p->string, str, len);
    p->color = color;
    p->len = (short) len;

    hdc = GetDC (win->hwnd);
    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }

    r.left = win->ClipRect.left;
    r.right = win->ClipRect.right;
    r.top =
        n * (W_Textheight + MENU_PAD * 2 + MENU_BAR) + MENU_PAD + win->border;
    r.bottom = r.top + W_Textheight;
    FillRect (hdc, &r, colortable[W_Black].brush);

    SetBkMode (hdc, TRANSPARENT);
    SelectObject (hdc, (HFONT) W_RegularFont);

    if (p->string)
    {
        SetTextColor (hdc, colortable[p->color].rgb);
        TextOut (hdc, WIN_EDGE + win->border,
                 n * (W_Textheight + MENU_PAD * 2 + MENU_BAR) + MENU_PAD +
                 win->border, p->string, p->len);
    }

    DrawBorder (win, hdc);
    ReleaseDC (win->hwnd, hdc);
}


void
AddToScrolling (Window * win,
                W_Color color,
                char *str,
                int len)
{
    struct stringList *p = win->strings;
    struct stringList *end, *p2;
    int NumStrings = win->NumItems;

    //Find the end of the linked-list of strings...
    if (p)                      // ...if the list has been created
    {
        end = p;
        while (end->next)
            end = end->next;
    }
    else
    {
        end = 0;
        NumStrings = 0;
    }

    if (NumStrings < maxScrollLines)    //Create a new stringList item
    {
        p2 = (struct stringList *) malloc (sizeof (struct stringList));
        if (!p2)
        {
            LineToConsole ("Not enough memory to allocate a new listbox string.\n");
            return;
        }

        if (!p)
            win->strings = p2;  //Store the new start of list

        p = p2;                 //Point p to the new string

        win->NumItems = (short) (++NumStrings);   //Inc the string number
    }
    else                        //Re-use the first string, place it at the end of the list
    {
        if (p->string)
            free (p->string);
        win->strings = p->next; //Store the new start of list
    }

    if (str[len - 1] == '\n')   //Remove trailing newlines
        str[--len] = 0;

    p->string = (char *) malloc (win->TextWidth + 1);
    if (!p->string)
    {
        LineToConsole ("Not enough memory to allocate a new listbox string.");
        return;
    }

    p->color = color;
    strncpy (p->string, str, win->TextWidth);

    /* we pad out the string with spaces so we don't have to clear
       the window */
    if (len < win->TextWidth - 1)
        memset (p->string + len, ' ', win->TextWidth - len - 1);
    p->string[win->TextWidth - 1] = 0;

    if (end)
        end->next = p;
    p->next = 0;

    //Mark as out of date...
    win->AddedStrings++;
}


//Redraw a scrolling window
void
RedrawScrolling (Window * win,
                 HDC hdc)
{
    struct stringList *p = win->strings;
    int NumStrings = win->NumItems;
    int HiddenLines;
    int i;

    if (!win->AddedStrings && !win->NumItems)
        return;

    if (win->AddedStrings)      //If strings were added since last paint,
    {                           //Turn off clipping == FULL redraw
        i = win->NumItems - win->TextHeight;
        i = max (0, i);
        if ((i - win->AddedStrings) > GetScrollPos (win->hwnd, SB_VERT))
        {
            // The player is looking back in the list. Don't move things on him!
            HiddenLines = GetScrollPos (win->hwnd, SB_VERT);
            win->AddedStrings = 0;
            SetScrollRange (win->hwnd, SB_VERT, 0, i, TRUE);
        }
        else
        {
            // We are at then end away, scroll away
            SelectClipRgn (hdc, NULL);
            win->AddedStrings = 0;
            SetScrollRange (win->hwnd, SB_VERT, 0, i, FALSE);
            //update the scrollbar range
            SetScrollPos (win->hwnd, SB_VERT, i, TRUE);
            //update the scrollbar pos and redraw the scroll bar
            HiddenLines = i;
        }
    }
    else
    {
        HiddenLines = GetScrollPos (win->hwnd, SB_VERT);
    }

    //Advance to the first visible line
    NumStrings -= HiddenLines;
    while (HiddenLines--)
        p = p->next;

    //Setup the HDC
    SelectObject (hdc, (HFONT) W_RegularFont);
    SetBkMode (hdc, OPAQUE);
    SetBkColor (hdc, colortable[W_Black].rgb);

    //Draw each line of text
    if (NumStrings > win->TextHeight)
        NumStrings = win->TextHeight;
    for (i = win->TextHeight - NumStrings; i < win->TextHeight && p; i++)
    {
        SetTextColor (hdc, colortable[p->color].rgb);
        TextOut (hdc, WIN_EDGE, MENU_PAD + i * W_Textheight, p->string,
                 strlen (p->string));
        p = p->next;
    }

}

void
W_FlushScrollingWindow (W_Window window)
{
    HDC hdc;
    struct stringList *p;
    int HiddenLines;
    int y;
    int clipped = 0;
    int totally_clipped = 0;
    RECT r;
    WINDOWPLACEMENT loc;
    FNHEADER_VOID;

    if (!win->AddedStrings)
        return;

    y = win->NumItems - win->TextHeight;
    y = max (0, y);
 
    loc.length = sizeof (WINDOWPLACEMENT); /* Have to set this */

    // Just update scrollbar if we are looking back
    if ((y - win->AddedStrings) > GetScrollPos (win->hwnd, SB_VERT))
    {
        SetScrollRange (win->hwnd, SB_VERT, 0, y, TRUE);
        win->AddedStrings = 0;
        return;
    }
    // For mapped windows, do full redraw if more than half of window content
    // is changed, or if part of window is outside viewable area, because
    // ScrollDC fails to properly scroll text in such cases
    if (W_IsMapped (window) && W_IsMapped (baseWin))
    {
    	GetWindowPlacement (win->hwnd, &loc);
    	GetWindowRect (((Window *) baseWin)->hwnd, &r);
    	// At least partially clipped
    	if (loc.rcNormalPosition.left < r.left || loc.rcNormalPosition.right > r.right
    	|| loc.rcNormalPosition.top < r.top || loc.rcNormalPosition.bottom > r.bottom)
    	    clipped = 1;
    	// Totally clipped, treat like unmapped window
    	if (loc.rcNormalPosition.right < r.left || loc.rcNormalPosition.left > r.right
    	|| loc.rcNormalPosition.bottom < r.top || loc.rcNormalPosition.top > r.bottom)
    	    totally_clipped = 1;
    	if (win->AddedStrings > (win->TextHeight / 2) || (clipped && !totally_clipped))
    	{
            InvalidateRect (win->hwnd, NULL, FALSE);
            UpdateWindow (win->hwnd);       //Generates paint msg, which calls RedrawScrolling
            return;
        }
    }

    //Setup the HDC
    hdc = GetDC (win->hwnd);
    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SelectObject (hdc, (HFONT) W_RegularFont);
    SetBkColor (hdc, colortable[W_Black].rgb);
    SetBkMode (hdc, OPAQUE);

    //update the scrollbar range and position
    SetScrollRange (win->hwnd, SB_VERT, 0, y, FALSE);
    SetScrollPos (win->hwnd, SB_VERT, y, TRUE);

    //Scroll up however many lines. Use ScrollDC so we don't invalidate the window
    y = win->TextHeight - win->AddedStrings;

    if (y < 0)                  //Pathalogical case (but it does happen):
    {                           //First Flush() call has more lines added than will fit
        win->AddedStrings += y; //in one window height
        y = 0;
    }
    else
        ScrollDC (hdc, 0, -win->AddedStrings * W_Textheight, &win->ClipRect,
                  &win->ClipRect, NULL, NULL);

    //Advance to the first visible line
    p = win->strings;
    HiddenLines = max (0, win->NumItems - win->AddedStrings);
    for (; HiddenLines && p; HiddenLines--)
        p = p->next;

    //Draw each line of text
    y = y * W_Textheight + MENU_PAD;
    while (win->AddedStrings && p)
    {
        SetTextColor (hdc, colortable[p->color].rgb);
        TextOut (hdc, WIN_EDGE, y, p->string, win->TextWidth);
        p = p->next;
        y += W_Textheight;
        win->AddedStrings--;
    }

    win->AddedStrings = 0;

    DrawBorder (win, hdc);
    ReleaseDC (win->hwnd, hdc);
}

void
W_DefineMapcursor (W_Window window)
{
    FNHEADER_VOID;

    if (win->UsingCreatedCursor)
    {
        DestroyCursor (win->cursor);
        win->UsingCreatedCursor = FALSE;
    }

    win->cursor = MapCursor;

}

void
W_DefineLocalcursor (W_Window window)
{
    FNHEADER_VOID;

    if (win->UsingCreatedCursor)
    {
        DestroyCursor (win->cursor);
        win->UsingCreatedCursor = FALSE;
    }

    win->cursor = LocalCursor;

}


//Sets the cursor to a Star-Trek badge
void
W_DefineTrekCursor (W_Window window)
{
    FNHEADER_VOID;

    if (win->UsingCreatedCursor)
    {
        DestroyCursor (win->cursor);
        win->UsingCreatedCursor = FALSE;
    }

    win->cursor = TrekCursor;
}

//Sets the cursor to an exclamation mark
void
W_DefineWarningCursor (W_Window window)
{
    FNHEADER_VOID;

    if (win->UsingCreatedCursor)
    {
        DestroyCursor (win->cursor);
        win->UsingCreatedCursor = FALSE;
    }

    win->cursor = WarnCursor;
}


//Sets the cursor to the ol' boring arrow...
void
W_DefineArrowCursor (W_Window window)
{
    FNHEADER_VOID;

    if (win->UsingCreatedCursor)
    {
        DestroyCursor (win->cursor);
        win->UsingCreatedCursor = FALSE;
    }

    win->cursor = LoadCursor (NULL, IDC_ARROW);
}

//Sets the cursor to a an I-beam, indicating text insertion
void
W_DefineTextCursor (W_Window window)
{
    FNHEADER_VOID;

    if (win->UsingCreatedCursor)
    {
        DestroyCursor (win->cursor);
        win->UsingCreatedCursor = FALSE;
    }

    win->cursor = LoadCursor (NULL, IDC_IBEAM);
}




/***************************************************************************/
/* Looks up any default geometry specified in the defaults file and        */
/* returns the values found there.  Geometry should be of the form         */
/* 502x885+1+1, 502x885, or +1+1.  This allows width or width and height   */
/* or width height and x, or width height x and y, or x or x and y. Note   */
/* that height or y can not be specified unless width and x are also       */
/* specified.                                                              */
/*                                                                         */
/*   The result returned indicates which values were set.                  */
/*                                                                         */
/* result & 0x1 ----> width set        G_SET_WIDTH                         */
/* result & 0x2 ----> height set       G_SET_HEIGHT                        */
/* result & 0x4 ----> x set            G_SET_X                             */
/* result & 0x8 ----> y set            G_SET_Y                             */
/***************************************************************************/
/* Hacked to handle negative geometries 21 Aug 96 -SAC */
#define G_SET_WIDTH 0x1
#define G_SET_HEIGHT 0x2
#define G_SET_X 0x4
#define G_SET_Y 0x8
int
checkGeometry (char *name,
               int *x,
               int *y,
               int *width,
               int *height)
{
    char *geom_default;
    char buf[100];
    char *s;
    int result = 0;

    sprintf (buf, "%s.geometry", name);
    geom_default = geometryDefault (buf);
    if (!geom_default)
        return 0;               /* nothing set */
    s = geom_default;
    if (*s != '+' && *s != '-') /* width and height at least */
    {
        while (*s != 'x' && *s != 0)
            s++;
        *width = atoi (geom_default);
        if (!strcmp("local", name))
        {
            TWINSIDE = *width;
            /* Sanity checks .. */
            if (TWINSIDE > 5000)
                TWINSIDE = 5000;
        }
        else if (!strcmp("map", name))
        {
            GWINSIDE = *width;
            if (GWINSIDE > 5000)
                GWINSIDE = 5000;
        }
        result |= G_SET_WIDTH;
        if (*s == 0)
            return result;
        s++;
        geom_default = s;
        while (*s != '+' && *s != '-' && *s != 0)
            s++;
        *height = atoi (geom_default);
        if (!strcmp("local", name))
        {
            if (*height > *width)
            {
                *width = *height;
                TWINSIDE = *height;
                if (TWINSIDE > 5000)
                    TWINSIDE = 5000;
            }
        }
        else if (!strcmp("map", name))
        {
            if (*height > *width)
            {
                *width = *height;
                GWINSIDE = *height;
                if (GWINSIDE > 5000)
                    GWINSIDE = 5000;
            }
        }
        result |= G_SET_HEIGHT;
        if (*s == 0)
            return result;
    }
    s++;
    geom_default = s;
    while (*s != '+' && *s != '-' && *s != 0)
        s++;
    *x = atoi (geom_default);
    if (*(geom_default - 1) == '-')
        *x = -(*x);
    result |= G_SET_X;
    if (*s == 0)
        return result;
    s++;
    geom_default = s;
    *y = atoi (geom_default);
    if (*(geom_default - 1) == '-')
        *y = -(*y);
    result |= G_SET_Y;
    return result;
}



//Should this window be initially mapped?
int
checkMapped (char *name)
{
    char buf[100];

    sprintf (buf, "%s.mapped", name);
    return (booleanDefault (buf, 0));
}

int
checkMappedPref (char *name, int preferred)
{
    char buf[100];

    sprintf (buf, "%s.mapped", name);
    return (booleanDefault (buf, preferred));
}

//Move the mouse cursor to the new window, or back to old pos if window==NULL
void
W_WarpPointer (W_Window window)
{
    static POINT warped_from;
    static int W_in_message = 0;

    if (window == NULL)
    {
        if (W_in_message)
        {
            SetCursorPos (warped_from.x, warped_from.y);
            W_in_message = 0;
        }
    }
    else
    {
        RECT r;
        GetWindowRect (((Window *) window)->hwnd, &r);
        GetCursorPos (&warped_from);
        SetCursorPos ((r.left + r.right) / 2, (r.top + r.bottom) / 2);
        W_in_message = 1;
    }
}


//An un-documented function that seems to be needed anyway
//Checks if the mouse is in the given window - and if so, where?
int
findMouseInWin (int *x,
                int *y,
                W_Window window)
{
    POINT p;
    HWND hwnd;
    FNHEADER;

    GetCursorPos (&p);
    hwnd = ChildWindowFromPoint (((Window *) baseWin)->hwnd, p);
    if (!hwnd || (W_Window) GetWindowLongPtr (hwnd, GWLP_USERDATA) != window)
    {
        *x = 0;
        *y = 0;
        return 0;
    }

    ScreenToClient (hwnd, &p);
    *x = p.x;
    *y = p.y;
    return 1;
}


void
DrawBorder (Window * win,
            HDC hdc)
{
    RECT r;
    register int size = win->border;

    GetClientRect (win->hwnd, &r);
    SelectObject (hdc, colortable[win->BorderColor].pen);
    SelectObject (hdc, GetStockObject (NULL_BRUSH));

    for (; size; size--)        //Draw a series of shrinking rectangles
    {
        Rectangle (hdc, r.left, r.top, r.right, r.bottom);
        r.left++;
        r.top++;
        r.bottom--;
        r.right--;
    }

}


//Converts a NxM X11 cursor bitmap, where N<=32 && M<=32 into a 32 x 32 Windows cursor bitmap
//To expand the size, it simply places the NxM in the upper left corner
//of the 32x32
unsigned char *
X11toCursor (unsigned char *bits,
             int width,
             int height)
{
    if (width > 32 || height > 32)
    {
        LineToConsole ("Attempt to define cursor > 32x32. Cursor will be garbled!");
        width = min (32, width);
        height = min (32, height);
    }
    return X11toDIBAndMirror (bits, width, height, 32, 32);
}


//Convert X11-style packed monochrome bitmap information to a Windows monochrome DIB
//Picks the smallest size that will fit the original X-bitmap (X-bitmaps are byte-padded
//while Windows bitmaps are word-padded). Does not mirror the bitmap  --  we use StretchBlt
//with negative width when we copy into the initial memory DC during StoreBitmap (faster method)
unsigned char *
X11toDIB (unsigned char *bits,
          int width,
          int height)
{
    int outbytewidth = ((width + 0xF) & ~0xF) / 8;      //width of a line in bytes
    int inbytewidth = (width + 0x7) / 8;
    int i;
    register int j;
    register unsigned char *data;
    unsigned char *base = (unsigned char *) malloc (outbytewidth * height);     //malloc decode space
    if (!base)
        return NULL;

    memset (base, 0, outbytewidth * height);    //Clear the array

    for (i = 0; i < height; i++)
    {
        data = base + ((i + 1) * outbytewidth);
        for (j = 0; j < inbytewidth; j++)
            *--data = *bits++;
    }

    return base;
}


//Converts X-style monochrome bitmap to GDI usable format
//and mirrors the image (As X11 bmps are right-to-left). Used for
//defining cursors.
unsigned char *
X11toDIBAndMirror (unsigned char *bits,
                   int width,
                   int height,
                   int outwidth,
                   int outheight)
{
    register unsigned char rot, rot2;   //Need lotsa register vars
    register unsigned char *data;       //for this to run fast
    register int j, i;

    int bytewidth = outwidth / 8;       //width of a line in bytes
    unsigned char *base = (unsigned char *) malloc (bytewidth * outheight);     //malloc decode space
    if (!base)
        return NULL;

    memset (base, 0, bytewidth * outheight);    //Clear the array

    //Perform the actual conversion. X11 bitmaps are stored in
    //reverse bit-order (i.e. 0xC0 when the image is actually 0x03)
    for (i = 0; i < height; i++)        //For each line in height...
    {
        data = base + (i * bytewidth);  //Get start of line in dest
        rot = 0x01;             //Mask = 00000001b
        rot2 = 0x80;            //Mask = 10000000b
        for (j = 0; j < width; j++)     //For each pixel in width...
        {
            if (*bits & rot)    //If bit is set in source,
                *data |= rot2;  // or dest with the other mask.

            rot <<= 1;          //Shift masks in opposite directions
            rot2 >>= 1;
            if (!rot2)          //If we have done 8 shifts,
            {
                rot = 0x01;     // reset masks,
                rot2 = 0x80;
                bits++;         // inc source and dest ptrs.
                data++;
            }
        }
        if (rot != 0x1)         //If we didn't process the whole byte,
            bits++;             //inc source ptr
    }

    return base;
}

#define MAKE_WINDOW_GETTER(name, part) \
 W_Callback name(W_Window w) \
  { \
  return ( w ? ((Window *)w)->part : NULL );\
  }

#define MAKE_WINDOW_SETTER(name, part) \
 W_Callback name(W_Window w, W_Callback c) \
  { \
  if (w)\
   ((Window *)w)->part = c;\
  return c;\
  }

MAKE_WINDOW_GETTER (W_GetWindowKeyDownHandler, HandleKeydown);
MAKE_WINDOW_SETTER (W_SetWindowKeyDownHandler, HandleKeydown);

MAKE_WINDOW_GETTER (W_GetWindowKeyUpHandler, HandleKeyup);
MAKE_WINDOW_SETTER (W_SetWindowKeyUpHandler, HandleKeyup);

MAKE_WINDOW_GETTER (W_GetWindowButtonHandler, HandleButton);
MAKE_WINDOW_SETTER (W_SetWindowButtonHandler, HandleButton);

MAKE_WINDOW_GETTER (W_GetWindowExposeHandler, HandleExpose);
MAKE_WINDOW_SETTER (W_SetWindowExposeHandler, HandleExpose);


void
W_ResizeWindow (W_Window window,
                int neww,
                int newh)       /* TSH 2/93 */
{
    if (!window)
        return;
    SetWindowPos (((Window *) window)->hwnd, NULL, 0, 0, neww, newh,
                  SWP_NOMOVE);
}

void
W_ResizeTextWindow (W_Window window,
                    int neww,
                    int newh)   /* TSH 2/93 */
{
    FNHEADER_VOID;

    if (!window)
        return;
    win->TextHeight = (short) newh;
    win->TextWidth = (short) neww;
    W_ResizeWindow (window, neww * W_Textwidth + WIN_EDGE * 2,
                    newh * W_Textheight + WIN_EDGE * 2);
}

void
W_ResizeMenu (W_Window window,
              int neww,
              int newh)         /* TSH 2/93 */
{
    FNHEADER_VOID;

    win->NumItems = (short) newh;
    win->TextHeight = (short) newh;
    win->TextWidth = (short) neww;

    W_ResizeWindow (window, neww * W_Textwidth + WIN_EDGE * 2,
                    newh * (W_Textheight + MENU_PAD * 2) + (newh -
                                                            1) * MENU_BAR);
}

//Is this a mono display?
int
W_Mono ()
{
    HDC hdc = GetDC (HWND_DESKTOP);
    register int result =
        ((GetDeviceCaps (hdc, PLANES) == 1)
         && (GetDeviceCaps (hdc, COLORRES) == 1));
    ReleaseDC (HWND_DESKTOP, hdc);
    return result;
}


#ifdef  SHORT_PACKETS
/* Apparently this solves some problem on X, but here it just forces a redraw...*/
/* Hrm - This was using redrawScrolling(win), updated to (win, hdc) -SAC 24 Jul 96 */
/* Was void W_SetSensitive(Window *window, int b) -SAC */
void
W_SetSensitive (Window * window,
                int b)
{
    HDC hdc;
    FNHEADER_VOID;

    b;                          /* Makes compiler happy -SAC */
    if (win->type == WIN_SCROLL)
    {
        hdc = GetDC (((Window *)win)->hwnd);
        if (hdc)
            RedrawScrolling (win, hdc);
        ReleaseDC (((Window *)win)->hwnd, hdc);
    }
}
#endif

// Uses SRCAND for raster operation - useful for masking
void
W_OverlayBitmap (int x,
                 int y,
                 W_Icon icon,
                 W_Color color,
                 W_Window window)
{
    register int border, width, height;
    register int srcx, srcy;
    DBICONHEADER_VOID;

    //Fast (I hope) rectangle intersection, don't overwrite our borders
    srcx = bitmap->x;
    srcy = bitmap->y;
    if (usebitmaphwnd)
        border = bitmap->ClipRectAddr->top;
    else
        border = win->ClipRect.top;
    x += border;
    y += border;
    if (x < border)
    {
        width = bitmap->width - (border - x);
        srcx += border - x;
        x = border;
    }
    else if ((width = (usebitmaphwnd ? bitmap->ClipRectAddr->right : win->ClipRect.right) - x) > bitmap->width)
        width = bitmap->width;
    if (y < border)
    {
        height = bitmap->height - (border - y);
        srcy += (border - y);
        y = border;
    }
    else if ((height = (usebitmaphwnd ? bitmap->ClipRectAddr->bottom : win->ClipRect.bottom) - y) > bitmap->height)
        height = bitmap->height;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    SelectObject (GlobalMemDC, bitmap->bm);

    //Set the color of the bitmap
    //(oddly enough, 1-bit = bk color, 0-bit = text (fg) color)
    SetBkColor (hdc, colortable[color].rgb);
    SetTextColor (hdc, colortable[BLACK].rgb);

    BitBlt (hdc, x, y, width, height, GlobalMemDC, srcx, srcy, SRCAND);

    if (!sdb || !doubleBuffering || !ingame)
    {
    	if (usebitmaphwnd)
    	    ReleaseDC (bitmap->hwnd, hdc);
    	else
            ReleaseDC (win->hwnd, hdc);
    }
}

/* Copy over destination imagemap, but make any black in source image transparent */
void
W_OverlayScaleBitmap (int x,
                      int y,
                      int destwidth,
                      int destheight,
                      int srcwidth,
                      int srcheight,
                      int angle,
                      W_Icon icon,
                      W_Color color,
                      W_Window window)
{
    register int border;
    register int srcx, srcy;
    HBITMAP newbmp;
    XFORM xForm;
    int newwidth, newheight;
    double radians;
    float cosine, sine, xscale, yscale, eDx, eDy;
    DBICONHEADER_VOID;

    // First copy bitmap into new bitmap, and scale it.  This makes life
    // easier for doing border intersection
    srcx = bitmap->x;
    srcy = bitmap->y;

    newbmp = CreateCompatibleBitmap ( hdc, destwidth, destheight );
    
    SelectObject (GlobalMemDC, bitmap->bm);
    SelectObject (GlobalMemDC2, newbmp);

    // Need to setup colors and stuff
    if (NetrekPalette)
    {
        SelectPalette (GlobalMemDC2, NetrekPalette, FALSE);
        RealizePalette (GlobalMemDC2);
    }

    SetBkColor (GlobalMemDC2, colortable[color].rgb);
    SetTextColor (GlobalMemDC2, colortable[BLACK].rgb);

    // Copy selected section of main bitmap into newbmp before rotation
    SetStretchBltMode(GlobalMemDC2, COLORONCOLOR);
    StretchBlt(GlobalMemDC2, 0, 0, destwidth, destheight, GlobalMemDC,
               srcx, srcy, srcwidth, srcheight, SRCCOPY);

    //Fast (I hope) rectangle intersection, don't overwrite our borders
    if (usebitmaphwnd)
        border = bitmap->ClipRectAddr->top;
    else
        border = win->ClipRect.top;
    
    // Reset srcx and srcy as our new source bitmap starts at 0,0
    srcx = 0;
    srcy = 0;
    x += border;
    y += border;

    if (NetrekPalette)
    {
        SelectPalette (hdc, NetrekPalette, FALSE);
        RealizePalette (hdc);
    }
    //Set the color of the bitmap
    //(oddly enough, 1-bit = bk color, 0-bit = text (fg) color)
    SetBkColor (hdc, colortable[color].rgb);
    SetTextColor (hdc, colortable[BLACK].rgb);
    
    if (angle != 0)
    {
        //Convert angle to radians 
        radians=(2*3.14159*angle/360);

        cosine=(float)cos(radians);
        sine=(float)sin(radians);

        newwidth = destwidth;
        newheight = destheight;

        // Scale used to find bitmap center
        xscale = (float)(destwidth/2);
        yscale = (float)(destheight/2);

        eDx = x + xscale - cosine*(xscale) + sine*(yscale);
        eDy = y + yscale - cosine*(yscale) - sine*(xscale);
        SetGraphicsMode(hdc,GM_ADVANCED);

        xForm.eM11=cosine;
        xForm.eM12=sine;
        xForm.eM21=-sine;
        xForm.eM22=cosine;
        xForm.eDx = eDx;
        xForm.eDy = eDy;

        SetWorldTransform(hdc,&xForm);
        TransparentBlt(hdc, 0, 0, newwidth, newheight, GlobalMemDC2, srcx, srcy, newwidth, newheight, colortable[BLACK].rgb);
    
        // Reset xForm
        xForm.eM11 = (FLOAT) 1.0; 
        xForm.eM12 = (FLOAT) 0.0; 
        xForm.eM21 = (FLOAT) 0.0; 
        xForm.eM22 = (FLOAT) 1.0; 
        xForm.eDx  = (FLOAT) 0.0; 
        xForm.eDy  = (FLOAT) 0.0; 

        SetWorldTransform(hdc,&xForm); 
        SetGraphicsMode(hdc,GM_COMPATIBLE);
    }
    else
    {
    	// Calculate border constraints with bitmap width/height
        if (x < border)
        {
            newwidth = destwidth - (border - x);
            srcx += border - x;
            x = border;
        }
        else if ((newwidth = (usebitmaphwnd ? bitmap->ClipRectAddr->right : win->ClipRect.right) - x) > destwidth)
            newwidth = destwidth;
        if (y < border)
        {
            newheight = destheight - (border - y);
            srcy += (border - y);
            y = border;
        }
        else if ((newheight = (usebitmaphwnd? bitmap->ClipRectAddr->bottom : win->ClipRect.bottom) - y) > destheight)
            newheight = destheight;

        TransparentBlt(hdc, x, y, newwidth, newheight, GlobalMemDC2, srcx, srcy, newwidth, newheight, colortable[BLACK].rgb);
    }

    SelectObject (GlobalMemDC2, GlobalOldMemDC2Bitmap);   //So we don't crunch on the next line...
    DeleteObject (newbmp);
    
    if (!sdb || !doubleBuffering || !ingame)
    {
    	if (usebitmaphwnd)
    	    ReleaseDC (bitmap->hwnd, hdc);
    	else
            ReleaseDC (win->hwnd, hdc);
    }
}

#ifdef BEEPLITE

void W_WriteTTSText(W_Window window, int max_width, int y, char *str, int len)
{
    register int x;
    register int border;
    SIZE ext;
    DBHEADER_VOID;

    y -= W_Textheight;
    border = win->border;
    
    if (NetrekPalette)
    {
        SelectPalette(hdc, NetrekPalette, FALSE);
        RealizePalette(hdc);
    }
  
    SetTextColor(hdc, colortable[GREY].rgb);
    SetBkMode(hdc, TRANSPARENT);
    SelectObject (hdc, (HFONT) W_HighlightFont);
    GetTextExtentPoint32 (hdc, str, len, &ext);
    x = (max_width - ext.cx)/2; 
    x += border;
    y += border;

    ExtTextOut (hdc, x, y, ETO_CLIPPED | ETO_OPAQUE, NULL, str, len, NULL);
    last_tts_xpos = x - border;
    last_tts_width = ext.cx;

    if (!sdb || !doubleBuffering || !ingame)
        ReleaseDC (win->hwnd, hdc);
}
#endif

void
W_SetWindowName (W_Window window,
                 char *name)
{
    FNHEADER_VOID;

    SetWindowText (win->hwnd, name);
    return;
}

void
W_ResizeMenuToNumItems (W_Window window, int numitems)
{
    WINDOWPLACEMENT loc;

    FNHEADER_VOID;

    if (win->type != WIN_MENU)
        return;

    if (!GetWindowPlacement (win->hwnd, &loc))
        return;

    loc.rcNormalPosition.bottom = numitems * (W_Textheight + MENU_PAD * 2) + (numitems - 1) *
                                    MENU_BAR + win->parent->border +
                                    loc.rcNormalPosition.top;

    SetWindowPlacement (win->hwnd, &loc);
}

void
W_SetWAM (W_Window window)
{
    char str[64], *tmp;
    unsigned int i;

    FNHEADER_VOID;

    /* First of all let's set defaults for all windows */
    if (strcmpi (win->name, "review") == 0)
        win->wam = WAM_ALL | WAM_INDIV | WAM_TEAM | WAM_KILL | WAM_PHASER;
    else if (strcmpi (win->name, "review_all") == 0)
        win->wam = WAM_ALL;
    else if (strcmpi (win->name, "review_your") == 0)
        win->wam = WAM_INDIV;
    else if (strcmpi (win->name, "review_team") == 0)
        win->wam = WAM_TEAM;
    else if (strcmpi (win->name, "review_kill") == 0)
        win->wam = WAM_KILL;
    else if (strcmpi (win->name, "review_phaser") == 0)
        win->wam = WAM_PHASER;

    sprintf (str, "%s.allow", win->name);
    if ((tmp = stringDefault (str)) != NULL)
    {
        /* We got something in window.allow, so let's clear default WAM flags */
        win->wam = WAM_NONE;
        for (i = 0; i < strlen (tmp); i++)
        {
            switch (tmp[i])
            {
            case 'I':
            case 'i':
                win->wam |= WAM_INDIV;
                break;
            case 'T':
            case 't':
                win->wam |= WAM_TEAM;
                break;
            case 'K':
            case 'k':
                win->wam |= WAM_KILL;
                break;
            case 'A':
            case 'a':
                win->wam |= WAM_ALL;
                break;
            case 'P':
            case 'p':
                win->wam |= WAM_PHASER;
                break;
            }
        }
    }
}


void
W_MessageAllowedWindows (int messtype, int x, int y, W_Color color, char *str, int len, 
                         W_Font font)
{
    int i;
    Window * win;

    
    for (i = 0; i < 6; i++)
    {
        win = (Window *)wam_windows[i];
        if (win->wam & messtype)
            W_WriteText (wam_windows[i], x, y, color, str, len, font);
    }
}

//////////////////////////////////////////////////////////////////////////////////
// Reset the system colors. inlines >> macros! -SAC

inline void
ResetSysColors (void)
{
    if (mungScrollbarColors = booleanDefault ("mungScrollbarColors", 0))
        SetSysColors (sizeof (SysColorNames), SysColorNames, SysColors);
    return;
}

inline void
SetTrekSysColors (void)
{
    if (mungScrollbarColors = booleanDefault ("mungScrollbarColors", 0))
        SetSysColors (sizeof (SysColorNames), SysColorNames, TrekSysColors);
    return;
}

/* Check window's original position against current position, if different,
   return a char string that is the equivalent of what the netrekrc entry
   for that window geometry would be */
   
char *
checkWindowsGeometry (W_Window window)
{
    char str[256];
    WINDOWPLACEMENT loc;
    int x, y, width, height;
    register Window *win;

    if (!window)
        return (NULL);

    win = (Window *)window;

    loc.length = sizeof (WINDOWPLACEMENT); /* Have to set this */
    
    if (GetWindowPlacement (win->hwnd, &loc))
    {
        x = loc.rcNormalPosition.left;
        y = loc.rcNormalPosition.top;
        width = loc.rcNormalPosition.right - loc.rcNormalPosition.left;
        height = loc.rcNormalPosition.bottom - loc.rcNormalPosition.top;

        if (x != win->actual_x || y != win->actual_y
         || width != win->actual_width || height != win->actual_height)
        {
            /* TODO: fix main window so it takes into account all
               the SYS_CAPTION/border size changes, too messy for now */
            if (strcmpi (win->name, "netrek") == 0)
                return (geometryDefault ("netrek.geometry"));
            /* Let's keep local and map windows square! */
            else if (strcmpi (win->name, "local") == 0)
            {
                if (width > height)
                    width = height;
                else if (height > width)
                    height = width;
            }
            else if (strcmpi (win->name, "map") == 0)
            {
                if (width > height)
                    width = height;
                else if (height > width)
                    height = width;
            }
            
            /* To convert actual window location into a netrekrc format,
               we need to know the inherent conversion difference between
               what the netrekrc says (e.g. local.geometry: 500x500+0+0)
               and what size/location window is actually created (e.g. 
               local window is 506x506 pixels at (x,y) = (1,1)).  So we
               subtract the actual values minus the original values to
               get the conversion factor.  A further complication is that
               text, richtext, menu and scroll windows have different formulas
               for converting netrekrc window geometry entries into actual
               pixel widths and heights.  All of these adjust window size
               based on the size of text characters, not pixels.  Width,
               win->actual_* and win->orig_* are in units of pixels, so
               a back conversion needs to be performed to get the geometry
               into a netrekrc entry */
            width = width - (win->actual_width - win->orig_width);
            height = height - (win->actual_height - win->orig_height);
            x = x - (win->actual_x - win->orig_x);
            y = y - (win->actual_y - win->orig_y);

            switch (win->type)
            {
                case WIN_TEXT:
                    width = (width - WIN_EDGE * 2) / W_Textwidth;
                    height = (height - MENU_PAD * 2) / W_Textheight;
                    break;
                case WIN_MENU:
                    width = (width - WIN_EDGE * 2) / W_Textwidth;
                    height = (height - MENU_BAR) / (W_Textheight + MENU_PAD * 2 + MENU_BAR);
                    break;
                case WIN_RICHTEXT:
                case WIN_SCROLL:
                    width = (width - WIN_EDGE * 2 - GetSystemMetrics (SM_CXVSCROLL)) / W_Textwidth;
                    height = (height - MENU_PAD * 2) / W_Textheight;
                    break;
                case WIN_GRAPH:
                default:
                    break;
            }
            sprintf (str, "%dx%d+%d+%d", width, height, x, y);
            return strdup (str);
        }
    }
    /* Geometry hasn't changed, so use the default string from netrekrc
       if there is one */
    strcpy(str, win->name);
    strcat(str, ".geometry");
    return (geometryDefault (str));
}

/* Reread rc for geometry changes and update window if geometry was changed,
*  currently unused and non-functional */
void
updateWindowsGeometry (W_Window window)
{
	WINDOWPLACEMENT loc;
	int x, y, width, height;
	int result, scrollpos;

	FNHEADER_VOID;

	loc.length = sizeof (WINDOWPLACEMENT); /* Have to set this */

	if (!W_IsMapped (window))
		return;

	if (GetWindowPlacement (win->hwnd, &loc))
	{
		/* WIN_SCROLL processing */
		if (win->type == WIN_SCROLL)
		{
			x = loc.rcNormalPosition.left;
			y = loc.rcNormalPosition.top;
			width = loc.rcNormalPosition.right - loc.rcNormalPosition.left;
			height = loc.rcNormalPosition.bottom - loc.rcNormalPosition.top;

			if (result = checkGeometry (win->name, &x, &y, &width, &height))
			{
				if (result & G_SET_X)
				{
					loc.rcNormalPosition.left = x + win->parent->border;
				}
				if (result & G_SET_Y)
				{
					loc.rcNormalPosition.top = y + win->parent->border;
				}
				if (result & G_SET_WIDTH)
				{
					win->TextWidth = width;		// Set new text width for window
					width = (width * W_Textwidth) + (WIN_EDGE * 2) +
							GetSystemMetrics (SM_CXVSCROLL);

					loc.rcNormalPosition.right = x + width + (win->border * 2) + 1;
				}
				if (result & G_SET_HEIGHT)
				{
					win->TextHeight = height;	// Set new text height for window
					height = (height * W_Textheight) + (MENU_PAD * 2);

					loc.rcNormalPosition.bottom = y + height + (win->border * 2) + 1;
				}
				SetWindowPlacement (win->hwnd, &loc);
				scrollpos = win->NumItems - win->TextHeight;
				scrollpos = max (0, scrollpos);
				if (scrollpos > GetScrollPos (win->hwnd, SB_VERT))
				{
					SetScrollRange (win->hwnd, SB_VERT, 0, scrollpos, TRUE);
					SetScrollPos (win->hwnd, SB_VERT, scrollpos, TRUE);
				}
				else
					SetScrollRange (win->hwnd, SB_VERT, 0, 0, TRUE);
				W_FlushScrollingWindow (window);
			}
			else
			{
				/* Let's restore to original position */
				loc.rcNormalPosition.left = win->orig_x + win->parent->border;
				loc.rcNormalPosition.top = win->orig_y + win->parent->border;
				loc.rcNormalPosition.right = win->orig_x + (win->orig_width *  W_Textwidth) + 
											(WIN_EDGE * 2) +
											GetSystemMetrics (SM_CXVSCROLL) +
											(win->border * 2) + 1;
				loc.rcNormalPosition.bottom = win->orig_y + (win->orig_height * W_Textheight) +
											(MENU_PAD * 2) + (win->border * 2) + 1;
				SetWindowPlacement (win->hwnd, &loc);
			
				win->TextWidth = win->orig_width;
				win->TextHeight = win->orig_height;

				scrollpos = win->NumItems - win->TextHeight;
				scrollpos = max (0, scrollpos);
				if (scrollpos > GetScrollPos (win->hwnd, SB_VERT))
				{
					SetScrollRange (win->hwnd, SB_VERT, 0, scrollpos, TRUE);
					SetScrollPos (win->hwnd, SB_VERT, scrollpos, TRUE);
				}
				else
					SetScrollRange (win->hwnd, SB_VERT, 0, 0, TRUE);
				W_FlushScrollingWindow (window);
			}
		}
		/* WIN_TEXT processing */
		if (win->type == WIN_TEXT)
		{
			x = loc.rcNormalPosition.left;
			y = loc.rcNormalPosition.top;
			width = loc.rcNormalPosition.right - loc.rcNormalPosition.left;
			height = loc.rcNormalPosition.bottom - loc.rcNormalPosition.top;

			if (result = checkGeometry (win->name, &x, &y, &width, &height))
			{
				if (result & G_SET_X)
				{
					loc.rcNormalPosition.left = x + win->parent->border;
				}
				if (result & G_SET_Y)
				{
					loc.rcNormalPosition.top = y + win->parent->border;
				}
				if (result & G_SET_WIDTH)
				{
					win->TextWidth = width;		// Set new text width for window
					width = (width * W_Textwidth) + (WIN_EDGE * 2);

					loc.rcNormalPosition.right = x + width + (win->border * 2) + 1;
				}
				if (result & G_SET_HEIGHT)
				{
					win->TextHeight = height;	// Set new text height for window
					height = (height * W_Textheight) + (MENU_PAD * 2);

					loc.rcNormalPosition.bottom = y + height + (win->border * 2) + 1;
				}
				SetWindowPlacement (win->hwnd, &loc);
			}
			else
			{
				/* Let's restore to original position */
				loc.rcNormalPosition.left = win->orig_x + win->parent->border;
				loc.rcNormalPosition.top = win->orig_y + win->parent->border;
				loc.rcNormalPosition.right = win->orig_x + (win->orig_width *  W_Textwidth) + 
											(WIN_EDGE * 2) +
											GetSystemMetrics (SM_CXVSCROLL) +
											(win->border * 2) + 1;
				loc.rcNormalPosition.bottom = win->orig_y + (win->orig_height * W_Textheight) +
											(MENU_PAD * 2) + (win->border * 2) + 1;
				SetWindowPlacement (win->hwnd, &loc);
			
				win->TextWidth = win->orig_width;
				win->TextHeight = win->orig_height;

			}
		}
	}
}

//
// Rebuilds the DCs for the SDBUFFER, either because of resize or on first creation.
// Returns 0 on failure, 1 on success.
//
int
W_RecreateSDBDCs(W_Window window, SDBUFFER* sdb)
{
    Window * win = ((Window *)window);
    HBITMAP junk;

    sdb->win_dc = GetDC (win->hwnd);
    if (sdb->win_dc == NULL)
        return 0;

    sdb->mem_dc = CreateCompatibleDC (sdb->win_dc);
    if (sdb->mem_dc == NULL)
        return 0;

    GetClientRect (win->hwnd, &(sdb->wr));
    //This code ripped from W_Initialize, better safe than sorry
    junk = CreateBitmap (1, 1, 1, 1, NULL);
    GlobalOldMemDCBitmap = (HBITMAP) SelectObject (sdb->win_dc, junk);
    SelectObject (sdb->win_dc, GlobalOldMemDCBitmap);
    GlobalOldMemDC2Bitmap = (HBITMAP) SelectObject (sdb->mem_dc, junk);
    SelectObject (sdb->mem_dc, GlobalOldMemDC2Bitmap);
    DeleteObject (junk);

    sdb->mem_bmp = CreateCompatibleBitmap (sdb->win_dc, sdb->wr.right, sdb->wr.bottom);
    if (sdb->mem_bmp == NULL)
        return 0;

    sdb->old_bmp = (HBITMAP) SelectObject (sdb->mem_dc, sdb->mem_bmp);

    FillRect (sdb->mem_dc, &sdb->wr, colortable[W_Black].brush);

    return 1;
}

//
// Deallocates and recreates the DCs associated with the SDBUFFER
//
void
W_UpdateSDB(W_Window window, SDBUFFER * sdb)
{
    Window * win = ((Window *)window);
    HBITMAP origObj;

    origObj = SelectObject(sdb->mem_dc, sdb->old_bmp);
    DeleteObject(sdb->mem_bmp);
    DeleteObject(origObj);
    ReleaseDC (((Window *)sdb->window)->hwnd, sdb->win_dc);
    DeleteDC (sdb->mem_dc);

    (void) W_RecreateSDBDCs(window, sdb);
}

SDBUFFER *
W_InitSDB (W_Window window)
{
    Window * win;
    SDBUFFER * sdb;

    win = ((Window *)window);

    if (win->hwnd == NULL)
        return NULL;

    if (strcmpi (win->name, "local") == 0)
        win->sdb_flags = SDB_FLAG_LOCAL;
    else if (strcmpi (win->name, "map") == 0)
        win->sdb_flags = SDB_FLAG_MAP;
    sdb = (SDBUFFER *) malloc (sizeof (SDBUFFER));
    sdb->window = (W_Window) malloc (sizeof (Window));

    sdb->window = window;

    if (0 == W_RecreateSDBDCs(window, sdb))
        return NULL;

    return sdb;
}

inline SDBUFFER *
SDB_lookup (W_Window window)
{
    Window * win;
    win = ((Window *)window);

    if (win->hwnd == NULL)
        return NULL;
    if (win->sdb_flags == SDB_FLAG_LOCAL)
        return localSDB;
    else if (win->sdb_flags == SDB_FLAG_MAP)
        return mapSDB;
    else
        return NULL;
}

void
W_Win2Mem (SDBUFFER * sdb)
{
    BitBlt (sdb->mem_dc, 0, 0, sdb->wr.right, sdb->wr.bottom, sdb->mem_dc, 0, 0, SRCCOPY);
}

void
W_Mem2Win (SDBUFFER * sdb)
{
    BitBlt (sdb->win_dc, 0, 0, sdb->wr.right, sdb->wr.bottom, sdb->mem_dc, 0, 0, SRCCOPY);
}

// Make a WIN_SCROLL type window.
// We use a scrollbar so we can look through the text, something the X version
// didn't have. Nyah, nyah.
W_Window
W_MakeScrollingRichTextWindow (char *name,
                               int x,
                               int y,
                               int width,
                               int height,
                               W_Window parent,
                               int border)
{
    Window *newwin;

    //Get the default position, etc.
    checkGeometry (name, &x, &y, &width, &height);

    //Get the default parent..
    checkParent (name, &parent);

    if (!(newwin = newWindow (name, x, y,
                              width * W_Textwidth + WIN_EDGE * 2 +
                              GetSystemMetrics (SM_CXVSCROLL),
                              height * W_Textheight + MENU_PAD * 2, parent,
                              border, W_White, WIN_RICHTEXT)))
        return (0);

    SetWindowLongPtr (newwin->hwnd, GWLP_USERDATA, (LONG_PTR) newwin);
    lpfnDefRichEditWndProc = (WNDPROC) SetWindowLongPtr (newwin->hwnd, GWLP_WNDPROC, 
                                                        (DWORD) RichTextWndProc);
    SendMessage (newwin->hwnd, EM_AUTOURLDETECT, TRUE, 0);
    SendMessage (newwin->hwnd, EM_SETEVENTMASK, 0, ENM_LINK);
    SendMessage (newwin->hwnd, EM_SETBKGNDCOLOR, 0, W_Black);
    SendMessage (newwin->hwnd, WM_SETFONT, (WPARAM) W_RegularFont, TRUE);

    //Store the original textheight, width
    newwin->TextHeight = height;
    newwin->TextWidth = width;

    /* Set original coordinates, so we will be able to restore to them */
    newwin->orig_x = x;
    newwin->orig_y = y;
    newwin->orig_width = width * W_Textwidth + WIN_EDGE * 2 +
                              GetSystemMetrics (SM_CXVSCROLL);
    newwin->orig_height = height * W_Textheight + MENU_PAD * 2;

    //Map (show) the window if the user spec'd it
    if (checkMapped (name))
        W_MapWindow ((W_Window) newwin);

    return (W_Window) newwin;
}


void
AddToRichText (Window * win,
               W_Color color,
               char *str,
               int len)
{
    struct stringList *p = win->strings;
    struct stringList *end, *p2;
    int NumStrings = win->NumItems;
    int diff;
    char str1[256];
    CHARFORMAT2 cf;
    POINT point;
    RECT rect;
    HWND hwnd;
    CHARRANGE cr;
    int numLines, visibleLines;
    int x, y;

    //Find the end of the linked-list of strings...
    if (p)                      // ...if the list has been created
    {
        end = p;
        while (end->next)
            end = end->next;
    }
    else
    {
        end = 0;
        NumStrings = 0;
    }

    if (NumStrings < maxScrollLines)    //Create a new stringList item
    {
        p2 = (struct stringList *) malloc (sizeof (struct stringList));
        if (!p2)
        {
            LineToConsole ("Not enough memory to allocate a new listbox string.\n");
            return;
        }

        if (!p)
            win->strings = p2;  //Store the new start of list

        p = p2;                 //Point p to the new string

        win->NumItems = ++NumStrings;   //Inc the string number
    }
    else                        //Re-use the first string, place it at the end of the list
    {
        if (p->string)
            free (p->string);
        win->strings = p->next; //Store the new start of list
    }

    if (str[len - 1] == '\n')   //Remove trailing newlines
        str[--len] = 0;

    p->string = (char *) malloc (win->TextWidth + 1);
    if (!p->string)
    {
        LineToConsole ("Not enough memory to allocate a new listbox string.");
        return;
    }

    p->color = color;
    strncpy (p->string, str, win->TextWidth);

    /* we pad out the string with spaces so we don't have to clear
       the window */
    if (len < win->TextWidth - 1)
        memset (p->string + len, ' ', win->TextWidth - len - 1);
    p->string[win->TextWidth - 1] = 0;

    if (end)
        end->next = p;
    p->next = 0;

    //Mark as out of date...
    win->AddedStrings++;

    hwnd = GetFocus ();
    SendMessage (win->hwnd, EM_EXGETSEL, 0, (LPARAM) &cr);
    //SendMessage (win->hwnd, EM_HIDESELECTION, TRUE, 0);

    cf.cbSize = sizeof (CHARFORMAT2);
    cf.dwEffects = 0;
    cf.dwMask = CFM_COLOR;
    cf.crTextColor = colortable[color].rgb;

    strcpy (str1, str);
    strcat (str1, "\n");

    SendMessage (win->hwnd, EM_GETSCROLLPOS, 0, (LPARAM) &point);
    numLines = SendMessage (win->hwnd, EM_GETLINECOUNT, 0, 0);
    SendMessage (win->hwnd, EM_GETRECT, 0, (LPARAM) &rect);
    visibleLines = rect.bottom / W_Textheight;
    SendMessage (win->hwnd, EM_SETSEL, -1, -1);
    SendMessage (win->hwnd, EM_SETCHARFORMAT, SCF_SELECTION, (LPARAM) &cf);
    SendMessage (win->hwnd, EM_REPLACESEL, FALSE, (LPARAM) str1);
    diff = numLines - point.y / W_Textheight - visibleLines;
    if (diff >= 1)
    {
        // Scroll if mouse isn't in window, or if mouse is in
        // window but scrollback is already at bottom
        if (!findMouseInWin (&x, &y, (W_Window) win) || diff == 1)
            point.y += W_Textheight * diff;
        SendMessage (win->hwnd, EM_SETSCROLLPOS, 0, (LPARAM) &point);
    }
    //SendMessage (win->hwnd, EM_HIDESELECTION, FALSE, 0);
    //SendMessage (win->hwnd, EM_SCROLLCARET, 0, 0);

    if (cr.cpMin != cr.cpMax)
        SendMessage (win->hwnd, EM_SETSEL, 0, (LPARAM) &cr);

    if (hwnd)
        SetFocus (hwnd);
}


LRESULT CALLBACK RichTextWndProc (HWND hwnd,
                                  UINT msg,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
//    Window * win;

    switch (msg)
    {
    case WM_LBUTTONDOWN:
        BringWindowToTop (hwnd);
        // fake a caption hit to move window
        if (richTextMove)
            SendMessage(hwnd, WM_NCLBUTTONDOWN, HTCAPTION, 0);
        break;
    case WM_NCRBUTTONDOWN:
        if (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_SIZEBOX)
        {
            // remove sizing border and redraw
            SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
            SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
        }
        else
        {
            // set sizing border and redraw
            SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) | WS_SIZEBOX);
            SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
        }
        break;
    case WM_RBUTTONDOWN:
        if (GetWindowLongPtr(hwnd, GWL_STYLE) & WS_SIZEBOX)
        {
            // remove sizing border and redraw
            SetWindowLongPtr(hwnd, GWL_STYLE, GetWindowLongPtr(hwnd, GWL_STYLE) & ~WS_SIZEBOX);
            SetWindowPos(hwnd, 0, 0, 0, 0, 0, SWP_FRAMECHANGED | SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
        }
    }
    return CallWindowProc (lpfnDefRichEditWndProc, hwnd, msg, wParam, lParam);
}
