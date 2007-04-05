/* Wlib.h

 * Include file for the Windowing interface.
 *
 * Kevin P. Smith  6/11/89
 *
 * The deal is this:
 *   Call W_Initialize(), and then you may call any of the listed fuinctions.
 *   Also, externals you are allowed to pass or use include W_BigFont,
 *     W_RegularFont, W_UnderlineFont, W_HighlightFont, W_White, W_Black,
 *     W_Red, W_Green, W_Yellow, W_Cyan, W_Grey, W_Textwidth, and W_Textheight.
 */

#ifndef _h_Wlib
#define _h_Wlib

#include <windows.h>

#include "copyright2.h"
#include "defs.h"

typedef void (*W_Callback) ();

typedef char *W_Window;

typedef struct event
{
    int type;
    W_Window Window;
    unsigned char key;
    int x, y;
#ifdef MOUSE_AS_SHIFT
    int modifier;
#endif
}
W_Event;

extern W_Callback W_GetWindowKeyDownHandler (W_Window w);
extern W_Callback W_GetWindowKeyUpHandler (W_Window w);
extern W_Callback W_GetWindowButtonHandler (W_Window w);
extern W_Callback W_GetWindowExposeHandler (W_Window w);
extern W_Callback W_SetWindowKeyDownHandler (W_Window w,
                                             W_Callback c);
extern W_Callback W_SetWindowKeyUpHandler (W_Window w,
                                           W_Callback c);
extern W_Callback W_SetWindowButtonHandler (W_Window w,
                                            W_Callback c);
extern W_Callback W_SetWindowExposeHandler (W_Window w,
                                            W_Callback c);

typedef char *W_Icon;
typedef char *W_Font;
typedef int W_Color;

extern W_Font W_BigFont, W_RegularFont, W_UnderlineFont, W_HighlightFont,
    W_IndyFont;
extern W_Color W_White, W_Black, W_Red, W_Green, W_Yellow, W_Cyan, W_Grey;
#ifdef RACE_COLORS
extern W_Color W_Ind, W_Fed, W_Rom, W_Kli, W_Ori;
#endif
extern int W_Textwidth, W_Textheight;
extern int W_FastClear;
extern W_Font W_MyPlanetFont, W_FriendlyPlanetFont, W_EnemyPlanetFont;

extern void W_Initialize (char *str);
extern W_Window W_MakeWindow (char *name,
                              int x,
                              int y,
                              int width,
                              int height,
                              W_Window parent,
                              int border,
                              W_Color color);
extern W_Icon W_StoreBitmap (int width,
                             int height,
                             char *data,
                             W_Window window);

extern W_Icon W_StoreBitmap2 (HINSTANCE hLibInstance,
                              int width,
                              int height,
                              int data,
                              W_Window window,
                              UINT BmpType);
extern W_Icon W_PointBitmap2 (W_Icon bigbitmap,
                              int col,
                              int row,
                              int width,
                              int height);

extern W_Window W_MakeTextWindow (char *name,
                                  int x,
                                  int y,
                                  int width,
                                  int height,
                                  W_Window parent,
                                  int border);
extern W_Window W_MakeScrollingWindow (char *name,
                                       int x,
                                       int y,
                                       int width,
                                       int height,
                                       W_Window parent,
                                       int border);
extern W_Window W_MakeMenu (char *name,
                            int x,
                            int y,
                            int width,
                            int height,
                            W_Window parent,
                            int border);
extern void W_WriteText (W_Window window,
                         int x,
                         int y,
                         W_Color color,
                         char *str,
                         int len,
                         W_Font font);
extern void W_MaskText (W_Window window,
                        int x,
                        int y,
                        W_Color color,
                        char *str,
                        int len,
                        W_Font font);
extern void W_WriteBitmap (int x,
                           int y,
                           W_Icon bit,
                           W_Color color,
                           W_Window window);
