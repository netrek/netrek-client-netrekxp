List for the netrekrc/installer:
1) there should be some sort of chance to give default resolution in GUI,
which then maps windows accordingly.
2) fix netrekrc append so that it doesn't keep adding new lines to the netrekrc

Things not gonna happen, or server problems:
1) Player list messaging doesn't work if you can't see player (cloaked, too far
away, etc).  Don't think this is fixable.
2) torp_other and plasma_other not working right due to lack of torp fuse info
3) Can't bomb enemy 3rd space planets in your T-mode opponent's space.
4) in orbit.c, pftranswarp strangeness.
5) look into swar/war in repair time, with obs and plr - doesn't seem to be any way
for an obs to know what the war decs are of the person he is observing. Best solution
is to just have server send repair time and planet orbit info.
6) twarping to base docked on base causes the twarp problem, but hard to reproduce
7) Server sometimes not updating tract/press flags for observers, several instances
where a ship is seen to have pressors on constantly, even when they are off
8) Triple mode blanks planet names, if you vote to dogfight or hockey, names are not restored
9) "The 0 0 20 minutes before empire collapses" - with surrenderstart = 1
11) all observer ghostbusts now do a double ghostbust: no ping observ, no ping alive
12) mars - lose nb_robots
13) newbie/pret: monitor vs full_hostname
14) on daemon exit in newbie/pret, kill any hosed bot slots
15) end game bots are stupid, and on timericide don't act right
16) if obs in game when merlin quits, queues still open..should be closed dammit

Sturgeon problems
1) after free upgrade, ship cap packet not being sent to everyone..seems to
   only be sent to 1 person.
1b) someone got my super ship cap packet when he had a normal GA
2) need ship cap packet sent after xtkill super
  
Things that go wrong when short packets are on:
1) cloaking/shields at warp 0 doesn't update someone else's tactical on what you did if
there is no movement anywhere on map, and observer sound also messes up
2) The new smooth turning only working on self, not obs or others - short packets only
send headings of 16 positions.
3) Others speed only sent to 16..messed up for obs locked onto puck in hockey,
or twarpers

Things that are sorta fixed, but could use improvement:
1) Bug with waraction..was being called even when clicking on border (broke function)
..I added a default return to switch statement, but the underlying bug still remains.
2) Border issue - quite complex problem, how to know the borders to obey after the bitmap
is rotated.  Only solution so far is to ignore borders with scaled bitmaps.
3) AddFontResourceEx caused problems with win 98 machines.  Commented out, and using
old AddFontResource function.
4) planet resource placement and refresh (for new planet bitmaps) will need adjusting if 
local is scaled

Things to do:
1) resizeable message windows.  Perhaps memory of window positions if you choose
to save settings.  Text in windows should adjust to new window size.
2) Save options for observer/servernick/servertype need to be saved
3) Although the borland build works without errors, the netrek executable crashes, and 
it is beyond my knowledge to know how to debug it.
4) Get source code for installer from Joe.
5) Get square planet bitmaps so I can turn them into mapped rotating globes.
7) problem with message windows, text getting garbled near top, goes away with
flushing with scroll button.  Doesn't show up initially, but do something like use
scroll button, alt-enter, change to review all..and it shows up.
7a) review all scrolling messes up text, if bottom of window is below the visible
bottom border of netrek window.  Actually will happen to whatever window is mapped 
down there at bottom of screen.  Apprently the native windows function doesn't properly
scroll text that is outside the viewable window.
8) Have client utilize new server torp vector code
10) Fade out ship explosion sound on quit
11) Pop-up dialog box for bad version response


Stas's list:
- color coded playerlist.
- check inl server for behavior (observers and tractor/pressor packets)
! server: ita won't block RCDs (why not ?)
- add usage on metaserver window (well, what would you write there ?)
! after res if there cloaker around he will be seen briefly (speed 0 bug)
- whydead dies when out of range (not confirmed)
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
