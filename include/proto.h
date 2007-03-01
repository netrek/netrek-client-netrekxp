/******************************************************************************/
/***  File:  proto.h                                                        ***/
/***                                                                        ***/
/***  Function:  Function prototypes for externally accessed functions.     ***/
/***                                                                        ***/
/***                                                                        ***/
/***                                                                        ***/
/***                                                                        ***/
/***  Revisions:                                                            ***/
/***    27Aug00  - Ssheldon cataloged functions and created this file       ***/
/***                                                                        ***/
/***                                                                        ***/
/***                                                                        ***/
/***                                                                        ***/
/******************************************************************************/

#ifndef __INCLUDED_proto_h__
#define __INCLUDED_proto_h__

#include <gmp.h>
#include <sys/types.h>

/******************************************************************************/
/***  beeplite.c                                                            ***/
/******************************************************************************/
void rcdlite (struct distress *dist);
void litedefaults (void);
void liteplanet (struct planet *l);
void liteplayer (struct player *j);
int makelite (struct distress * dist, char *pm);

/******************************************************************************/
/***  check.c                                                               ***/
/******************************************************************************/
void check (void);

/******************************************************************************/
/***  colors.c                                                              ***/
/******************************************************************************/
void getColorDefs (void);

/******************************************************************************/
/***  console.c                                                             ***/
/******************************************************************************/
int IsConsoleVisible (void);
void ShowConsole (void);
void LineToConsole (char * format, ...);
void BufferToConsole (void);

/******************************************************************************/
/***  cowmain.c                                                             ***/
/******************************************************************************/
char *query_cowid (void);
int cowmain (char *server,
             int port,
             char *name);
// static RETSIGTYPE handle_exception(int _dummy);
void terminate (int error);

/******************************************************************************/
/***  dashboard.c                                                           ***/
/******************************************************************************/
//static void    db_box(int x, int y, int w, int h, int f, int color);
//static void    db_bar(char *lab, int x, int y, int value, int tmpmax, int max, int digits, int color);
//static void    db_flags(int fr);
void db_timer (int fr, int xloc, int yloc);
void db_redraw_krp (int fr);
void db_redraw_COW (int fr);
void db_redraw (int fr);
void db_special (int fr, int x, int y);
int repair_time (void);
int get_closest_planet(int x, int y);

/******************************************************************************/
/***  dashboard3.c                                                          ***/
/******************************************************************************/
//static int db_itoa (char *s, int v);
//static int db_ftoa (char *s, float v);
//static void db_bar (char *l, int x, int y, int w, int h, int m, int t, int v, int b);
//static void db_flags (int fr);
void db_redraw_lab2 (int fr);

/******************************************************************************/
/***  death.c                                                               ***/
/******************************************************************************/
void death (void);
void updatedeath (void);

/******************************************************************************/
/***  defaults.c                                                            ***/
/******************************************************************************/
void initDefaults (char *deffile);
void saveOptions (void);
char *getdefault (char *str);
char *getServerNick (char *srvName);
int getServerType (char *srvName);
#ifndef __BORLANDC__
#ifndef CYGWIN
int strncmpi (char *str1, char *str2, int max);
#endif
#endif
int booleanDefault (char *def, int preferred);
int intDefault (char *def, int preferred);
char *stringDefault (char *str);
int findDefaults (char *deffile, char *file);
void resetdefaults (void);
void shipchange (int type);
int findfile (char *fname,
              char *found);

/******************************************************************************/
/***  showdef.c                                                             ***/
/******************************************************************************/
void showdef (void);
//static char *btoa(int v);
//static char *itos(int v);

/******************************************************************************/
/***  distress.c                                                            ***/
/******************************************************************************/
//static itoa2(int n, char *s);
//static void testmacro(char *bufa, char *bufb, int *inda, int *indb);
//static int solvetest(char *bufa, int *inda);
//static int condmacro(char *bufa, char *bufb, int *inda, int *indb, int flag);
//static int skipmacro(char *buf, int index);
char *strcap (char *s);
void HandleGenDistr (char *message,
                     unsigned char from,
                     unsigned char to,
                     struct distress *dist);
void Dist2Mesg (struct distress *dist,
                char *buf);
int makedistress (struct distress *dist,
                  char *cry,
                  char *pm);

/******************************************************************************/
/***  dmessage.c                                                            ***/
/******************************************************************************/
void dmessage (char *message,
               unsigned char flags,
               unsigned char from,
               unsigned char to);