extern void W_WriteScaleBitmap (int x,
                                int y,
                                int destwidth,
                                int destheight,
                                int srcwidth,
                                int srcheight,
                                int angle,
                                W_Icon icon,
                                W_Color color,
                                W_Window window);
extern void W_ClearArea (W_Window window,
                         int x,
                         int y,
                         int width,
                         int height);
extern void W_MakeLine (W_Window window,
                        int x0,
                        int y0,
                        int x1,
                        int y1,
                        W_Color color);
extern void W_MakeDashedLine (W_Window window,
                              int x0,
                              int y0,
                              int x1,
                              int y1,
                              W_Color color);
extern void W_MapWindow (W_Window window);
extern void W_UnmapWindow (W_Window window);
extern int W_EventsPending (void);
extern void W_NextEvent (W_Event * wevent);
extern void W_SetWindowName (W_Window window,
                             char *name);
extern void W_TileWindow (W_Window window,
                          W_Icon bit);
extern void W_UnTileWindow (W_Window window);
extern void W_ChangeBorder (W_Window window,
                            int color);
extern void W_DefineCursor (W_Window window,
                            int width,
                            int height,
                            char *bits,
                            char *mask,
                            int xhot,
                            int yhot);
extern int W_IsMapped (W_Window window);
extern void W_Beep (void);
extern void W_DestroyWindow (W_Window window);
extern int W_WindowWidth (W_Window window);
extern int W_WindowHeight (W_Window window);
extern int W_Socket (void);
extern void W_ClearWindow (W_Window window);
extern void W_CacheLine (W_Window,
                         int,
                         int,
                         int,
                         int,
                         int);
extern void W_MakeTractLine (W_Window,
                             int,
                             int,
                             int,
                             int,
                             W_Color);
extern void W_MakePhaserLine (W_Window,
                              int,
                              int,
                              int,
                              int,
                              W_Color);
extern void W_WriteCircle (W_Window,
                           int,
                           int,
                           int,
                           int,
                           int,
                           W_Color);
extern void W_WriteTriangle (W_Window,
                             int,
                             int,
                             int,
                             int,
                             W_Color);
extern void W_CacheClearArea (W_Window,
                              int,
                              int,
                              int,
                              int);
extern void W_OverlayBitmap (int,
                             int,
                             W_Icon,
                             W_Color,
                             W_Window);
extern void W_OverlayScaleBitmap (int,
                                  int,
                                  int,
                                  int,
                                  int,
                                  int,
                                  int,
                                  W_Icon,
                                  W_Color,
                                  W_Window);

#define W_EV_EXPOSE	1
#define W_EV_KEY	2
#define W_EV_BUTTON	3

#ifdef MOUSE_AS_SHIFT
#define W_EV_MKEY	4
#endif

#ifdef AUTOKEY
#define W_EV_KEY_OFF	4
#endif /*AUTOKEY */

#ifdef MOTION_MOUSE
#define W_EV_CM_BUTTON	5
#endif

#define W_LBUTTON	    1
#define W_MBUTTON	    2
#define W_RBUTTON	    3

/* Microsoft XButtons support */
#define W_XBUTTON1	    4
#define W_XBUTTON2	    5

/* Wheel Mouse support */
#define W_WHEELUP	    6
#define W_WHEELDOWN     7

#ifdef SHIFTED_MOUSE
#define W_LBUTTON2	    8
#define W_MBUTTON2	    9
#define W_RBUTTON2	    10
#define W_XBUTTON1_2	11
#define W_XBUTTON2_2	12

#define W_LBUTTON3	    13
#define W_MBUTTON3	    14
#define W_RBUTTON3	    15
#define W_XBUTTON1_3	16
#define W_XBUTTON2_3	17

#define W_LBUTTON4	    18
#define W_MBUTTON4	    19
#define W_RBUTTON4	    20
#define W_XBUTTON1_4	21
#define W_XBUTTON2_4	22
#endif

#define W_BoldFont W_HighlightFont

#endif /* _h_Wlib */
