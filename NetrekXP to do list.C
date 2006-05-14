List for the netrekrc/installer:

1) Change recommendation on metaserver from off to on!
2) there should be some sort of chance to give default resolution in GUI,
which then maps windows accordingly.
3) Default keymap/buttonamp is horrible.
4) Add chance for player to input their default login name - tired of seeing
new-be-nice!!


Things not gonna happen, or server problems:
1) Player list messaging doesn't work if you can't see player (cloaked, too far
away, etc).  Don't think this is fixable.
3) torp_other and plasma_other not working right due to lack of torp fuse info
4) cloaking/shields at warp 0 doesn't update someone else's tactical on what you did -
server not sending right info - short packets!
4a) observer sound messes up when someone flips shields at warp 0, again a server problem
4b) Locking onto robot cause the observer sound to mess up for shields up/down (this is
server side problem, for the shield packet not being sent)
5) Observers don't get any geno message at all (head to default which is error message) - short packets!
6) Can't bomb enemy 3rd space planets in your T-mode opponent's space.

Things to do:
1) Add warning hull.
2) resizeable message windows.  Perhaps memory of window positions if you choose
to save settings.  Text in windows should adjust to new window size.
3) flipping through options menus clears too large a path to the right on the galactic,
noticeable with viewbox disappearing at low speeds , depending on what length of menu you
flip to.  Also, closing hint window has same effect
4) Save options 
- name/login/meta stuff needs to be saved
- Default windows placements need to be saved
- uselite options need to be saved
- review window mapping needs to be saved
- option server/servernick/servertype needs to be saved
5) A lot of annoying pop up windows (like macro window, default controls) should be
closeable by shift right click.
6) Implement Defcom's new planet and ship bitmaps
6a) Border issue - quite complex problem, how to know the borders to obey after the bitmap
   is rotated, and where to center bitmap
6b) The new smooth turning only working on self, not obs or others - short packets!
7) login not read in before metaserver info (for metablocking purposes)

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
- add bitmap themes option (change default bitmap naming and processing)
- add shiftMouse, controlMouse on/off
- add timestamps for messages (no place for timestamp)
- fix accelerated scroll for microsoft mice
- add msg.phaser to enable formatting of phaser messages
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

Simple in-game, realtime variable checking:
   char text[120];
   sprintf (text, "Variable has value %d.", variable);
   W_MessageAllowedWindows (WAM_ALL, 0, 0, textColor, text, 120, 0);