int instr (char *string1,
           char *string2);
// static void CheckFeatures(char *m);
void sendVersion (void);
#ifdef CLUECHECKBORG
void ClueRespond (char *message);
#endif

/******************************************************************************/
/***  docwin.c                                                              ***/
/******************************************************************************/
void showdocs (int atline);
void loaddocs (void);
void showxtrekrc (int atline);
void loadxtrekrc (void);

/******************************************************************************/
/***  enter.c                                                               ***/
/******************************************************************************/
void enter (void);
void openmem (void);
void rotateTeams (void);
void drawTstats (void);
#ifdef HOCKEY_LINES
void check_hockey_mode (void);
void init_hockey_lines (void);
#endif

/******************************************************************************/
/***  feature.c                                                             ***/
/******************************************************************************/
#ifdef FEATURE_PACKETS
void sendFeature (char *name,
                  char feature_type,
                  int value,
                  char arg1,
                  char arg2);
struct feature_cpacket;
void handleFeature (struct feature_cpacket *packet);
#endif

/******************************************************************************/
/***  findslot.c                                                            ***/
/******************************************************************************/
//static void mapWaitWin(W_Window waitWin);
//static void mapWaitQuit(W_Window qwin);
//static void mapWaitCount(W_Window waitWin, W_Window countWin, int count);
//static void mapWaitMotdButton(W_Window motdButtonWin);
int findslot (void);

/******************************************************************************/
/***  getname.c                                                             ***/
/******************************************************************************/
//static void noautologin(void);
//static void showreadme(void);
//static void try_autologin(char *defname);
//static void loaddude();
//static void checkpassword();
//static void makeNewGuy();
//static void adjustString(unsigned char ch, char *str, char *defname);
//static void displayFormat (char *defname);
//static void displayStartup(char *defname);
void getname (char *defname,
              char *defpasswd);
void noserver (void);

/******************************************************************************/
/***  getship.c
/******************************************************************************/
void getshipdefaults ();
void getship (struct ship *shipp,
              int s_type);

/******************************************************************************/
/***  helpwin.c
/******************************************************************************/
void fillhelp (void);
void update_Help_to_Keymap (char *helpmessage);
void pbfillhelp (void);    /* Help window for playback */
void helpaction (W_Event * data);

/******************************************************************************/
/***  hintwin.c
/******************************************************************************/
void fillhint (void);
void inithints (void);
void showhint (int);
void hintaction (W_Event * data);
void hintshow (W_Event * data);

/******************************************************************************/
/***  inform.c
/******************************************************************************/
void inform (W_Window ww,
             int x,
             int y,
             char key);
void destroyInfo (void);

/******************************************************************************/
/***  input.c                                                               ***/
/******************************************************************************/
unsigned char getctrlkey (unsigned char **s);
unsigned char getkeyfromctrl (unsigned char c);
void initkeymap (void);
void initinput (void);
RETSIGTYPE detsetallow (int _dummy);
void input ();
#ifdef THREADED
void input2 ();
#endif
int process_event (void);
void keyaction (W_Event * data);
#ifdef MOUSE_AS_SHIFT
void mkeyaction (W_Event * data);
#endif
void buttonaction (W_Event * data);
int getcourse (W_Window ww,
               int x,
               int y);
void scan (W_Window w,
           int x,
           int y);
