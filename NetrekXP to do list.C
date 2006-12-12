List for the netrekrc/installer:
1) there should be some sort of chance to give default resolution in GUI,
which then maps windows accordingly.
2) fix netrekrc append so that it doesn't keep adding new lines to the netrekrc
3) Get source code for installer from Joe.
4) Add a Cambot playback and Cambot record shortcut links to the installation package.

Things not gonna happen, or server problems:
1) Player list messaging doesn't work if you can't see player (cloaked, too far
away, etc).  Don't think this is fixable.
2) torp_other and plasma_other not working right due to lack of torp fuse info
3) twarping to base docked on base causes the twarp problem, but hard to reproduce
4) Server sometimes not updating tract/press flags for observers, several instances
where a ship is seen to have pressors on constantly, even when they are off
5) Triple mode blanks planet names, if you vote to dogfight or hockey, names are not restored
6) "The 0 0 20 minutes before empire collapses" - with surrenderstart = 1
7) mars - lose nb_robots
8) newbie/pret: monitor vs full_hostname
9) on daemon exit in newbie/pret, kill any hosed bot slots
10) end game bots are stupid, and on timericide don't act right
11) if obs in game when merlin quits, queues still open..should be closed dammit
12) final planet count and cscore would be nice in pw_stats

Things that are sorta fixed, but could use improvement:
1) Border issue - quite complex problem, how to know the borders to obey after the bitmap
is rotated.  Only solution so far is to ignore borders with scaled bitmaps.
2) AddFontResourceEx caused problems with win 98 machines.  Commented out, and using
old AddFontResource function.
3) planet resource placement and refresh (for new planet bitmaps) will need adjusting if 
local is scaled

Things to do:
1) resizeable message windows.  Perhaps memory of window positions if you choose
to save settings.  Text in windows should adjust to new window size.
2) Save options for observer/servernick/servertype need to be saved
3) Get square planet bitmaps so I can turn them into mapped rotating globes.
4) problem with message windows, text getting garbled near top, goes away with
flushing with scroll button.  Doesn't show up initially, but do something like use
scroll button, alt-enter, change to review all..and it shows up.
4a) review all scrolling messes up text, if bottom of window is below the visible
bottom border of netrek window.  Actually will happen to whatever window is mapped 
down there at bottom of screen.  Apprently the native windows function doesn't properly
scroll text that is outside the viewable window.
5) Have client utilize new server torp vector code
6) Pop-up dialog box for bad version response
7) new UDP metaserver code has several problems re loading/saving metacache, status
field not updating due to refresh not happening when it should, metawindow still can't
handle correctly the case where servers would overwrite the quit button

Stas's list:
- color coded playerlist.
! server: ita won't block RCDs (why not ?)
! after res if there cloaker around he will be seen briefly (speed 0 bug)
- add "reset to defaults" to options menu
- add bitmap themes option (change default bitmap naming and processing)
- add shiftMouse, controlMouse on/off
- fix accelerated scroll for microsoft mice
- add msg.phaser to enable formatting of phaser messages
- add copy/paste possibility from message windows (richtext ?)
- add clickable urls from message windows (richtext ?)
- add "mKeyOnlyToStartMessages: on/off" to ignore keys [0-9][a-z][ATFOKRG] in message window when the messageon=0
- scrollable player list
- parse Announcer messages in hockey games
- resize documentation window
- pasting multiline prints \r\n instead of splitting lines
