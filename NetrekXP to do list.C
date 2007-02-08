List for the netrekrc/installer:
1) there should be some sort of chance to give default resolution in GUI,
which then maps windows accordingly.
2) fix netrekrc append so that it doesn't keep adding new lines to the netrekrc
3) Get source code for installer from Joe.

Things not gonna happen, or server problems:
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
13) add auto guest# name generation for INL server
14) sysdef option for base orbit enemy planets
15) phaser misses have the client computer phaser length, but with phaser upgrades,
this causes a mismatch in the actual phaser length for other players.  Server needs
to somehow relay phaser damage values for other ships, perhaps in generic_32?

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
4) Pop-up dialog box for bad version response
5) new UDP metaserver code has issue if 2nd metaserver responding has more servers, can be
awkward to resize window if user going to click on a server.  However if cache is working
properly, after the first connect, all servers should be stored and the metaserver window
should not resize due to differences between the metaservers.
5a) Metaping on a Unknown server is causing unseemly delay due to threading.
7) Some sort of click action on player list brings up a menu with settings for that
player's candock, cantranswarp, ban, ignore, etc.
8) Maybe fix galactic new planet bitmap redraw sequence, caused incredible lag on a P166
Win98 xp computer
9) Out of bounds message window calcs don't work with taskbar or menu bar, basewin
rect doesn't take these into account.

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