int detmine (void);
//static void lockPlanetOrBase(W_Window ww, int x, int y);
void macro_on (void);
void doMacro (W_Event *);
void reread_defaults (void);
void emptyKey (void),
  Key32 (void),
  Key33 (void),
  Key34 (W_Event * data),
  Key35 (void),
  Key36 (void),
  Key37 (void),
  Key38 (void),
  Key39 (void),
  Key40 (void),
  Key41 (W_Event * data),
  Key42 (void),
  Key43 (void),
  Key44 (void),
  Key45 (void),
  Key46 (void),
  Key47 (void),
  Key48 (void),
  Key49 (void),
  Key50 (void),
  Key51 (void),
  Key52 (void),
  Key53 (void),
  Key54 (void),
  Key55 (void),
  Key56 (void),
  Key57 (void),
  Key58 (void),
  Key59 (W_Event * data),
  Key60 (void),
  Key61 (void),
  Key62 (void),
  Key63 (void),
  Key64 (void),
  Key65 (W_Event * data),
  Key66 (void),
  Key67 (void),
  Key68 (void),
  Key69 (W_Event * data),
  Key70 (W_Event * data),
  Key71 (W_Event * data),
  Key72 (W_Event * data),
  Key73 (W_Event * data),
  Key74 (W_Event * data),
  Key75 (void),
  Key76 (void),
  Key77 (W_Event * data),
  Key78 (void),
  Key79 (void),
  Key80 (void),
  Key81 (void),
  Key82 (void),
  Key83 (void),
  Key84 (W_Event * data),
  Key85 (void),
  Key86 (void),
  Key87 (W_Event * data),
  Key88 (void),
  Key89 (W_Event * data),
  Key90 (W_Event * data),
  Key91 (void),
  Key92 (void),
  Key93 (void),
  Key94 (W_Event * data),
  Key95 (W_Event * data),
  Key96 (void),
  Key97 (W_Event * data),
  Key98 (void),
  Key99 (void),
  Key100 (void),
  Key101 (void),
  Key102 (W_Event * data),
  Key103 (W_Event * data),
  Key104 (void),
  Key105 (W_Event * data),
  Key106 (W_Event * data),
  Key107 (W_Event * data),
  Key108 (W_Event * data),
  Key109 (void),
  Key110 (W_Event * data),
  Key111 (void),
  Key112 (W_Event * data),
  Key113 (void),
  Key114 (void),
  Key115 (void),
  Key116 (W_Event * data),
  Key117 (void),
  Key118 (W_Event * data),
  Key119 (void),
  Key120 (void),
  Key121 (W_Event * data),
  Key122 (void),
  Key123 (void),
  Key124 (void),
  Key125 (void),
  Key126 (W_Event * data),
  Key127 (W_Event * data);
/* control keys */
void Key131 (W_Event * data),
  Key144 (W_Event * data),
  Key145 (W_Event * data),
  Key146 (W_Event * data),
  Key147 (W_Event * data),
  Key148 (W_Event * data),
  Key149 (W_Event * data),
  Key150 (W_Event * data),
  Key151 (W_Event * data),
  Key152 (W_Event * data),
  Key153 (W_Event * data),
  Key160 (W_Event * data),
  Key162 (W_Event * data),
  Key163 (W_Event * data),
  Key175 (W_Event * data),
  Key180 (W_Event * data),
  Key194 (W_Event * data),
  Key195 (W_Event * data),
  Key197 (W_Event * data),
  Key198 (W_Event * data),
  Key200 (W_Event * data),
  Key206 (W_Event * data),
  Key207 (W_Event * data),
  Key204 (W_Event * data),
  Key205 (W_Event * data),
  Key208 (W_Event * data),
  Key212 (W_Event * data);

/******************************************************************************/
/***  interface.c
/******************************************************************************/
void set_speed (int speed);
void set_course (unsigned char dir);
void shield_up (void);
void shield_down (void);
void shield_tog (void);
void bomb_planet (void);
void beam_up (void);
void beam_down (void);
void repair (void);
void repair_off (void);
void repeat_message (void);
void cloak (void);
void cloak_on (void);
void cloak_off (void);
unsigned long mstime (void);
unsigned long msetime (void);
void run_clock (time_t curtime);

/******************************************************************************/
/***  local.c
/******************************************************************************/
void initStars (void);
static inline W_Icon planetBitmapC(register struct planet * p);
static inline void planetResourcesC (register struct planet *p, int destwidth,
                                     int destheight, int dx, int dy, W_Window window);
//static void DrawPlanets(void);
//static void DrawShips(void);
//static void DrawTorps(void);
void DrawPlasmaTorps (void);
//static void DrawMisc(void);
inline void local (void);
inline void clearLocal (void);

/******************************************************************************/
/***  macrowin.c
/******************************************************************************/
int formatline (char *line);
void filldist (int fill);
void fillmacro (void);
void switchmacros (void);
void showMacroWin (void);

/******************************************************************************/
/***  main.c
/******************************************************************************/
void main2 (int argc,
            char *argv[]);
// static void printUsage(char *prog);

/******************************************************************************/
/***  map.c
/******************************************************************************/
void initPlanets (void);
//inline static void checkRedraw(int x, int y);
static inline W_Icon mplanetBitmapC(register struct planet * p);
static inline void mplanetResourcesC (register struct planet *p, int destwidth,
                                      int destheight, int dx, int dy, W_Window window);
//static void DrawPlanets();
void map (void);

/******************************************************************************/
/***  mswindow.c
/******************************************************************************/
void W_Initialize (char *display);
void GetColors ();
W_Window W_RenameWindow (W_Window window,
                         char *str);
