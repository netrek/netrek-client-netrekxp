List for the netrekrc/installer:

1) Change recommendation on metaserver from off to on!
2) there should be some sort of chance to give default resolution in GUI,
which then maps windows accordingly.
3) Default keymap/buttonamp is horrible.
4) Add chance for player to input their default login name - tired of seeing
new-be-nice!!


Things not gonna hapen:
1) Player list messaging doesn't work if you can't see player (cloaked, too far
away, etc).  Don't think this is fixable.
2) Get mkkey working.

Things to do:
1) Add warning hull.
2) resizeable message windows.  Perhaps memory of window positions if you choose
to save settings.  Text in windows should adjust to new window size.
5) A lot of annoying pop up windows (like macro window, default controls) should be
closeable by shift right click.
6) genoed as obs on team with no planets -> you were killed by something not in this
game
7a) red_alert sound sounds kinda static-y
7b) self destruct and enter ship sounds are near identical.
11) Save options 
- name/login/meta stuff needs to be saved
- Default windows placements need to be saved
- uselite options need to be saved
- review window mapping needs to be saved
- option server/servernick/servertype needs to be saved
12) Icon not working under Visual Studio
14) observer sounds slightly messed up
14a) torp_other not working right due to lack of torp fuse info
15) closing hint window at warp 0 turns off the box on the galaxy map.  Also, cloaking/shields
at warp 0 doesn't update someone else's tactical on what you did.
15a) 
16) why is beeplite going on when i'm obsing someone who cloaks after they used a 
beeplite macro that lit themselves up
17) add dll, lib, and header files to repository

Might be done:
3) mouse scroll in message windows gets the up/down coord wrong when main title
bar is turned on.
4) CRASH BUG! Resize main window and try to do mouse scroll, it will crash client.
13) Disable window move stuff still broken.
11) save_options keymap: not saving right
13) if waiting on queue and get in, quit clock missing

Stas's list:
- color coded playerlist.
! change default playerListStlyle to 1 (nah, this won't happen)
- check inl server for behavior
+ add option to parse netrekrc.txt and xtrekrc.txt files
! server: ita won't block RCDs (why not ?)
- add usage on metaserver window (well, what would you write there ?)
! after res if there cloaker around he will be seen briefly (speed 0 bug)
- whydead dies when out of range (not confirmed)
- add "reset to defaults" to options menu
+ add installer
- maybe add two column playerlist
- add bitmap themes option (change default bitmap naming and processing)
- add shiftMouse, controlMouse on/off
- add timestamps for messages (no place for timestamp)
- fix accelerated scroll for microsoft mice
- add msg.phaser to enable formatting of phaser messages
- add geno bitmap
- add detCircle
- add copy/paste possibility from message windows (richtext ?)
- add clickable urls from message windows (richtext ?)
+ fix bug with undefined behavior in case of odd keymap/buttonmap/ckeymap length (partially fixed)
- add "mKeyOnlyToStartMessages: on/off" to ignore keys [0-9][a-z][ATFOKRG] in message window when the messageon=0
- scrollable player list
- parse Announcer messages in hockey games
- check geno/shift-q wrong behavior
- resize documentation window
- pasting multiline prints \r\n instead of splitting lines
- add sortPlayersFirst
- fix makedistress bug that creates 0 instead of empty string when macro starts with conditional