Window *newWindow (char *name,
                   int x,
                   int y,
                   int width,
                   int height,
                   W_Window parent,
                   int border,
                   W_Color color,
                   int type);
W_Window W_MakeWindow (char *name,
                       int x,
                       int y,
                       int width,
                       int height,
                       W_Window parent,
                       int border,
                       W_Color color);
W_Window W_MakeTextWindow (char *name,
                           int x,
                           int y,
                           int width,
                           int height,
                           W_Window parent,
                           int border);
W_Window W_MakeScrollingWindow (char *name,
                                int x,
                                int y,
                                int width,
                                int height,
                                W_Window parent,
                                int border);
W_Window W_MakeScrollingRichTextWindow (char *name,
                                        int x,
                                        int y,
                                        int width,
                                        int height,
                                        W_Window parent,
                                        int border);
W_Window W_MakeMenu (char *name,
                     int x,
                     int y,
                     int width,
                     int height,
                     W_Window parent,
                     int border);
void W_ResizeMenuToNumItems (W_Window window, int numitems);
void W_ChangeBorder (W_Window window,
                     W_Color color);
void W_MapWindow (W_Window window);
void W_UnmapWindow (W_Window window);
int W_IsMapped (W_Window window);
void W_FillArea (W_Window window,
                 int x,
                 int y,
                 int width,
                 int height,
                 int color);
void W_ClearArea (W_Window window,
                  int x,
                  int y,
                  int width,
                  int height);
void W_CacheClearArea (W_Window window,
                       int x,
                       int y,
                       int width,
                       int height);
void W_ClearAreas (W_Window window,
                   int *xs,
                   int *ys,
                   int *widths,
                   int *heights,
                   int num);
void W_ClearWindow (W_Window window);
int W_EventsPending ();
int W_WaitForEvent ();
void W_TerminateWait ();
void W_NextEvent (W_Event * event);
int W_SpNextEvent (W_Event * event);
LRESULT CALLBACK NetrekWndProc (HWND hwnd,
                                UINT msg,
                                WPARAM wParam,
                                LPARAM lParam);
int W_EventsQueued ();
void W_MakeLine (W_Window window,
                 int x0,
                 int y0,
                 int x1,
                 int y1,
                 W_Color color);
void W_CacheLine (W_Window window,
                  int x0,
                  int y0,
                  int x1,
                  int y1,
                  W_Color color);
void W_FlushLineCaches (W_Window window);
void W_MakeLines (W_Window window,
                  int *x0,
                  int *y0,
                  int *x1,
                  int *y1,
                  int num,
                  W_Color color);
void W_MakePoint (W_Window window,
                 int x0,
                 int y0,
                 W_Color color);
void W_CachePoint(W_Window window, 
                  int x0,
                  int y0,
                  W_Color color);
void W_MakeTractLine (W_Window window,
                      int x0,
                      int y0,
                      int x1,
                      int y1,
                      W_Color color);
void Old_W_MakeTractLine (W_Window window,
                          int x0,
                          int y0,
                          int x1,
                          int y1,
                          W_Color color);
void W_MakePhaserLine (W_Window window,
                       int x0,
                       int y0,
                       int x1,
                       int y1,
                       W_Color color);
void W_WriteCircle (W_Window window,
                    int x,
                    int y,
                    int r,
                    int highlight,
                    W_Color color);
void W_WriteTriangle (W_Window window,
                      int x,
                      int y,
                      int s,
                      int t,
                      W_Color color);
void W_WriteText (W_Window window,
                  int x,
                  int y,
                  W_Color color,
                  char *str,
                  int len,
                  W_Font font);
void W_MaskText (W_Window window,
                 int x,
                 int y,
                 W_Color color,
                 char *str,
                 int len,
                 W_Font font);
void W_ShowBitmaps ();
W_Icon W_StoreBitmap (int width,
                      int height,
                      char *bits,
                      W_Window window);
W_Icon W_StoreBitmap2 (HINSTANCE hDLLInstance,
                       int width,
                       int height,
                       int bits,
                       W_Window window,
                       UINT BmpType);
W_Icon W_StoreBitmap3 (char *BMPfile,
                       int width,
                       int height,
                       int bits,
                       W_Window window,
                       UINT BmpType);
void W_WriteBitmap (int x,
                    int y,
                    W_Icon icon,
                    W_Color color,
                    W_Window window);
void W_WriteScaleBitmap (int x,
                         int y,
                         int destwidth,
                         int destheight,
                         int srcwidth,
                         int srcheight,
                         int angle,
                         W_Icon icon,
                         W_Color color,
                         W_Window window);
void W_WriteBitmapGrey (int x,
                        int y,
                        W_Icon icon,
                        W_Color color,
                        W_Window window);
void W_TileWindow (W_Window window,
                   W_Icon icon);
void W_UnTileWindow (W_Window window);
int W_WindowWidth (W_Window window);
int W_WindowHeight (W_Window window);
int W_Socket ();
void W_Beep ();
void W_DestroyWindow (W_Window window);
void RedrawMenu (Window * win,
                 HDC hdc);
void ChangeMenuItem (Window * win,
                     int n,
                     char *str,
                     int len,
                     W_Color color);
void AddToScrolling (Window * win,
                     W_Color color,
                     char *str,
                     int len);
void AddToRichText (Window * win,
                    W_Color color,
                    char *str,
                    int len);
void RedrawScrolling (Window * win,
                      HDC hdc);
void W_FlushScrollingWindow (W_Window window);
void W_DefineMapcursor (W_Window window);
void W_DefineLocalcursor (W_Window window);
void W_DefineTrekCursor (W_Window window);
void W_DefineWarningCursor (W_Window window);
void W_DefineArrowCursor (W_Window window);
void W_DefineTextCursor (W_Window window);
int checkGeometry (char *name,
                   int *x,
                   int *y,
                   int *width,
                   int *height);
int checkMapped (char *name);
int checkMappedPref (char *name,
                     int preferred);
void W_WarpPointer (W_Window window);
int findMouseInWin (int *x,
                    int *y,
                    W_Window window);
void DrawBorder (Window * win,
                 HDC hdc);
unsigned char *X11toCursor (unsigned char *bits,
                            int width,
                            int height);
unsigned char *X11toDIB (unsigned char *bits,
                         int width,
                         int height);
unsigned char *X11toDIBAndMirror (unsigned char *bits,
                                  int width,
                                  int height,
                                  int outwidth,
                                  int outheight);
void W_ResizeWindow (W_Window window,
                     int neww,
                     int newh);
void W_ResizeTextWindow (W_Window window,
                         int neww,
                         int newh);

void W_ResizeMenu (W_Window window,
                   int neww,
                   int newh);
int W_Mono ();
void W_SetSensitive (Window * window,
                     int b);
void W_OverlayBitmap (int x,
                      int y,
                      W_Icon icon,
                      W_Color color,
                      W_Window window);
void W_OverlayScaleBitmap (int x,
                           int y,
                           int destwidth,
                           int destheight,
                           int srcwidth,
                           int srcheight,
                           int angle,
                           W_Icon icon,
                           W_Color color,
                           W_Window window);
void W_WriteTTSText  (W_Window window,
		     int max_width,
		     int tts_ypos,
		     char *str,
		     int len);
void W_SetWindowName (W_Window window,
                      char *name);
inline void ResetSysColors (void);
inline void SetTrekSysColors (void);
void updateWindowsGeometry (W_Window win);
void W_MessageAllowedWindows (int messtype,
                              int x,
                              int y,
                              W_Color color,
                              char *str,
                              int len,
                              W_Font font);
void W_SetWAM (W_Window win);
/* RichText */
LRESULT CALLBACK RichTextWndProc (HWND hwnd,
                                  UINT msg,
                                  WPARAM wParam,
                                  LPARAM lParam);

SDBUFFER * W_InitSDB (W_Window window);
SDBUFFER * SDB_lookup (W_Window window);
void W_Win2Mem (SDBUFFER * sdb);
void W_Mem2Win (SDBUFFER * sdb);

/******************************************************************************/
/***  newwin.c
/******************************************************************************/
//static void handleMessageWindowKeyDown(W_Event * event);
//static void handleMessageWindowButton(W_Event * event);
void newwin (char *hostmon,
        char *progname);
void mapAll (void);
void savebitmaps (void);
void entrywindow (int *team,
                  int *s_type);
int teamRequest (int team,
                 int ship);
int numShips (int owner);
int realNumShips (int owner);
int deadTeam (int owner);
int checkBold (char *line);
void showMotdWin (W_Window motdwin, int atline);
struct list;
void showValues (struct list *data);
void newMotdLine (char *line);
void getResources (char *prog);
void redrawTeam (W_Window win,
                 int teamNo,
                 int *lastnum);
void redrawQuit (void);
void showTimeLeft (time_t time, time_t max);
void W_DefineFedCursor (W_Window window);
void W_DefineRomCursor (W_Window window);
void W_DefineKliCursor (W_Window window);
void W_DefineOriCursor (W_Window window);

/******************************************************************************/
/***  option.c
/******************************************************************************/
void optionwindow (void);
void RefreshOptions (void);
void OptionClear (int i);
void optionredrawtarget (W_Window win);
void optionredrawoption (int *ip);
struct option;
void optionrefresh (register struct option *op);
void optionaction (W_Event * data);
void SetMenuPage (int pagenum);
void optiondone (void);
int InitOptionMenus (void);
void AddOptMenu (struct option *NewMenu,
                 int updated);
void UpdateOptions (void);
int NumOptions (struct option *OpMenu);

/******************************************************************************/
/***  parsemeta.c
/******************************************************************************/
//static int open_port(char *host, int port, int verbose);
//static void parseInput(char *in, FILE * out, int metaStatusLevel);
//static int ReadFromMeta(int metaStatusLevel);
//static int ReadFromCache(int metaStatusLevel);
void parsemeta (int metaType);
//static void metarefresh(int i, W_Color color);
void metawindow (void);
//static void metadone(void);
void metaaction (W_Event * data);
void metainput (void);
#ifdef METAPING
void metapinginit(void);
DWORD WINAPI metaPing_thread(void);
#endif

/******************************************************************************/
/***  ping.c
/******************************************************************************/
struct ping_spacket;
void handlePing (struct ping_spacket *packet);
void startPing (void);
void stopPing (void);
void sendServerPingResponse (int number);
void calc_lag (void);

/******************************************************************************/
/***  pingstats.c
/******************************************************************************/
int pStatsHeight (void);
int pStatsWidth (void);
void initPStats (void);
void redrawPStats (void);
void updatePStats (void);
// int box(int filled, int x, int y, int wid, int hei, W_Color color);
//static void text(int value, int y);

/******************************************************************************/
/***  planetlist.c
/******************************************************************************/
void planetlist (void);
void updatePlanetw (void);

/******************************************************************************/
/***  playback.c
/******************************************************************************/
int pbmain (char *name);
void pbsetspeed (char key);
void pblockplayer (int who);
void pblockplanet (int pl);
inline int ckRecordPacket (char packet);
int pb_dopacket (char *buf);
int readFromFile ();
void pb_framectr(int xloc, int yloc);   /* show frame counter on dashboard */
#ifdef REVERSE_PLAYBACK
void rpb_init (void);
void rpb_insert (int diskpos,
                 int hdl);
void rpb_analyze (int diskpos,
                  void *packet);
int intcomp (const void *a,
             const void *b);
void rpb_dorev (char *buf);
#endif

/******************************************************************************/
/***  playerlist.c
/******************************************************************************/
void InitPlayerList ();
int PlistMaxWidth ();
int PlistMaxWidth2 ();
void RedrawPlayerList ();
void UpdatePlistFn ();
int GetPlayerFromPlist (int x, int y);

/******************************************************************************/
/***  ranklist.c
/******************************************************************************/
void ranklist (void);

/******************************************************************************/
/***  redraw.c
/******************************************************************************/
#ifdef BRMH
void intrupt (fd_set * readfds);
#else
void intrupt (void);
#endif
void redraw (void);
void stline (int flag);
void redrawTstats (void);
void updateMaxStats (int redraw);

/******************************************************************************/
/***  reserved.c
/******************************************************************************/
struct reserved_spacket;
struct reserved_cpacket;
void makeReservedPacket (struct reserved_spacket *packet);
void encryptReservedPacket (struct reserved_spacket *spacket,
                            struct reserved_cpacket *cpacket,
                            int pno);

/******************************************************************************/
/***  rotate.c
/******************************************************************************/
void rotate_dir (unsigned char *d,
                 int r);
void rotate_coord (int *x,
                   int *y,
                   int d,
                   int cx,
                   int cy);
void rotateGalaxy (void);

/******************************************************************************/
/***  rsa_box.c
/******************************************************************************/
void rsa_black_box (unsigned char *out,
                    unsigned char *in,
                    unsigned char *public,
                    unsigned char *global);
void rsa_partial_box_0 (MP_INT * m,
                        MP_INT * r,
                        MP_INT * g);
void rsa_partial_box_1 (MP_INT * m,
                        MP_INT * r,
                        MP_INT * g);
void rsa_partial_box_2 (MP_INT * m,
                        MP_INT * r,
                        MP_INT * g);
void rsa_partial_box_3 (MP_INT * m,
                        MP_INT * r,
                        MP_INT * g);
void rsa_partial_box_4 (MP_INT * m,
                        MP_INT * r,
                        MP_INT * g);

/******************************************************************************/
/***  senddist.c
/******************************************************************************/
struct distress *loaddistress (enum dist_type i, W_Event * data);
void emergency (enum dist_type i, W_Event * data);
int pmacro (int mnum, char who, W_Event * data);

/******************************************************************************/
/***  short.c
/******************************************************************************/
void sendThreshold (short unsigned int v);
void handleVTorp (unsigned char *sbuf);
struct youshort_spacket;
void handleSelfShort (struct youshort_spacket *packet);
struct youss_spacket;
void handleSelfShip (struct youss_spacket *packet);
void handleVPlayer (unsigned char *sbuf);
struct mesg_s_spacket;
void handleSMessage (struct mesg_s_spacket *packet);
struct shortreply_spacket;
void handleShortReply (struct shortreply_spacket *packet);
void handleVTorpInfo (unsigned char *sbuf);
void handleVPlanet (unsigned char *sbuf);
void resetWeaponInfo (void);
void sendShortReq (char state);
struct warning_s_spacket;
void handleSWarning (struct warning_s_spacket *packet);
void add_whydead (char *s,
                  int m);
void handleVKills (unsigned char *sbuf);
void handleVPhaser (unsigned char *sbuf);
struct stats_s_spacket;
void handle_s_Stats (struct stats_s_spacket *packet);
void new_flags (unsigned int data,
                int which);

/******************************************************************************/
/***  smessage.c
/******************************************************************************/
void DisplayMessage ();
void smessage (char ichar);
void pmessage (char *str, 
               short recip, 
               short group);
char *getaddr2 (int flags,
                int recip);
void message_on (void);
void message_off (void);
#ifdef XTRA_MESSAGE_UI
void message_hold (void);
#endif
int getgroup (char addr,
              int *recip);
void pnbtmacro (int c);

/******************************************************************************/
/***  socket.c
/******************************************************************************/
void resetForce (void);
void checkForce (void);
void setNoDelay (int fd);
void connectToServer (int port);
void callServer (int port,
                 char *server);
int isServerDead (void);
void socketPause (void);
int readFromServer (fd_set * readfds);
void dotimers (void);
int getvpsize (char *bufptr);
int doRead (int asock);
struct torp_spacket;
void handleTorp (struct torp_spacket *packet);
struct torp_info_spacket;
void handleTorpInfo (struct torp_info_spacket *packet);
struct status_spacket;
void handleStatus (struct status_spacket *packet);
struct you_spacket;
void handleSelf (struct you_spacket *packet);
struct player_spacket;
void handlePlayer (struct player_spacket *packet);
struct warning_spacket;
void handleWarning (struct warning_spacket *packet);
void sendShortPacket (char type,
                      char state);
void sendServerPacket (struct player_spacket *packet);
struct planet_spacket;
void handlePlanet (struct planet_spacket *packet);
struct phaser_spacket;
void handlePhaser (struct phaser_spacket *packet);
struct mesg_spacket;
void handleMessage (struct mesg_spacket *packet);
struct queue_spacket;
void handleQueue (struct queue_spacket *packet);
void sendTeamReq (int team,
                  int ship);
struct pickok_spacket;
void handlePickok (struct pickok_spacket *packet);
void sendLoginReq (char *name,
                   char *pass,
                   char *login,
                   char query);
struct login_spacket;
void handleLogin (struct login_spacket *packet);
void sendTractorReq (char state,
                     char pnum);
void sendRepressReq (char state,
                     char pnum);
void sendDetMineReq (short int torp);
struct plasma_info_spacket;
void handlePlasmaInfo (struct plasma_info_spacket *packet);
struct flags_spacket;
void handleFlags (struct flags_spacket *packet);
struct plasma_spacket;
void handlePlasma (struct plasma_spacket *packet);
struct kills_spacket;
void handleKills (struct kills_spacket *packet);
struct pstatus_spacket;
void handlePStatus (struct pstatus_spacket *packet);
struct motd_spacket;
void handleMotd (struct motd_spacket *packet);
void sendMessage (char *mes,
                  int group,
                  int indiv);
struct mask_spacket;
void handleMask (struct mask_spacket *packet);
void sendOptionsPacket (void);
void pickSocket (int old);
struct badversion_spacket;
void handleBadVersion (struct badversion_spacket *packet);
long gwrite (int fd,
             char *buf,
             register int bytes);
struct hostile_spacket;
void handleHostile (struct hostile_spacket *packet);
struct plyr_login_spacket;
void handlePlyrLogin (struct plyr_login_spacket *packet,
                      int sock);
struct stats_spacket;
void handleStats (struct stats_spacket *packet);
struct plyr_info_spacket;
void handlePlyrInfo (struct plyr_info_spacket *packet);
void sendUpdatePacket (LONG speed);
struct planet_loc_spacket;
void handlePlanetLoc (struct planet_loc_spacket *packet);
void handleReserved (struct reserved_spacket *packet,
                     int sock);
struct ship_cap_spacket;
void handleShipCap (struct ship_cap_spacket *packet);
struct generic_32_spacket;
void handleGeneric32 (struct generic_32_spacket *packet);
struct rsa_key_spacket;
void handleRSAKey (struct rsa_key_spacket *packet);
#ifdef INCLUDE_SCAN
void
handleScan (packet)
     struct scan_spacket *packet;
#endif
void sendUdpReq (int req);
struct udp_reply_spacket;
void handleUdpReply (struct udp_reply_spacket *packet);
int openUdpConn (void);
#ifdef UDP_PORTSWAP
int connUdpConn ();
#endif
int recvUdpConn (void);
int closeUdpConn (void);
void printUdpInfo (void);
struct sequence_spacket;
void handleSequence (struct sequence_spacket *packet);
void Log_Packet (char type, int act_size);
void Log_OPacket (int tpe, int size);
void print_opacket (char *packet, int size);
void print_packet (char *packet, int size); 
void Dump_Packet_Log_Info (void);
char *strcpyp_return (register char *s1,
                      register char *s2,
                      register int length);
char *strcpy_return (register char *s1,
                     register char *s2);

/******************************************************************************/
/***  spopt.c
/******************************************************************************/
     void sprefresh (int i);
     void spaction (W_Event * data);
     void spwindow (void);
     void spdone (void);

/******************************************************************************/
/***  stats.c
/******************************************************************************/
     void initStats ();
     void redrawStats ();
     void updateStats ();
//static void box(int filled, int x, int y, int wid, int hei,W_Color color);
     void calibrate_stats ();

/******************************************************************************/
/***  string_util.c
/******************************************************************************/
     char *itoapad (int val,
                    char *result,
                    int pad,
                    int prec);
     char *ftoa (float fval,
                 char *result,
                 int pad,
                 int iprec,
                 int dprec);
     char *format (char *buf,
                   char *from,
                   int width,
                   int right_justify);

/******************************************************************************/
/***  tools.c
/******************************************************************************/
#ifdef TOOLS
     void sendTools (char *str);
showToolsWin (void);
#endif

/******************************************************************************/
/***  udpopt.c
/******************************************************************************/
void udpwindow (void);
void udprefresh (int i);
void udpaction (W_Event * data);
void udpdone (void);

/******************************************************************************/
/***  util.c
/******************************************************************************/
int angdist (unsigned char x,
             unsigned char y);
struct obtype *gettarget (W_Window ww,
                           int x,
                           int y,
                           int targtype);
struct obtype *gettarget2 (int x,
                           int y,
                           int targtype);
short troop_capacity (void);
void setObserverMode (int);

/******************************************************************************/
/***  war.c
/******************************************************************************/
void fillwin (int menunum,
              char *string,
              int hostile,
              int warbits,
              int team);
void warrefresh (void);
void warwindow (void);
void waraction (W_Event * data);

/******************************************************************************/
/***  warning.c
/******************************************************************************/
void warning (char *text);

/******************************************************************************/
/***  winmain.c
/******************************************************************************/
int main (int argc,
          char *argv[]);
void WinMainCleanup (void);
//int getpid ();
struct passwd *getpwuid ();
void sleep (int seconds);
double rint (double r);
//void perror (const char *str);
#ifdef NEW_SELECT
int PASCAL select (int nfds,
                   fd_set * readfds,
                   fd_set * writefds,
                   fd_set * exceptfds,
                   struct timeval *timeout);
#endif
char *GetExeDir ();
void HideConsoleWindow (void);
BOOL CALLBACK EnumWindowsProc (HWND hwnd, LPARAM lparam);

#endif /* __INCLUDED_proto_h__ */
